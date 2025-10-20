import argparse
import ipaddress
import json
import os
import select
import socket
import struct
import threading
import time
import uuid

DEFAULT_MCAST = "239.255.42.99"
DEFAULT_PORT = 5007


def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("8.8.8.8", 80))
        return s.getsockname()[0]
    finally:
        s.close()


def get_ip_params(ip):
    try:
        import psutil

        for addrs in psutil.net_if_addrs().values():
            for addr in addrs:
                try:
                    fam = addr.family
                    if fam == socket.AF_INET and addr.address == ip:
                        netmask = addr.netmask
                        broadcast = addr.broadcast
                        return netmask, broadcast
                except Exception:
                    continue
    except Exception:
        pass

    return None, None


def derive_broadcast(ip, netmask):
    try:
        iface = ipaddress.IPv4Interface(f"{ip}/{netmask}")
        return str(iface.network.broadcast_address)
    except Exception:
        return "255.255.255.255"


class ChatNode:
    def __init__(
        self, mode="broadcast", port=DEFAULT_PORT, mgroup=DEFAULT_MCAST, name=None
    ):
        self.name = name or os.getenv("USER") or socket.gethostname()
        self.mode = mode
        self.port = port
        self.mgroup = mgroup
        self.id = uuid.uuid4().hex
        self.ip = get_local_ip()
        self.netmask, bc = get_ip_params(self.ip)
        self.broadcast = bc or (
            derive_broadcast(self.ip, self.netmask)
            if self.netmask
            else "255.255.255.255"
        )
        self.running = False
        self.ignore = set()
        self.peers = set()
        self.lock = threading.Lock()
        self._host_cache = {}
        self.rx_sock = None
        self.tx_sock = None
        self.rx_thr = None
        self.hb_thr = None

    def setup(self):
        self.close_sockets()
        if self.mode == "broadcast":
            self.rx_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.rx_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            try:
                self.rx_sock.setsockopt(
                    socket.SOL_SOCKET,
                    getattr(socket, "SO_REUSEPORT", socket.SO_REUSEADDR),
                    1,
                )
            except Exception:
                pass
            self.rx_sock.bind(("", self.port))
            self.tx_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.tx_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        else:  # multicast
            self.rx_sock = socket.socket(
                socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP
            )
            self.rx_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            try:
                self.rx_sock.setsockopt(
                    socket.SOL_SOCKET,
                    getattr(socket, "SO_REUSEPORT", socket.SO_REUSEADDR),
                    1,
                )
            except Exception:
                pass
            self.rx_sock.bind(("", self.port))
            self._mcast_join()
            self.tx_sock = socket.socket(
                socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP
            )
            ttl = struct.pack("b", 1)
            self.tx_sock.setsockopt(
                socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl
            )  # multicast only in local subnet
            try:
                self.tx_sock.setsockopt(
                    socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(self.ip)
                )
            except Exception:
                pass

    def _mcast_join(self):
        group = socket.inet_aton(self.mgroup)
        mreq = group + socket.inet_aton("0.0.0.0")
        self.rx_sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    def _mcast_leave(self):
        try:
            group = socket.inet_aton(self.mgroup)
            mreq = group + socket.inet_aton("0.0.0.0")
            self.rx_sock.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)
        except Exception:
            pass

    def close_sockets(self):
        if self.rx_sock:
            try:
                self._mcast_leave()
            except Exception:
                pass
            try:
                self.rx_sock.close()
            except Exception:
                pass
        if self.tx_sock:
            try:
                self.tx_sock.close()
            except Exception:
                pass
        self.rx_sock = None
        self.tx_sock = None

    def _packet(self, typ, **kwargs):
        pkt = {
            "type": typ,
            "id": self.id,
            "sender": self.ip,
            "name": self.name,
            **kwargs,
        }
        return json.dumps(pkt).encode()

    def resolve_host(self, ip):
        try:
            return self._host_cache[ip]
        except KeyError:
            try:
                host = socket.gethostbyaddr(ip)[0]
            except Exception:
                host = ip
            self._host_cache[ip] = host
            return host

    def send_ctrl(self, typ, **kwargs):
        self._send_raw(self._packet(typ, **kwargs))

    def send_chat(self, text):
        self._send_raw(self._packet("chat", text=text))

    def _send_raw(self, data: bytes):
        if self.mode == "broadcast":
            dst = (self.broadcast, self.port)
        else:
            dst = (self.mgroup, self.port)
        try:
            self.tx_sock.sendto(data, dst)
        except Exception as e:
            print(f"Send error: {e}")

    def start(self):
        self.running = True
        self.setup()
        self.rx_thr = threading.Thread(target=self._rx_loop, daemon=True)
        self.rx_thr.start()
        self.hb_thr = threading.Thread(target=self._hb_loop, daemon=True)
        self.hb_thr.start()
        self.send_ctrl("hello")

    def stop(self):
        self.send_ctrl("goodbye")
        self.running = False
        try:
            self.rx_thr.join(timeout=0.5)
        except Exception:
            pass
        try:
            self.hb_thr.join(timeout=0.5)
        except Exception:
            pass
        self.close_sockets()

    def _hb_loop(self):
        while self.running:
            self.send_ctrl("hello")
            time.sleep(5)

    def _handle_parsed_message(self, msg, addr):
        ip = addr[0]
        s_ip = msg.get("sender", ip)
        s_id = msg.get("id")
        if s_id == self.id:
            return
        typ = msg.get("type")
        with self.lock:
            self.peers.add(s_ip)
            ignored = s_ip in self.ignore
        if ignored and typ != "unignore_me":
            return
        if typ == "chat":
            nm = msg.get("name", s_ip)
            host = self.resolve_host(s_ip)
            text = msg.get("text", "")
            print(f"[{nm}@{host}] {text}")
        elif typ == "hello":
            return
        elif typ == "goodbye":
            with self.lock:
                self.peers.discard(s_ip)
        elif typ == "ignore_me":
            with self.lock:
                self.ignore.add(s_ip)
        elif typ == "unignore_me":
            with self.lock:
                self.ignore.discard(s_ip)

    def _rx_loop(self):
        while self.running:
            try:
                r, _, _ = select.select([self.rx_sock], [], [], 0.5)
                if not r:
                    continue
                data, addr = self.rx_sock.recvfrom(65535)
            except OSError:
                if not self.running:
                    break
                time.sleep(0.5)
                continue
            try:
                msg = json.loads(data.decode(errors="ignore"))
            except Exception:
                continue
            self._handle_parsed_message(msg, addr)

    def cmd_info(self):
        print(
            f"IP: {self.ip}\nNetmask: {self.netmask}\nBroadcast: {self.broadcast}\nMode: {self.mode}\nGroup: {self.mgroup}\nPort: {self.port}"
        )

    def cmd_peers(self):
        with self.lock:
            if not self.peers:
                print("No peers discovered yet.")
            else:
                print("Peers:")
                for p in sorted(self.peers):
                    host = self.resolve_host(p)
                    flag = " (ignored)" if p in self.ignore else ""
                    if host != p:
                        print(f" - {host}{flag} [{p}]")
                    else:
                        print(f" - {p}{flag}")

    def cmd_mode(self, mode):
        if mode not in ("broadcast", "multicast"):
            print("Mode must be 'broadcast' or 'multicast'")
            return
        if self.mode == mode:
            return
        self.mode = mode
        self.setup()
        print(f"Switched to {mode}")

    def cmd_join(self):
        if self.mode != "multicast":
            print("Join only applies to multicast mode.")
            return
        try:
            self._mcast_join()
            print("Joined multicast group")
        except Exception as e:
            print(f"Join failed: {e}")

    def cmd_leave(self):
        if self.mode != "multicast":
            print("Leave only applies to multicast mode.")
            return
        try:
            self._mcast_leave()
            print("Left multicast group")
        except Exception as e:
            print(f"Leave failed: {e}")

    def cmd_ignore(self, ip):
        with self.lock:
            self.ignore.add(ip)
        print(f"Locally ignoring {ip}")

    def cmd_unignore(self, ip):
        with self.lock:
            self.ignore.discard(ip)
        print(f"Removed local ignore for {ip}")

    def cmd_force_ignore_me(self):
        self.send_ctrl("ignore_me")
        print("Requested others to ignore me")

    def cmd_unforce_ignore_me(self):
        self.send_ctrl("unignore_me")
        print("Requested others to stop ignoring me")


def repl(node: ChatNode):
    print(
        "Commands: /peers, /info, /mode [broadcast|multicast], /join, /leave, /ignore <ip>, /unignore <ip>, /forceignoreme, /unforceignoreme, /discover, /quit"
    )
    try:
        while True:
            try:
                line = input()
            except KeyboardInterrupt:
                break
            if not line:
                continue
            if line.startswith("/"):
                parts = line[1:].split()
                cmd = parts[0].lower()
                arg = parts[1:] if len(parts) > 1 else []
                if cmd == "peers":
                    node.cmd_peers()
                elif cmd == "info":
                    node.cmd_info()
                elif cmd == "mode" and arg:
                    node.cmd_mode(arg[0])
                elif cmd == "join":
                    node.cmd_join()
                elif cmd == "leave":
                    node.cmd_leave()
                elif cmd == "ignore" and arg:
                    node.cmd_ignore(arg[0])
                elif cmd == "unignore" and arg:
                    node.cmd_unignore(arg[0])
                elif cmd == "forceignoreme":
                    node.cmd_force_ignore_me()
                elif cmd == "unforceignoreme":
                    node.cmd_unforce_ignore_me()
                elif cmd == "discover":
                    node.send_ctrl("hello")
                elif cmd == "quit":
                    break
                else:
                    print("Unknown command or missing args")
            else:
                node.send_chat(line)
    finally:
        node.stop()


def main():
    ap = argparse.ArgumentParser(
        description="P2P UDP chat with broadcast and multicast modes"
    )
    ap.add_argument("--mode", choices=["broadcast", "multicast"], default="broadcast")
    ap.add_argument("--port", type=int, default=DEFAULT_PORT)
    ap.add_argument("--group", default=DEFAULT_MCAST, help="Multicast group")
    ap.add_argument("--name", default=None)
    ap.add_argument(
        "--info", action="store_true", help="Print detected network params and exit"
    )
    args = ap.parse_args()
    node = ChatNode(mode=args.mode, port=args.port, mgroup=args.group, name=args.name)
    if args.info:
        node.cmd_info()
        return 0
    print(
        f"Your name: {node.name} | IP: {node.ip} | Netmask: {node.netmask} | Broadcast: {node.broadcast}"
    )
    print(f"Mode: {node.mode} | Port: {node.port} | Group: {node.mgroup}")
    node.start()
    repl(node)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

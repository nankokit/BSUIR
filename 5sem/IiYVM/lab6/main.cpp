#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "BluetoothAPIs.lib")

#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <time.h>

class BluetoothSocket {
public:
	SOCKET socket;

	BluetoothSocket() : socket(INVALID_SOCKET) {}

	virtual ~BluetoothSocket() {
		if (socket != INVALID_SOCKET) {
			closesocket(socket);
		}
	}

	void initializeWinsock() {
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
			fprintf(stderr, "Failed to initialize Winsock\n");
			exit(EXIT_FAILURE);
		}
	}

	void cleanup() {
		WSACleanup();
	}
};

class Sender : public BluetoothSocket {
public:
	Sender(BLUETOOTH_DEVICE_INFO device_info) {
		SOCKADDR_BTH sock_addr = {};
		sock_addr.addressFamily = AF_BTH;
		sock_addr.serviceClassId = RFCOMM_PROTOCOL_UUID;
		sock_addr.port = 10;
		sock_addr.btAddr = device_info.Address.ullLong;

		socket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		if (connect(socket, (sockaddr*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR) {
			fprintf(stderr, "Error connecting to receiver\n");
			closesocket(socket);
			exit(EXIT_FAILURE);
		}
		printf("Connected to receiver\n");
	}

	int sendFile(FILE* fp) {
		fseek(fp, 0, SEEK_END);
		int file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char file_size_str[256];
		_itoa(file_size, file_size_str, 10);
		send(socket, file_size_str, sizeof(file_size_str), NULL);

		char buffer[1024];
		int sent_bytes = 0;

		while (!feof(fp)) {
			size_t bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fp);
			int bytes_sent = send(socket, buffer, bytes_read, NULL);
			if (bytes_sent == SOCKET_ERROR) return -1;

			sent_bytes += bytes_sent;
			system("CLS");
			printf("Sending the file...\nBytes sent: %d / %d\n", sent_bytes, file_size);
		}
		return 0;
	}
};

class Receiver : public BluetoothSocket {
public:
	Receiver() {
		SOCKADDR_BTH sock_addr = {};
		sock_addr.addressFamily = AF_BTH;
		sock_addr.serviceClassId = RFCOMM_PROTOCOL_UUID;
		sock_addr.port = 10;

		socket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		bind(socket, (sockaddr*)&sock_addr, sizeof(sock_addr));
		listen(socket, SOMAXCONN);
	}

	SOCKET acceptConnection() {
		SOCKADDR_BTH client_addr = {};
		int addr_len = sizeof(client_addr);
		SOCKET client_socket = accept(socket, (sockaddr*)&client_addr, &addr_len);
		return client_socket;
	}

	int receiveFile(SOCKET client) {
		char file_size_str[256] = {};
		if (recv(client, file_size_str, sizeof(file_size_str), NULL) == SOCKET_ERROR) {
			return -1;
		}

		int file_size = atoi(file_size_str);
		if (file_size <= 0) return 0;

		FILE* fp = fopen("received_file.mp3", "wb");
		if (!fp) {
			fprintf(stderr, "Error opening file for writing\n");
			return -1;
		}

		char buffer[1024];
		int received_bytes = 0;
		int last_printed_bytes = received_bytes;
		while (file_size > 0) {
			int bytes_to_receive = min(1024, file_size);
			int bytes_received = recv(client, buffer, bytes_to_receive, NULL);
			if (bytes_received <= 0) {
				fclose(fp);
				return -1;
			}
			fwrite(buffer, sizeof(char), bytes_received, fp);
			file_size -= bytes_received;
			received_bytes += bytes_received;

			if (received_bytes - last_printed_bytes > 3)
			{
				system("CLS");
				printf("Receiving the file...\nBytes received: %d / %d\n", received_bytes, atoi(file_size_str));
			}
		}

		fclose(fp);
		return 0;
	}
};

int getBluetoothDevices(BLUETOOTH_DEVICE_INFO** devices) {
	int count = 0;
	BLUETOOTH_DEVICE_SEARCH_PARAMS filter = {};
	filter.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
	filter.fReturnAuthenticated = true;
	filter.fReturnConnected = true;
	filter.fReturnRemembered = false;
	filter.fReturnUnknown = false;
	filter.cTimeoutMultiplier = 8;

	BLUETOOTH_DEVICE_INFO device_info = {};
	device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
	HBLUETOOTH_DEVICE_FIND h_find = BluetoothFindFirstDevice(&filter, &device_info);

	if (h_find != NULL) {
		do {
			count++;
			*devices = (BLUETOOTH_DEVICE_INFO*)realloc(*devices, sizeof(BLUETOOTH_DEVICE_INFO) * count);
			(*devices)[count - 1] = device_info;
		} while (BluetoothFindNextDevice(h_find, &device_info));
		BluetoothFindDeviceClose(h_find);
	}
	return count;
}

void printDevices(BLUETOOTH_DEVICE_INFO* devices, int count) {
	if (count == 0)
		printf("There are no Bluetooth devices\n");
	else
		for (int i = 0; i < count; i++)
			printf("%i. %ls\n", i + 1, devices[i].szName);
}

int main() {
	setlocale(LC_ALL, "Russian");
	BluetoothSocket socketManager;
	socketManager.initializeWinsock();
	printf("Select mode:\n"
		"s - sender\n"
		"r - receiver\n"
		"e - exit\n\n");
	char c = _getch();
	switch (c) {
	case 's': {
		while (true) {
			system("CLS");
			BLUETOOTH_DEVICE_INFO* devices = nullptr;
			int devices_count = getBluetoothDevices(&devices);
			printf("Devices:\n");
			printDevices(devices, devices_count);
			printf("\nChoose device\n"
				"0 - exit\n"
				"Another symbol - update list\n");
			int choice;
			if (scanf("%i", &choice) != 1) {
				rewind(stdin);
				continue;
			}
			if (choice == 0) {
				free(devices);
				return 0;
			}
			if (choice > 0 && choice <= devices_count) {
				BLUETOOTH_DEVICE_INFO device = devices[choice - 1];
				FILE* fp = fopen("music.mp3", "rb");
				if (fp == NULL) {
					printf("File opening error\n");
					free(devices);
					return 0;
				}
				Sender sender(device);
				if (sender.sendFile(fp) == -1)
					printf("File sending error or connection is broken\n");
				else
					printf("File sent successfully\n");
				fclose(fp);
				free(devices);
				break;
			}
			free(devices);
		}
		break;
	}
	case 'r': {
		system("CLS");
		Receiver receiver;
		printf("Receiver is ready!\n");

		while (true) {
			SOCKET client_socket = receiver.acceptConnection();
			if (client_socket == INVALID_SOCKET) continue;

			if (receiver.receiveFile(client_socket) == -1)
			{
				printf("File receiving error or connection is broken\n");
				break;
			}
			else {
				printf("File received successfully\n");
				system("start ./received_file.mp3");
				break;
			}
			closesocket(client_socket);
		}
		break;
	}
	case 'e':
		socketManager.cleanup();
		return 0;
	default:
		system("CLS");
		break;
	}

	socketManager.cleanup();
	return 0;
}
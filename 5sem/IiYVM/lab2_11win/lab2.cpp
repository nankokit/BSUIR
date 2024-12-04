#include <iostream>
#include <Windows.h>
#include "86-isa-io-driver-master/ISA_IO.h"
#include <stdint.h>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

// структура 0CF8h -W порта адреса:
// |31 |30      24|23   16|15      11|10       8|7              2|1|0|
// | с |  reserv  |  bus  |  device  | function | register index |0|0|

// структура 0CFCh RW порта данных:
// |31            16|15            0|
// |    deviceID    |   vendorID    |

void getDeviceName(uint16_t vendorId, uint16_t deviceId, std::string &vendor, std::string &device, std::ifstream &fp)
{
    fp.clear();
    fp.seekg(0);
    bool vendor_flag = false, device_flag = false;

    std::string line;
    while (!(vendor_flag && device_flag) && std::getline(fp, line))
    {
        uint16_t id;
        std::string name;

        std::istringstream iss(line);
        if (iss >> std::hex >> id)
        {
            std::getline(iss, name);
            name.erase(0, name.find_first_not_of(" \t"));

            if (!vendor_flag)
            {
                if (id == vendorId)
                {
                    vendor = name;
                    vendor_flag = true;
                }
            }
            else if (id == deviceId)
            {
                device = name;
                device_flag = true;
            }
        }
    }

    if (!vendor_flag)
        vendor = "Unknown vendor";
    if (!device_flag)
        device = "Unknown device";
}

int main()
{
    std::ifstream fp("pci.ids");
    if (!fp.is_open())
    {
        std::cerr << "error opening source file" << std::endl;
        return 1;
    }

    HANDLE hDevice = CreateFileW(
        L"\\??\\ISA-IO",              // lpFileName - имя файла (устройства) для открытия
        GENERIC_READ | GENERIC_WRITE, // dwDesiredAccess - права доступа к устройству
        FILE_SHARE_READ,              // dwShareMode - режим совместного доступа
        NULL,                         // lpSecurityAttributes - атрибуты безопасности
        OPEN_EXISTING,                // dwCreationDisposition - тип операции открытия
        FILE_ATTRIBUTE_NORMAL,        // dwFlagsAndAttributes - атрибуты файла
        NULL);                        // hTemplateFile - дескриптор шаблона

    uint32_t addressPort = 0xCF8;
    uint32_t dataPort = 0xCFC;

    std::cout << "| vendor ID | device ID | device name " << std::endl;
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;

    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            for (int function = 0; function < 8; function++)
            {
                // forming address
                uint32_t address = (1 << 31) | (bus << 16) | (device << 11) | (function << 8);

                // forming write request to address port
                IsaIoRequestWrite addressPortRequest;
                addressPortRequest.port = addressPort;
                addressPortRequest.value = address;

                DeviceIoControl(hDevice, IOCTL_ISA_WRITE_32, &addressPortRequest, sizeof(addressPortRequest), NULL, 0, NULL, NULL);

                // forming read request to data port
                IsaIoRequestRead dataPortRequest;
                dataPortRequest.port = dataPort;

                IsaIoResponse response = {0};
                DeviceIoControl(hDevice, IOCTL_ISA_READ_32, &dataPortRequest, sizeof(dataPortRequest), &response, sizeof(response), NULL, NULL);

                // reading result from data port
                uint32_t data = response.value;
                uint16_t deviceId = uint16_t(data >> 16);
                uint16_t vendorId = uint16_t(data);

                if (vendorId != 0xFFFF) //
                {
                    std::string vendorName, deviceName;
                    getDeviceName(vendorId, deviceId, vendorName, deviceName, fp);
                    std::cout << "| " << std::hex << std::setw(9) << vendorId << " | " << std::hex << std::setw(9) << deviceId << " | " << deviceName << std::endl;
                }
            }
        }
    }
    fp.close();
    return 0;
}
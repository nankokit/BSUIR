#include <iostream>
#include <vector>
#include <sstream>
#include "hexioctrl.h"
#include <windows.h> // Для использования Sleep и GetTickCount
#include <conio.h>

class StorageDevice
{
private:
    static const int dataAddresses[2];
    static const int driveAddresses[2];
    static const int statusAddresses[2];

    unsigned short dataBuffer[256];

    bool isChannelAvailable(int channel)
    {
        unsigned long startTime = GetTickCount();
        while (true)
        {
            unsigned char status = _inp(statusAddresses[channel]);
            if (status & (1 << 6))
                return true;
            if (GetTickCount() - startTime >= 1000)
                break;
            Sleep(1);
        }
        return false;
    }

    void waitForDeviceReady(int channel)
    {
        while (true)
        {
            unsigned char status = _inp(statusAddresses[channel]);
            if (!(status & (1 << 7)))
                break;
            Sleep(1);
        }
    }

    bool fetchDeviceProperties(int device, int channel)
    {
        int commands[] = {0xA1, 0xEC};
        for (int i = 0; i < 2; i++)
        {
            waitForDeviceReady(channel);
            unsigned char config = (device << 4) | (7 << 5);
            _outp(driveAddresses[channel], config);
            if (!isChannelAvailable(channel))
                return false;
            _outp(statusAddresses[channel], commands[i]);
            waitForDeviceReady(channel);
            unsigned char deviceStatus = _inp(statusAddresses[channel]);
            if (!(deviceStatus & (1 << 3)))
            {
                if (i)
                    return false;
                continue;
            }
            else
                break;
        }

        for (int i = 0; i < 256; i++)
        {
            dataBuffer[i] = _inpw(dataAddresses[channel]);
        }
        return true;
    }

    void showModes(const std::string &label, unsigned short *data, int startBit, int endBit)
    {
        std::stringstream ss;
        ss << label << ": ";
        bool first = true;
        for (int i = startBit; i <= endBit; i++)
        {
            if (data[i / 16] & (1 << (i % 16)))
            {
                if (!first)
                    ss << ", ";
                ss << (i - startBit);
                first = false;
            }
        }
        std::cout << ss.str() << std::endl;
    }

    void printAdditionalCommands(unsigned short *data, int offset, int commandCount)
    {
        const char *commandNames[] = {
            "SMART commands",
            "Encryption and access restriction",
            "Removable device",
            "Power management",
            "ATAPI commands",
            "Write Cache",
            "Look-Ahead",
            "Release Interrupt",
            "Service Interrupt",
            "Device Reset",
            "Host Protected Area",
            "Write Buffer",
            "Read Buffer",
            "NOP"};

        std::cout << "\nSupported additional commands:\n";
        for (int i = 0; i < commandCount; i++)
        {
            if ((data[(offset + i) / 16] >> ((offset + i) % 16)) & 1)
            {
                std::cout << "\t" << commandNames[i] << "\n";
            }
        }
        std::cout << std::endl;
    }

public:
    void displayInfo(int channel, int device)
    {
        if (fetchDeviceProperties(device, channel))
        {
            std::cout << "Channel: " << channel << ", Device: " << device << "\n";

            std::cout << "\nConsumer and model: ";
            for (int i = 27; i <= 46; i++)
                std::cout << static_cast<char>(dataBuffer[i] >> 8) << static_cast<char>(dataBuffer[i]);

            std::cout << "\nSerial number: ";
            for (int i = 10; i <= 19; i++)
                std::cout << static_cast<char>(dataBuffer[i] >> 8) << static_cast<char>(dataBuffer[i]);

            std::cout << "\nFirmware version: ";
            for (int i = 23; i <= 26; i++)
                std::cout << static_cast<char>(dataBuffer[i] >> 8) << static_cast<char>(dataBuffer[i]);

            std::cout << "\nInterface type: ";
            std::cout << ((dataBuffer[0] >> 15) ? "ATAPI" : "ATA") << "\n";

            if (!(dataBuffer[0] >> 15))
            {
                unsigned long long size = ((unsigned long long)dataBuffer[61] << 16) | dataBuffer[60];
                std::cout << "Disk Size: " << size * 512 / 1024 / 1024 << " MB\n";
            }

            std::cout << "\nAvailable functions:\n";
            showModes("PIO Modes", dataBuffer + 64, 0, 4);
            showModes("MWDMA Modes", dataBuffer + 63, 0, 2);
            showModes("UDMA Modes", dataBuffer + 88, 0, 7);
            showModes("ATA Commands", dataBuffer + 80, 0, 7);
            printAdditionalCommands(dataBuffer + 82, 0, 14);
        }
    }
};

// Определение статических массивов
const int StorageDevice::dataAddresses[2] = {0x1F0, 0x170};
const int StorageDevice::driveAddresses[2] = {0x1F6, 0x176};
const int StorageDevice::statusAddresses[2] = {0x1F7, 0x177};

int main()
{
    ALLOW_IO_OPERATIONS; // Разрешаем операции ввода-вывода
    StorageDevice storageDevice;

    for (int channel = 0; channel < 2; ++channel)
    {
        for (int device = 0; device < 2; ++device)
        {
            storageDevice.displayInfo(channel, device);
        }
    }

    std::cin.get(); // Ожидаем ввода для завершения
    return 0;
}
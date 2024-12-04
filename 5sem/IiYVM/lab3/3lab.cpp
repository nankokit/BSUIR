#include <conio.h>
#include "hexioctrl.h"
#include <iostream>

// Определяем регистры для доступа к устройствам
const int dataRegisters[] = {0x1F0, 0x170}; // Регистр данных DR
// 0x1F0: Первичный канал (обычно для первого устройства)
// 0x170: Вторичный канал (обычно для второго устройства)
// Каждый из этих регистров представляет 16-битное значение, которое передает или получает данные от устройства. Данные передаются по 2 байта за раз.

const int driveRegisters[] = {0x1F6, 0x176}; // Регистр DH номер устройства и головки или LBA[27:24]
// 0x1F6 : Первичный канал
// 0x176 : Вторичный канал
// Биты 7 : 6 — номер устройства(0 = устройство 0, 1 = устройство 1)
// Биты 5 : 4 — номер головки или LBA [27:24]
// Биты 3 : 0 — зарезервированы.

const int commandStatusRegisters[] = {0x1F7, 0x177}; // Регистр состояния SR при чтении и регистр команд CR при записи

unsigned short deviceData[256]; // Массив для хранения данных устройства

// Проверяем готовность канала
bool isChannelReady(int channel)
{
    unsigned long startTime = GetTickCount(); // Фиксируем время начала ожидания

    while (true)
    {
        unsigned char status = _inp(commandStatusRegisters[channel]); // Читаем статус канала
        if (status & (1 << 6))
            return true; // Если 6-й бит установлен, канал готов

        if (GetTickCount() - startTime >= 1000)
            break; // Проверка времени ожидания

        Sleep(1); // Небольшая пауза
    }

    return false; // Канал не готов
}

// Ожидаем готовности устройства
void waitForDevice(int channel)
{
    while (true)
    {
        unsigned char status = _inp(commandStatusRegisters[channel]); // Читаем статус устройства
        if (!(status & (1 << 7)))
            break; // Проверяем 7-й бит на готовность
        Sleep(1);  // Небольшая пауза
    }
}

// Получаем информацию о устройстве
bool retrieveDeviceInfo(int device, int channel)
{
    int commands[] = {0xA1, 0xEC}; // Команды для получения информации о устройстве

    for (int i = 0; i < 2; i++)
    {
        waitForDevice(channel); // Ожидаем готовности устройства

        unsigned char configuration = (device << 4) | (7 << 5); // Настраиваем параметры
        _outp(driveRegisters[channel], configuration);          // Записываем конфигурацию

        if (!isChannelReady(channel))
            return false; // Проверяем готовность канала

        _outp(commandStatusRegisters[channel], commands[i]); // Отправляем команду

        waitForDevice(channel); // Ожидаем завершения операции

        unsigned char deviceStatus = _inp(commandStatusRegisters[channel]); // Читаем статус устройства

        if (!(deviceStatus & (1 << 3)))
        { // Проверяем готовность устройства
            if (i)
                return false; // Если это не первая попытка, выходим
            continue;         // Переходим к следующей команде
        }
        else
            break; // Устройство готово
    }

    for (int i = 0; i < 256; i++)
    {
        deviceData[i] = _inpw(dataRegisters[channel]); // Читаем данные
    }

    return true; // Успешно получили информацию
}

// Выводим поддерживаемые режимы устройства
void displaySupportedModes(const char *title, unsigned short *data, int startBit, int endBit, const char *modes[], int modeCount)
{
    std::cout << title << ":\n";
    for (int i = startBit; i <= endBit; i++)
    {
        if (data[i / 16] & (1 << (i % 16)))
        { // Проверяем установленный бит
            if (i - startBit < modeCount)
                std::cout << "\t" << modes[i - startBit] << "\n"; // Выводим режим
        }
    }
}

// Выводим дополнительные команды устройства
void displayAdditionalCommands(unsigned short *data, int offset, const char *commandNames[], int commandCount)
{
    std::cout << "\nSupported additional commands:\n";
    for (int i = 0; i < commandCount; i++)
    {
        if ((data[(offset + i) / 16] >> ((offset + i) % 16)) & 1)
        {
            std::cout << "\t" << commandNames[i] << "\n"; // Выводим команду
        }
    }
    std::cout << "\n\n";
}

// Выводим информацию о устройстве
void displayDeviceInfo()
{
    std::cout << "\nModel: ";
    for (int i = 27; i <= 46; i++)
        std::cout << static_cast<char>(deviceData[i] >> 8) << static_cast<char>(deviceData[i]); // Выводим модель

    std::cout << "\nSerial: ";
    for (int i = 10; i <= 19; i++)
        std::cout << static_cast<char>(deviceData[i] >> 8) << static_cast<char>(deviceData[i]); // Выводим серийный номер

    std::cout << "\nFirmware version: ";
    for (int i = 23; i <= 26; i++)
        std::cout << static_cast<char>(deviceData[i] >> 8) << static_cast<char>(deviceData[i]); // Выводим версию прошивки

    std::cout << "\nInterface type: ";
    std::cout << ((deviceData[0] >> 15) ? "ATAPI" : "ATA") << "\n"; // Определяем тип интерфейса

    if (!(deviceData[0] >> 15))
    {                                                                                          // Если интерфейс ATA
        unsigned long long size = ((unsigned long long)deviceData[61] << 16) | deviceData[60]; // Вычисляем размер
        std::cout << "Disk Size: " << size * 512 / 1024 / 1024 << " MB\n";                     // Выводим размер диска
    }

    std::cout << "\nAvailable functions:\n";

    const char *pioModes[] = {"PIO 0", "PIO 1", "PIO 2", "PIO 3", "PIO 4"};
    displaySupportedModes("PIO Modes", deviceData + 64, 0, 4, pioModes, 5); // Выводим PIO режимы

    const char *mwdmaModes[] = {"MWDMA 0", "MWDMA 1", "MWDMA 2"};
    displaySupportedModes("MWDMA Modes", deviceData + 63, 0, 2, mwdmaModes, 3); // Выводим MWDMA режимы

    const char *udmaModes[] = {"UDMA Mode 0", "UDMA Mode 1", "UDMA Mode 2", "UDMA Mode 3", "UDMA Mode 4", "UDMA Mode 5", "UDMA Mode 6", "UDMA Mode 7"};
    displaySupportedModes("UDMA Modes", deviceData + 88, 0, 7, udmaModes, 8); // Выводим UDMA режимы

    const char *ataCommands[] = {"ATA 0", "ATA 1", "ATA 2", "ATA 3", "ATA 4", "ATA 5", "ATA 6", "ATA 7"};
    displaySupportedModes("ATA Commands", deviceData + 80, 0, 7, ataCommands, 8); // Выводим ATA команды

    const char *additionalCommands[] = {
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
    displayAdditionalCommands(deviceData + 82, 0, additionalCommands, 14); // Выводим дополнительные команды
}

// Основная функция программы
int main()
{
    ALLOW_IO_OPERATIONS; // Разрешаем операции ввода-вывода
    for (int channel = 0; channel < 2; ++channel)
    { // Проходим по каналам
        for (int device = 0; device < 2; ++device)
        { // Проходим по устройствам
            if (retrieveDeviceInfo(device, channel))
            { // Получаем информацию о устройстве
                std::cout << "Channel: " << channel << ", Device: " << device << "\n";
                displayDeviceInfo(); // Выводим информацию о устройстве
            }
        }
    }
    std::cin.get(); // Ожидаем ввода для завершения
    return 0;
}
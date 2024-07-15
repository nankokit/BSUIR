#include <stdio.h>
#include <windows.h>

void setup_serial_port(HANDLE serial_port)
{
    DCB dcb_serial_params = {0};                             // Создание структуры DCB и инициализация нулевыми значениями
    dcb_serial_params.DCBlength = sizeof(dcb_serial_params); // Установка размера структуры
    if (!GetCommState(serial_port, &dcb_serial_params))      // Получение текущих параметров порта и сохранение их в структуре
    {
        printf("ERROR GET STATE.\n");
    }
    dcb_serial_params.BaudRate = CBR_9600;              // Установка скорости передачи
    dcb_serial_params.ByteSize = 8;                     // Установка количества бит
    dcb_serial_params.StopBits = ONESTOPBIT;            // Установка стопового бита
    dcb_serial_params.Parity = NOPARITY;                // Установка проверки четности
    if (!SetCommState(serial_port, &dcb_serial_params)) // Инициализация порта с новыми параметрами
    {
        printf("ERROR SETTING STATE.\n");
    }
    if (serial_port == INVALID_HANDLE_VALUE) // Проверка успешного открытия порта
    {
        printf("OPEN ERROR.\n");
        ExitProcess(1);
    }
}

int main()
{
    unsigned char data_buffer[] = "a";           // Буфер данных для отправки
    unsigned char received_data_buffer[2] = {0}; // Буфер для принятых данных (размер 8 байт, обнулен)

    unsigned long buffer_size = sizeof(data_buffer);                 // Размер данных для отправки
    unsigned long received_data_size = sizeof(received_data_buffer); // Размер принятых данных

    HANDLE serial_port_second = CreateFile("\\\\.\\COM2", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL); // Открытие COM-порта COM2 для чтения
    setup_serial_port(serial_port_second);                                                                // Инициализация параметров порта

    HANDLE serial_port_first = CreateFile("\\\\.\\COM1", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // Открытие COM-порта COM1 для записи
    setup_serial_port(serial_port_first);                                                                 // Инициализация параметров порта

    printf("COM1 OPENED\n");

    if (WriteFile(serial_port_first, data_buffer, buffer_size, &buffer_size, 0)) // Запись данных в COM1
        printf("DATA SENT\n");                                                   // Вывод сообщения об успешной отправке данных

    printf("COM2 OPENED\n");

    ReadFile(serial_port_second, received_data_buffer, received_data_size, &received_data_size, 0); // Чтение данных из COM2
    printf("RECEIVED DATA: %s\n", received_data_buffer);                                            // Вывод информации о принятых данных

    getchar(); // Ожидание ввода от пользователя

    CloseHandle(serial_port_first); // Закрытие портов
    CloseHandle(serial_port_second);

    return 0;
}
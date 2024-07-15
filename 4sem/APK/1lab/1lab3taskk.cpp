#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

int Com_Init(int port, unsigned long baud);
void com_outchar(char chr, int base_port);
char com_inchar(int base_port);
int check_snd(int base_port);

int Com_Init(int port, unsigned long baud)
{
    // char base_port = 0x3f8 - 0x100 * port;
    char base_port = port;
    unsigned int div;
    switch (baud)
    {
    case 110:
        div = 1040;
        break;
    case 150:
        div = 768;
        break;
    case 300:
        div = 384;
        break;
    case 600:
        div = 192;
        break;
    case 1200:
        div = 96;
        break;
    case 2400:
        div = 48;
        break;
    case 4800:
        div = 24;
        break;
    case 9600:
        div = 12;
        break;
    case 19200:
        div = 6;
        break;
    case 38400:
        div = 3;
        break;
    case 57600:
        div = 2;
        break;
    case 115200:
        div = 1;
        break;
    default:
        return 0;
    }
    unsigned int regst;

    regst = inp(base_port + 0x03);
    outp(base_port + 0x03, regst | 0x80);
    outp(base_port + 0x01, (div >> 8) & 0x00ff); // DLM
    outp(base_port, div & 0x00ff);               // DLL
    // DLAB LCR
    outp(base_port + 0x03, regst & 0x7f);
    outp(base_port + 0x01, 0x00);
    outp(base_port + 0x03, 29);
    outp(base_port + 0x04, 0x00); // DTR=0 RTS=0
    return 1;
};

void com_outchar(char chr, int base_port)
{
    char regst = inp(base_port + 0x04);
    outp(base_port + 0x04, regst | 0x02); // RTS
    outp(base_port, chr);
    delay(100);
    while (!check_snd(base_port))
        ;
    outp(base_port + 0x04, regst & 0xfd); // RTS
}

char com_inchar(int base_port)
{
    return inp(base_port);
}

int check_snd(int base_port)
{
    unsigned char regst;
    regst = inp(base_port + 0x05);
    return ((regst & 0x20) >> 5) == 0x01;
}

int main()
{
    int port1 = 0x3f8;
    int port2 = 0x2f8;
    char inf;

    if (!Com_Init(port1, 9600))
        puts("(1) ERROR COM1");
    if (!Com_Init(port2, 9600))
        puts("(1) ERROR COM2");

    printf("oldInfo(COM1) : %c\n", com_inchar(port1));
    printf("oldInfo(COM2) : %c\n", com_inchar(port2));

    if (check_snd(port1) == 1)
        puts("ready");
    else
        puts("not ready");

    puts("Enter char to sending:");
    rewind(stdin);
    inf = getchar();

    com_outchar(inf, port1);
    printf("info(COM1) : %c\n", com_inchar(port1));
    printf("info(COM2) : %c\n", com_inchar(port2));
}
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <windows.h>
#include <math.h>
unsigned int delayTime = 0;
double currentDelayTime = 0;

void interrupt (*oldDelay)(...);
void interrupt (*oldAlarm)(...);

unsigned int bcdToDec(unsigned int bcd)
{
    return (bcd / 16 * 10) + (bcd % 16);
}
unsigned int decToBcd(unsigned int dec)
{
    return (dec / 10 * 16) + (dec % 10);
}

void interrupt newAlarm(...)
{
    puts("Alarm!!! Alarm!!! Alarm!!!");

    outp(0x70, 0x0C);
    inp(0x71);

    outp(0x20, 0x20); // 0010 0000
    outp(0xA0, 0x20); // неспецефичный EOI

    disable();
    setvect(0x70, oldAlarm);
    enable();
    outp(0x70, 0x0B);             // разрешаем сигнальные прерывания
    outp(0x71, inp(0x71) & 0xDF); // 1101 1111
}

void readTime()
{
    unsigned char state;
    do
    {
        outp(0x70, 0x0A);
        state = inp(0x71);
    } while (state >> 7);
    unsigned int seconds = 0, minutes = 0, hours = 0, days = 0, month = 0, year = 0, century = 0;

    outp(0x70, 0x00);
    seconds = inp(0x71);
    outp(0x70, 0x02);
    minutes = inp(0x71);
    outp(0x70, 0x04);
    hours = inp(0x71);
    outp(0x70, 0x07);
    days = inp(0x71);
    outp(0x70, 0x08);
    month = inp(0x71);
    outp(0x70, 0x09);
    year = inp(0x71);
    outp(0x70, 0x32);
    century = inp(0x71);

    printf("Time:%02u.%02u.%02u\n", bcdToDec(hours), bcdToDec(minutes), bcdToDec(seconds));
    printf("Date:%02u.%02u.%02u\n", bcdToDec(days), bcdToDec(month), bcdToDec(year) + bcdToDec(century) * 100);
}

void setTime()
{
    int n;
    do
    {
        fflush(stdin);
        printf("Hours: ");
    } while ((scanf("%d", &n) != 1 || n > 23 || n < 0));
    unsigned int hours = decToBcd(n);
    do
    {
        fflush(stdin);
        printf("Minutes: ");
    } while (scanf("%d", &n) != 1 || n > 59 || n < 0);
    unsigned int minutes = decToBcd(n);
    do
    {
        fflush(stdin);
        printf("Seconds: ");
    } while (scanf("%d", &n) != 1 || n > 59 || n < 0);
    unsigned int seconds = decToBcd(n);

    unsigned char state;
    do
    {
        outp(0x70, 0x0A);
        state = inp(0x71);
    } while (state >> 7);

    disable();
    outp(0x70, 0x0B);
    outp(0x71, inp(0x71) | 0x80); // 1000 0000

    outp(0x70, 0x00);
    outp(0x71, seconds);
    outp(0x70, 0x02);
    outp(0x71, minutes);
    outp(0x70, 0x04);
    outp(0x71, hours);

    outp(0x70, 0x0B);
    outp(0x71, inp(0x71) & 0x7F);

    enable();

    printf("\nTime updated!\n");
}

void setAlarm()
{
    int n;
    do
    {
        fflush(stdin);
        printf("Hours: ");
    } while ((scanf("%d", &n) != 1 || n > 23 || n < 0));
    unsigned int hours = decToBcd(n);
    do
    {
        fflush(stdin);
        printf("Minutes: ");
    } while (scanf("%d", &n) != 1 || n > 59 || n < 0);
    unsigned int minutes = decToBcd(n);
    do
    {
        fflush(stdin);
        printf("Seconds: ");
    } while (scanf("%d", &n) != 1 || n > 59 || n < 0);
    unsigned int seconds = decToBcd(n);
    disable();

    oldAlarm = getvect(0x70);
    setvect(0x70, newAlarm);
    outp(0xA1, (inp(0xA0) & 0xFE)); // чтение состояния ведомого контроллера и разрешение прерывания о будильника

    do
    {
        outp(0x70, 0x0A);
    } while (inp(0xA0) & 0xFE);

    outp(0x70, 0x01);
    outp(0x71, seconds);
    outp(0x70, 0x03);
    outp(0x71, minutes);
    outp(0x70, 0x05);
    outp(0x71, hours);

    enable();

    outp(0x70, 0x0B);
    outp(0x71, inp(0x71) | 0x20); // 0010 0000

    printf("Alarm enable!\n");
}

void interrupt newDelay(...)
{
    currentDelayTime++;
    outp(0x70, 0x0C);
    inp(0x71);

    // EOI
    outp(0x20, 0x20); // 0010 0000
    outp(0xA0, 0x20);

    disable();
    if (currentDelayTime >= delayTime)
    {
        puts("Delay's end");
        setvect(0x70, oldDelay);
        outp(0x70, 0x0B);
        outp(0x71, inp(0x71) & 0xBF); // 1011 1111
    }
    enable();
}

void delay()
{
    printf("Input delay (ms): ");
    scanf("%u", &delayTime);
    currentDelayTime = 0;

    disable();
    oldDelay = getvect(0x70);
    setvect(0x70, newDelay);
    outp(0xA1, (inp(0xA0) & 0xFE));
    enable();

    outp(0x70, 0x0B); // разрешение периодических прерываний
    outp(0x71, inp(0x71) | 0x40);
}

void changeFreq()
{
    printf("1 - 2Hz\n2 - 4Hz\n3 - 8Hz\n4 - 16Hz\n5 - 32Hz\n6 - 64Hz\n7 - 128Hz\n8 - 256Hz\n9 - 512Hz\n10 - 1024Hz\n11 - 2048Hz\n12 - 4096Hz\n");
    int q;
    scanf("%d", &q);
    int freq = 0x0F - q + 1;

    disable();
    outp(0x70, 0x0A);
    outp(0x71, inp(0x71) | (0x0F));

    outp(0x70, 0x0A);
    outp(0x71, inp(0x71) & (0xA0 + freq));
    enable();

    outp(0x70, 0x0A);
    int bin = inp(0x71);
    printf("\nNew freq %.0f \n", pow(2, (16 - (bin % 16))));
}

int main()
{
    int flag = 1;
    while (flag)
    {
        printf("1 - Read time\n2 - Set new time\n3 - set alarm\n4 - delay\n5 - change freq\n0 - exit\n");

        switch (getch())
        {
        case '1':
            readTime();
            break;
        case '2':
            setTime();
            break;
        case '3':
            setAlarm();
            break;
        case '4':
            delay();
            break;
        case '5':
            changeFreq();
            break;
        case '0':
            flag = 0;
            break;
        }
        rewind(stdin);
    }
    return 0;
}
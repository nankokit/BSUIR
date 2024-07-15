#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <iostream.h>
int program_flag = 1;
int pressed = 1;

void interrupt far (*oldIRQ1)(...);

void interrupt far newIRQ1(...)
{
    unsigned char value = 0;
    value = inp(0x60); // get value from 60h port
    if (value == 0x10) // scan code of q - 0x10
    {
        program_flag = 0;
    }
    if (pressed)
    {
        cout << "Pressed key scan-code: " << hex << (int)value << endl;
        pressed = 0;
    }
    else
    {
        cout << "Released key scan-code: " << hex << (int)value << endl;
        pressed = 1;
    }
    outp(0x20, 0x20); // reset interruption controller
}

void light(unsigned char value)
{
    do
    {
        while ((inp(0x64) & 0x02) != 0x00)
            ;             // 0000 0010
        outp(0x60, 0xED); // говорим что собираемся мигать
        delay(20);
    } while (inp(0x60) == 0xFE); // пока обработано не успешно(FE)

    do
    {
        while ((inp(0x64) & 0x02) != 0x00)
            ;
        outp(0x60, value); // мигаем
        delay(20);
    } while (inp(0x60) == 0xFE);
}

void blink()
{
    cout << "Blink started!" << endl;
    for (int i = 0; i < 5; i++)
    {
        light(0x04);
        delay(500);
        light(0x00);
        delay(500);
    }
    cout << "Blinking ends!" << endl;
}

int main()
{
    cout << "Press any key to start" << endl;
    char c = getch();
    blink();
    oldIRQ1 = getvect(0x09); // считали состояние клавиатуры
    setvect(0x09, newIRQ1);  // установили новый
    cout << "Press key to see code of the key (q for exit)" << endl;
    while (program_flag)
        ;
    setvect(0x09, oldIRQ1); // вернули старое состояние
    cout << "Press any key to exit " << endl;
    getch();
    return 0;
}
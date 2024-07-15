#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_register(int reg, char far *pos)
{
	for (int i = 0; i < 8; i++)
	{
		*pos = reg % 2 + '0'; // divide by 2, get each byte individually
		pos += 2;			  // first byte is char, second is attribute
		reg = reg >> 1;		  // /2
	}
}

void output_string(char far *pos, char *str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		*pos = str[i];
		pos += 2;
	}
}

void print()
{
	// MK_FP creates far pointer (segment, offset)
	// 0xB800 is video buffer start, 0xA0 (160) is next line, 0x140 (160+160) is another line offset
	output_string((char far *)MK_FP(0xB800, 0), "mask:              \0");
	output_string((char far *)MK_FP(0xB800, 0xA0), "serv:              \0");
	output_string((char far *)MK_FP(0xB800, 0x140), "reqv:              \0");
	char temp;
	int i, val;
	char far *screen = (char far *)MK_FP(0xB800, 0);
	screen += 20;

	val = inp(0x21); // master mask register
	print_register(val, screen);

	screen += 18;

	val = inp(0xA1); // slave mask register
	print_register(val, screen);

	screen += 142;

	outp(0x20, 0x0A); // write to select request register
	val = inp(0x20);
	print_register(val, screen); // get master request register

	screen += 18;

	outp(0xA0, 0x0A);
	val = inp(0xA0); // get slave request register
	print_register(val, screen);

	screen += 142;

	outp(0x20, 0x0B); // request service register
	val = inp(0x20);  // get master service register
	print_register(val, screen);

	screen += 18;

	outp(0xA0, 0x0B);
	val = inp(0xA0); // get slave service register
	print_register(val, screen);
}

void prepare()
{
	_disable(); // disable interrupts to avoid nested interrupts
	print();
}

void interrupt (*oldint1[8])(...); // IRQ 0-7
void interrupt (*oldint2[8])(...); // IRQ 8-15

void interrupt newint70(...)
{
	prepare();
	oldint1[0]();
	_enable();
}
void interrupt newint71(...)
{
	prepare();
	oldint1[1]();
	_enable();
}
void interrupt newint72(...)
{
	prepare();
	oldint1[2]();
	_enable();
}
void interrupt newint73(...)
{
	prepare();
	oldint1[3]();
	_enable();
}
void interrupt newint74(...)
{
	prepare();
	oldint1[4]();
	_enable();
}
void interrupt newint75(...)
{
	prepare();
	oldint1[5]();
	_enable();
}
void interrupt newint76(...)
{
	prepare();
	oldint1[6]();
	_enable();
}
void interrupt newint77(...)
{
	prepare();
	oldint1[7]();
	_enable();
}

void interrupt newint08(...)
{
	prepare();
	oldint2[0]();
	_enable();
}
void interrupt newint09(...)
{
	prepare();
	oldint2[1]();
	_enable();
}
void interrupt newint0A(...)
{
	prepare();
	oldint2[2]();
	_enable();
}
void interrupt newint0B(...)
{
	prepare();
	oldint2[3]();
	_enable();
}
void interrupt newint0C(...)
{
	prepare();
	oldint2[4]();
	_enable();
}
void interrupt newint0D(...)
{
	prepare();
	oldint2[5]();
	_enable();
}
void interrupt newint0E(...)
{
	prepare();
	oldint2[6]();
	_enable();
}
void interrupt newint0F(...)
{
	prepare();
	oldint2[7]();
	_enable();
}

void initialize()
{
	int i;
	for (i = 0x70; i <= 0x77; i++)
	{
		oldint2[i - 0x70] = getvect(i);
	}

	for (i = 0x08; i <= 0x0F; i++)
	{
		oldint1[i - 0x08] = getvect(i);
	}
	// set new handlers for IRQ 0-7
	setvect(0x70, newint70);
	setvect(0x71, newint71);
	setvect(0x72, newint72);
	setvect(0x73, newint73);
	setvect(0x74, newint74);
	setvect(0x75, newint75);
	setvect(0x76, newint76);
	setvect(0x77, newint77);
	// set new handlers for IRQ8-15
	setvect(0x08, newint08);
	setvect(0x09, newint09);
	setvect(0x0A, newint0A);
	setvect(0x0B, newint0B);
	setvect(0x0C, newint0C);
	setvect(0x0D, newint0D);
	setvect(0x0E, newint0E);
	setvect(0x0F, newint0F);

	_disable(); // CLI
	// interrupt initialization for Master
	// ICW (initialization command word)
	outp(0x20, 0b00010001); // ICW1 - initialize master (call ICW4, use slave controller, vector interrupt size 8 bytes, trigger at level)
	outp(0x21, 0x70);		// ICW2 - base vector for master
	outp(0x21, 0x00000100); // ICW3 - the port bit of Slave (port 4 (0 1 2 3 4, first 2 - power, then IRQ0 timer and IRQ1 keyboard))
	outp(0x21, 0b00000001); // ICW4 - default (use intel 8086 cpu, no automatic EOI, no bufferization, no nested mode)
	// interrupt initialization for Slave
	outp(0xA0, 0b00010001); // ICW1 - initialize slave (call ICW4, use slave controller, vector interrupt size 8 bytes, trigger at level)
	outp(0xA1, 0x08);		// ICW2 - base vector for slave
	outp(0xA1, 0x00000010); // ICW3 - the port number of connected port on Master (0 1 2, without first 2 power lanes)
	outp(0xA1, 0b00000001); // ICW4 - default
	_enable();				// STI
}

int main()
{
	unsigned far *fp;
	initialize();
	system("cls");

	printf("mask:   \n");
	printf("obsl:\n");
	printf("requ:\n");

	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

	return 0;
}
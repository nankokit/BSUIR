#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

const int SOUND_DURATION_MILISECONDS = 400;
int multyplied = 1;

void ReadStatusWords(void);
void EnableSound();
void DisableSound();
short HerzToShort(int);
void ChannelsKd();
void Ringtone();

void main()
{
	char c = 0;
	outp(0x43, 0xB6);

	while (c != 27)
	{
		printf("Press: \n0 - play ringtone\n1 - read status words\n2 - read CE\nesc-exit\n");
		c = getch();
		switch (c)
		{
		case '0':
			Ringtone();
			clrscr();
			break;
		case '1':
			ReadStatusWords();
			break;
		case '2':
			ChannelsKd();
			break;
		case 27:
			break;
		}
	}

	return;
}

void EnableSound()
{
	outp(0x61, inp(0x61) | 3); // 61h 0 and 1 bits to 1
}

void DisableSound()
{
	outp(0x61, (inp(0x61) & 0xFFFC)); // 61h 0 and 1 bits to 0
}

void SetTonality(int frequency)
{
	short value_of_CE = 1193180 / frequency;
	outp(0x42, (char)value_of_CE);		  // low byte
	outp(0x42, (char)(value_of_CE >> 8)); // high byte
}

void ReadStatusWords()
{
	char *str = (char *)calloc(9, 1);
	unsigned char value;
	outp(0x43, 0xE2); // command to read first channel (1110 0010)
	value = inp(0x40);
	itoa(value, str, 2);
	printf("first channel status word : %s\n", str);

	outp(0x43, 0xE4); // command to read second status word
	value = inp(0x41);
	itoa(value, str, 2);
	printf("second channel status word: %s\n", str);

	outp(0x43, 0xE8); // command to read second status word
	value = inp(0x42);
	itoa(value, str, 2);
	printf("third channel status word : %s\n", str);
	free(str);
}

void ChannelsKd()
{
	char *str = (char *)calloc(9, 1);
	char ce_high = 0;
	char ce_low = 0;
	short result = 0;
	outp(0x43, 0x06);
	outp(0x43, 0xD2);
	ce_low = inp(0x40);
	ce_high = inp(0x40);
	result = ce_high * 256 + ce_low;
	itoa(result, str, 2);
	printf("first channel CE value %s (%d)\n", str, result);

	outp(0x43, 0x46);
	outp(0x43, 0xD4);
	ce_low = inp(0x41);
	ce_high = inp(0x41);
	result = ce_high * 256 + ce_low;
	itoa(result, str, 2);
	printf("second channel CE value %s (%d)\n", str, result);

	outp(0x43, 0x86);
	outp(0x43, 0xD8);
	ce_low = inp(0x42);
	ce_high = inp(0x42);
	result = ce_high * 256 + ce_low;
	itoa(result, str, 2);
	printf("third channel CE value %s (%d)\n", str, result);
	free(str);
}

void Ringtone()
{
	SetTonality(196);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(261);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(329);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(196);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(261);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(329);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(196);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(261);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	SetTonality(329);
	EnableSound();
	delay(SOUND_DURATION_MILISECONDS);
	DisableSound();

	clrscr();
}
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

const int SOUND_DURATION_MILISECONDS = 200;

void SetTonality(int);
void EnableSound();
void DisableSound();
void ReadStatusWords(void);
void ChannelsCE();
void PrintStr8b(char *);

int main()
{
	int music[] = {196, 261, 329, 196, 261, 329, 196, 261, 329};
	outp(0x43, 0xB6); // 1011 0110

	for (int i = 0; i < 9; i++)
	{
		SetTonality(music[i]);
		EnableSound();
		delay(400);
		DisableSound();
	}
	ReadStatusWords();
	ChannelsCE();
	return 0;
}

void EnableSound()
{
	outp(0x61, inp(0x61) | 3);
}

void DisableSound()
{
	outp(0x61, inp(0x61) & 0xFC);
}

void SetTonality(int frequency)
{
	short value_of_CE = 1193180 / frequency;
	outp(0x42, (char)value_of_CE);		  // low byte
	outp(0x42, (char)(value_of_CE >> 8)); // high byte
}

void ReadStatusWords()
{
	char *str = (char *)malloc(255);
	unsigned char value;
	outp(0x43, 0xE2); // command to read first channel 1110 0010
	value = inp(0x40);
	itoa(value, str, 2);
	printf("first channel status word:");
	PrintStr8b(str);

	outp(0x43, 0xE4); // command to read second status word 1110 0100
	value = inp(0x41);
	itoa(value, str, 2);
	printf("second channel status word:");
	PrintStr8b(str);

	outp(0x43, 0xE8); // command to read third status word 1110 1000
	value = inp(0x42);
	itoa(value, str, 2);
	printf("third channel status word:");
	PrintStr8b(str);
	free(str);
}

void ChannelsCE()
{
	char *str = (char *)malloc(255);
	unsigned char ce_high = 0;
	unsigned char ce_low = 0;
	unsigned short result = 0;
	outp(0x43, 0x06); // 0000 0110
	outp(0x43, 0xD2); // 1101 0010
	ce_low = inp(0x40);
	ce_high = inp(0x40);
	result = (ce_high << 8) + ce_low;
	itoa(result, str, 16);
	printf("first channel CE value: %s (%u)\n", str, result);

	outp(0x43, 0x46); // 0100 0110
	outp(0x43, 0xD4); // 1101 0100
	ce_low = inp(0x41);
	ce_high = inp(0x41);
	result = (ce_high << 8) + ce_low;
	itoa(result, str, 16);
	printf("second channel CE value: %s (%u)\n", str, result);

	outp(0x43, 0x86); // 1000 0110
	outp(0x43, 0xD8); // 1101 1000
	ce_low = inp(0x42);
	ce_high = inp(0x42);
	result = (ce_high << 8) + ce_low;
	itoa(result, str, 16);
	printf("third channel CE value: %s (%u)\n", str, result);
	free(str);
}

void PrintStr8b(char *str)
{
	if (strlen(str) < 8)
	{
		for (int i = 0; i < 8 - strlen(str); i++)
			printf("0");
	}
	printf("%s\n", str);
}
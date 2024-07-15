#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

const int SOUND_DURATION_MILISECONDS = 50;
int multyplied=1;

void ReadStatusWords(void);
void EnableSound();
void DisableSound();
short HerzToShort(int);
void ChannelsKd();
void piano();

void main()
{
	char c=0;
	outp(0x43, 0xB6); 

	while(c!=27){
		printf("Press: \n0 - play piano\n1 - read status words\n2 - read CE\nesc-exit\n");
		c = getch();
		switch(c){
			case '0':
				piano();
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
	outp(0x61, inp(0x61) | 3);
}


void DisableSound()
{
	char port61 = inp(0x61);
	port61 = port61 & 0xFFFC;
	outp(0x61, port61);
}

void SetTonality (int frequency)
{
	short value_of_CE = 1193180/frequency;
    outp(0x42, (char)value_of_CE);		// low byte
    outp(0x42, (char)(value_of_CE >> 8)); // high byte
}

void ReadStatusWords()
{
	char* str = (char*)calloc(9,1);
	unsigned char value;
	outp (0x43, 0xE2); // command to read first channel
	value = inp(0x40);
	itoa(value, str,2);
	printf ("first channel status word : %s\n", str);

	outp (0x43, 0xE4); // command to read second status word
	value = inp(0x41);
	itoa(value, str,2);
	printf ("second channel status word: %s\n", str);

	outp (0x43, 0xE8); // command to read second status word
	value = inp(0x42);
	itoa(value, str,2);
	printf ("third channel status word : %s\n", str);
	free (str);
}

void ChannelsKd()
{
	char * str =  (char*)calloc(9,1);
	char ce_high =0;
	char ce_low = 0;
	short result = 0;
	outp(0x43, 0x06);
	outp (0x43, 0xD2);
	ce_low = inp(0x40);
	ce_high = inp(0x40);
	result = ce_high*256+ce_low;
	itoa(result, str,2);
	printf ("first channel CE value %s (%d)\n", str, result);

	outp(0x43, 0x46);
	outp (0x43, 0xD4);
	ce_low = inp(0x41);
	ce_high = inp(0x41);
	result = ce_high*256+ce_low;
	itoa(result, str,2);
	printf ("second channel CE value %s (%d)\n", str, result);
	
	outp(0x43, 0x86);
	outp (0x43, 0xD8);
	ce_low = inp(0x42);
	ce_high = inp(0x42);
	result = ce_high*256+ce_low;
	itoa(result, str,2);
	printf ("third channel CE value %s (%d)\n", str, result);
	free(str);
}

void piano(){

	char c=0;
	printf ("press keys to play:)\n");
	printf ("__________________________________________________________\n");
	printf ("|   |  |  |  |   |   |  |  |  |  |  |   |   |  |  |  |   |\n");
	printf ("|   |  |  |  |   |   |  |  |  |  |  |   |   |  |  |  |   |\n");
	printf ("|   | w|  | e|   |   | t|  | y|  | u|   |   | o|  | p|   |\n");
	printf ("|   |__|  |__|   |   |__|  |__|  |__|   |   |__|  |__|   |\n");
	printf ("|    |      |    |    |     |      |    |    |      |    |\n");
	printf ("|  a |   s  |  d |  f |  g  |   h  |  j |  k |   l  |  ; |\n");
	printf ("|____|______|____|____|_____|______|____|____|______|____|\n");
	while(c != 27)
	{
		c = getch();
		switch(c)
		{
		
			case 'a':
				SetTonality(131*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 's':
				SetTonality(294/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'd':
				SetTonality(330/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'f':
				SetTonality(349/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'g':
				SetTonality(392/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'h':
				SetTonality(440/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'j':
				SetTonality(494/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'k':
				SetTonality(131*(multyplied*2));
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'l':
				SetTonality(294/2*(multyplied*2));
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case ';':
				SetTonality(330/2*(multyplied*2));
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;

			case 'w':
				SetTonality(277/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'e':
				SetTonality(311/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			
			case 't':
				SetTonality(370/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'y':
				SetTonality(415/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'u':
				SetTonality(466/2*multyplied);
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'o':
				SetTonality(277/2*(multyplied*2));
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;
			case 'p':
				SetTonality(311/2*(multyplied*2));
				EnableSound();
				delay(SOUND_DURATION_MILISECONDS);
				DisableSound();
				break;	
			case '1':
				multyplied = 1;
				break;
			case '2':
				multyplied = 2;
				break;
			case '3':
				multyplied = 4;
				break;
			case '4':
				multyplied = 8;
				break;
			case 27:
			    break;
		}
	}
	clrscr();
}
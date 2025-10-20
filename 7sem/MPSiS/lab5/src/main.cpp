#include <msp430.h>
#include <math.h>

#define ENABLE_BIT(reg, bit) ((reg) |= (bit))
#define DISABLE_BIT(reg, bit) ((reg) &= ~(bit))
#define FLIP_BIT(reg, bit) ((reg) ^= (bit))
#define IS_BIT_SET(reg, bit) (((reg) & (bit)) != 0)

typedef unsigned char uchar;

#define SCROL_CTL 0x78
#define SET_MIRROR_COL 0xA0
#define SET_MIRROR_ROW 0xC0
#define ALL_PIXEL_ON 0xA4
#define LCD_INVERSE 0xA6
#define BIAS_RATIO_VCC 0xA2
#define POW_CTL 0x2F
#define SET_CONTRAST_RESISTOR 0x27
#define MSB_SELECT_VOLUME 0x81
#define LSB_SELECT_VOLUME 0x10
#define ADV_CTL_MSB 0xFA
#define ADV_CTL_LSB 0x90
#define COLUMN_ADR_MSB 0x12
#define COLUMN_ADR_LSB 0x00
#define PAGE_ADR 0xB0
#define LCD_EN 0xAF
#define COLUMNS 4
#define COLUMN_OFFSET 64

#define READ_X_AXIS_DATA 0x18
#define READ_Z_AXIS_DATA 0x20
#define NONE 0
#define SAMPLING_FREQUENCY_HZ 400
#define DISPLAY_UPDATE_INTERVAL 4
#define ANGLE_MIN 60
#define ANGLE_MAX 120

long int CONVERT_TO_100G = 980;

uchar plus[1][COLUMNS] = {{0x08, 0x1C, 0x08, 0x00}};
uchar minus[1][COLUMNS] = {{0x08, 0x08, 0x08, 0x00}};
uchar digits[10][COLUMNS] = {
    {0x7F, 0x41, 0x7F, 0x00},
    {0x7F, 0x02, 0x04, 0x00},
    {0x4F, 0x49, 0x79, 0x00},
    {0x7F, 0x49, 0x49, 0x00},
    {0x7F, 0x08, 0x0F, 0x00},
    {0x79, 0x49, 0x4F, 0x00},
    {0x79, 0x49, 0x7F, 0x00},
    {0x0F, 0x71, 0x03, 0x00},
    {0x7F, 0x49, 0x7F, 0x00},
    {0x7F, 0x49, 0x4F, 0x00}};

uchar LCD_INIT_COMMANDS_PART_1[7] = {
    SCROL_CTL,
    SET_MIRROR_COL,
    SET_MIRROR_ROW,
    ALL_PIXEL_ON,
    LCD_INVERSE,
    BIAS_RATIO_VCC,
    POW_CTL};
uchar LCD_INIT_COMMANDS_PART_2[6] = {
    SET_CONTRAST_RESISTOR,
    MSB_SELECT_VOLUME,
    LSB_SELECT_VOLUME,
    ADV_CTL_MSB,
    ADV_CTL_LSB,
    LCD_EN};
int AMOUNT_OF_COMMANDS_1 = 7;
int AMOUNT_OF_COMMANDS_2 = 6;

uchar BITx[] = {BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0};
int MAPPING_VALUES[] = {4571, 2286, 1141, 571, 286, 143, 71};

void SetupLCD(void);
void SetupAccelerometer(void);
void SetupTimer(void);
void Clear(void);
void ShowNumber(long int number);
void __LCD_SetAddress(uchar page, uchar column);
void Dogs102x6_writeData(uchar *sData, uchar i);
void Dogs102x6_writeCommand(uchar *sCmd, uchar i);
uchar CMA3000_writeCommand(uchar firstByte, uchar secondByte);
long int parseProjectionByte(uchar projectionByte);
int calculateAngleFromProjection(double projection);
void Delay(long int value);

void ClearLCD(void)
{
  uchar lcd_data[] = {0x00}; // Данные для очистки (нулевые байты)
  uchar page, column;
  for (page = 0; page < 8; page++) // Перебор всех 8 страниц дисплея
  {
    __LCD_SetAddress(page, 0);               // Установка адреса страницы и начального столбца
    for (column = 0; column < 132; column++) // Перебор всех 132 столбцов
    {
      Dogs102x6_writeData(lcd_data, 1); // Запись нуля в текущий столбец
    }
  }
}

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  SetupLCD();
  SetupAccelerometer();
  SetupTimer();

  ENABLE_BIT(P1DIR, BIT0);  // Настраиваем P1.0 как выход
  DISABLE_BIT(P1OUT, BIT0); // Изначально выключаем светодиод

  Clear();
  СlearLCD();
  __bis_SR_register(LPM0_bits + GIE);
  return 0;
}

void SetupLCD(void)
{
  ENABLE_BIT(P5DIR, BIT7);
  DISABLE_BIT(P5OUT, BIT7);
  ENABLE_BIT(P5OUT, BIT7);
  Delay(550);
  ENABLE_BIT(P7DIR, BIT4);
  DISABLE_BIT(P7OUT, BIT4);
  ENABLE_BIT(P5DIR, BIT6);
  DISABLE_BIT(P5OUT, BIT6);
  ENABLE_BIT(P4SEL, BIT1 | BIT3);
  ENABLE_BIT(P4DIR, BIT1 | BIT3);
  ENABLE_BIT(P7DIR, BIT6);
  ENABLE_BIT(P7OUT, BIT6);
  DISABLE_BIT(P7SEL, BIT6);
  ENABLE_BIT(P7OUT, BIT4);
  ENABLE_BIT(UCB1CTL1, UCSWRST);
  UCB1CTL0 = (UCCKPH & ~UCCKPL | UCMSB | UCMST | UCSYNC | UCMODE_0);
  UCB1CTL1 = UCSSEL_2 | UCSWRST;
  UCB1BR0 = 0x01;
  UCB1BR1 = 0;
  DISABLE_BIT(UCB1CTL1, UCSWRST);
  DISABLE_BIT(UCB1IFG, UCRXIFG);
  Dogs102x6_writeCommand(LCD_INIT_COMMANDS_PART_1, AMOUNT_OF_COMMANDS_1);
  Delay(12500);
  Dogs102x6_writeCommand(LCD_INIT_COMMANDS_PART_2, AMOUNT_OF_COMMANDS_2);
}

void SetupAccelerometer(void)
{
  DISABLE_BIT(P2DIR, BIT5);
  ENABLE_BIT(P2OUT, BIT5);
  ENABLE_BIT(P2REN, BIT5);
  ENABLE_BIT(P2IE, BIT5);
  DISABLE_BIT(P2IES, BIT5);
  DISABLE_BIT(P2IFG, BIT5);

  ENABLE_BIT(P3DIR, BIT5);
  ENABLE_BIT(P3OUT, BIT5);
  ENABLE_BIT(P2DIR, BIT7);
  ENABLE_BIT(P2SEL, BIT7);
  ENABLE_BIT(P3DIR, BIT3 | BIT6);
  DISABLE_BIT(P3DIR, BIT4);
  ENABLE_BIT(P3SEL, BIT3 | BIT4);
  ENABLE_BIT(P3OUT, BIT6);
  UCA0CTL1 = UCSSEL_2 | UCSWRST;
  UCA0BR0 = 0x30;
  UCA0BR1 = 0;
  UCA0CTL0 = (UCCKPH & ~UCCKPL | UCMSB | UCMST | UCSYNC | UCMODE_0);
  DISABLE_BIT(UCA0CTL1, UCSWRST);

  CMA3000_writeCommand(0x04, NONE);
  Delay(1250);
  CMA3000_writeCommand(0x0A, BIT4 | BIT2);
  Delay(25000);
}

void SetupTimer(void)
{
  TA0CCR0 = 2500;
  TA0CTL = TASSEL_2 | MC_1 | TACLR;
  ENABLE_BIT(TA0CCTL0, CCIE);
}

void Clear(void)
{
  uchar lcd_data[] = {0x00};
  uchar page, column;
  for (page = 0; page < 8; page++)
  {
    __LCD_SetAddress(page, 0);
    for (column = 0; column < 132; column++)
    {
      Dogs102x6_writeData(lcd_data, 1);
    }
  }
}

void ShowNumber(long int number)
{
  volatile int length = 1;
  volatile int digit = 0;
  volatile int j = 0;
  volatile long int i = 10;
  while (1)
  {
    if (number / i != 0)
    {
      i *= 10;
      length++;
    }
    else
    {
      break;
    }
  }
  long int temp = number;
  for (j = 0; j < length; j++)
  {
    digit = (int)(temp % 10);
    digit = digit < 0 ? (-1) * digit : digit;
    if (digit < 10)
    {
      __LCD_SetAddress(0, COLUMN_OFFSET + j * COLUMNS);
      Dogs102x6_writeData(digits[digit], COLUMNS);
    }
    temp /= 10;
  }
  if (number >= 0)
  {
    __LCD_SetAddress(0, COLUMN_OFFSET + length * COLUMNS);
    Dogs102x6_writeData(plus[0], COLUMNS);
  }
  else
  {
    __LCD_SetAddress(0, COLUMN_OFFSET + length * COLUMNS);
    Dogs102x6_writeData(minus[0], COLUMNS);
  }
}

void __LCD_SetAddress(uchar page, uchar column)
{
  uchar cmd[1];
  if (page > 7)
  {
    page = 7;
  }
  if (column > 101)
  {
    column = 101;
  }
  cmd[0] = PAGE_ADR + (7 - page);
  uchar command_high = 0x00;
  uchar command_low = 0x00;
  uchar column_address[] = {COLUMN_ADR_MSB, COLUMN_ADR_LSB};
  command_low = (column & 0x0F);
  command_high = (column & 0xF0);
  command_high = (command_high >> 4);
  column_address[0] = COLUMN_ADR_LSB + command_low;
  column_address[1] = COLUMN_ADR_MSB + command_high;
  Dogs102x6_writeCommand(cmd, 1);
  Dogs102x6_writeCommand(column_address, 2);
}

void Dogs102x6_writeCommand(uchar *sCmd, uchar i)
{
  DISABLE_BIT(P7OUT, BIT4);
  DISABLE_BIT(P5OUT, BIT6);
  while (i)
  {
    while (!IS_BIT_SET(UCB1IFG, UCTXIFG))
      ;
    UCB1TXBUF = *sCmd++;
    i--;
  }
  while (UCB1STAT & UCBUSY)
    ;
  UCB1RXBUF;
  ENABLE_BIT(P7OUT, BIT4);
}

void Dogs102x6_writeData(uchar *sData, uchar i)
{
  DISABLE_BIT(P7OUT, BIT4);
  ENABLE_BIT(P5OUT, BIT6);
  while (i)
  {
    while (!IS_BIT_SET(UCB1IFG, UCTXIFG))
      ;
    UCB1TXBUF = *sData++;
    i--;
  }
  while (UCB1STAT & UCBUSY)
    ;
  UCB1RXBUF;
  ENABLE_BIT(P7OUT, BIT4);
}

uchar CMA3000_writeCommand(uchar firstByte, uchar secondByte)
{
  uchar indata;
  DISABLE_BIT(P3OUT, BIT5);
  indata = UCA0RXBUF;
  while (!(UCA0IFG & UCTXIFG))
    ;
  UCA0TXBUF = firstByte;
  while (!(UCA0IFG & UCRXIFG))
    ;
  indata = UCA0RXBUF;
  while (!(UCA0IFG & UCTXIFG))
    ;
  UCA0TXBUF = secondByte;
  while (!(UCA0IFG & UCRXIFG))
    ;
  indata = UCA0RXBUF;
  while (UCA0STAT & UCBUSY)
    ;
  ENABLE_BIT(P3OUT, BIT5);
  return indata;
}

long int parseProjectionByte(uchar projectionByte)
{
  int i = 0;
  long int projectionValue = 0;
  int isNegative = IS_BIT_SET(projectionByte, BIT7);
  for (; i < 7; i++)
  {
    if (isNegative)
    {
      projectionValue += IS_BIT_SET(projectionByte, BITx[i]) ? 0 : MAPPING_VALUES[i];
    }
    else
    {
      projectionValue += IS_BIT_SET(projectionByte, BITx[i]) ? MAPPING_VALUES[i] : 0;
    }
  }
  projectionValue *= isNegative ? -1 : 1;
  return projectionValue;
}

int calculateAngleFromProjection(double projection)
{
  projection /= 1000;
  projection = projection > 1 ? 1 : projection < -1 ? -1
                                                    : projection;
  double angle = acos(projection);
  angle *= 57.2958;
  return (int)angle;
}

void Delay(long int value)
{
  volatile long int i = 0;
  volatile long int temp = 0;
  for (; i < value; i++)
  {
    temp++;
  }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
  static volatile int display_counter = 0;
  volatile uchar xProjectionByte = CMA3000_writeCommand(READ_X_AXIS_DATA, NONE);
  volatile long int xAxisProjection = parseProjectionByte(xProjectionByte);
  volatile long int acceleration = xAxisProjection * CONVERT_TO_100G;

  int angle = calculateAngleFromProjection((double)xAxisProjection);
  if (angle >= ANGLE_MIN && angle <= ANGLE_MAX)
  {
    DISABLE_BIT(P1OUT, BIT0);
  }
  else
  {
    ENABLE_BIT(P1OUT, BIT0);
  }

  display_counter++;
  if (display_counter >= DISPLAY_UPDATE_INTERVAL)
  {
    Clear();
    ShowNumber(acceleration);
    display_counter = 0;
  }
}

#pragma vector = PORT2_VECTOR
__interrupt void accelerometerInterrupt(void)
{
  DISABLE_BIT(P2IFG, BIT5);
}
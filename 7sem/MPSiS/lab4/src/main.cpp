#include <msp430.h>
#include <math.h>

#define ENABLE_BIT(reg, bit) ((reg) |= (bit))      
#define DISABLE_BIT(reg, bit) ((reg) &= ~(bit))     
#define FLIP_BIT(reg, bit) ((reg) ^= (bit))        
#define IS_BIT_SET(reg, bit) (((reg) & (bit)) != 0)

typedef unsigned char uchar;

#define SCROL_CTL                   0x78 // Прокрутка
#define SCROL_CTL_DOWN              0x58 // Прокрутка вниз
#define SET_MIRROR_COL              0xA0 // Зеркалирование столбцов
#define SET_MIRROR_ROW              0xC0 // Зеркалирование строк
#define ALL_PIXEL_ON                0xA4 // Нормальный режим пикселей
#define LCD_INVERSE                 0xA6 // Нормальный дисплей
#define LCD_INVERSE_YES             0xA7 // Инвертированный дисплей
#define BIAS_RATIO_VCC              0xA2 // Соотношение напряжения
#define POW_CTL                     0x2F // Питание дисплея
#define SET_CONTRAST_RESISTOR       0x27 // Контрастность
#define MSB_SELECT_VOLUME           0x81 // Старший байт контрастности
#define LSB_SELECT_VOLUME           0x10 // Младший байт контрастности
#define ADV_CTL_MSB                 0xFA // Расширенные настройки (MSB)
#define ADV_CTL_LSB                 0x90 // Расширенные настройки (LSB)
#define COLUMN_ADR_MSB              0x12 // Старший байт адреса столбца
#define COLUMN_ADR_LSB              0x00 // Младший байт адреса столбца
#define PAGE_ADR                    0xB0 // Адрес страницы
#define LCD_EN                      0xAF // Включить дисплей
#define ROWS                        7    // Количество строк
#define COLUMNS                     4    // Столбцы для символа
#define PAGES                       1    // Страницы для символов
#define DELAY                       500  // Задержка антидребезга
#define COLUMN_OFFSET_BIG           80   // Смещение столбцов
#define COLUMN_OFFSET_NONE          0    // Без смещения

void Delay(long int value);
int GetS1State();
int GetS2State();
void SetupSPI();
void SetupLCD();
void SetupButtons();
void __LCD_SetAddress(uchar page, uchar column);
void Dogs102x6_writeData(uchar *sData, uchar i);
void Dogs102x6_writeCommand(uchar *sCmd, uchar i);
void Clear(void);
void ShowNumber(void);

uchar scroll[1] = {SCROL_CTL}; 
uchar plus[PAGES][COLUMNS]  = {{0x08, 0x1C, 0x08, 0x00}}; // Знак "+"
uchar minus[PAGES][COLUMNS] = {{0x08, 0x08, 0x08, 0x00}}; // Знак "-"
uchar digits[10][COLUMNS] = { // Шаблоны цифр 0–9
    {0x7F, 0x41, 0x7F, 0x00}, // 0
    {0x7F, 0x02, 0x04, 0x00}, // 1
    {0x4F, 0x49, 0x79, 0x00}, // 2
    {0x7F, 0x49, 0x49, 0x00}, // 3
    {0x7F, 0x08, 0x0F, 0x00}, // 4
    {0x79, 0x49, 0x4F, 0x00}, // 5
    {0x79, 0x49, 0x7F, 0x00}, // 6
    {0x0F, 0x71, 0x03, 0x00}, // 7
    {0x7F, 0x49, 0x7F, 0x00}, // 8
    {0x7F, 0x49, 0x4F, 0x00}  // 9
};

int number = 4903; // Начальное число
int subtrahend = 571; // Значение для вычитания
int column_offset = COLUMN_OFFSET_BIG; // Смещение столбцов

// Команды инициализации дисплея
uchar LCD_INIT_COMMANDS_PART_1[7] = {
    SCROL_CTL,
    SET_MIRROR_COL,
    SET_MIRROR_ROW,
    ALL_PIXEL_ON,
    LCD_INVERSE,
    BIAS_RATIO_VCC,
    POW_CTL
};
uchar LCD_INIT_COMMANDS_PART_2[6] = {
    SET_CONTRAST_RESISTOR,
    MSB_SELECT_VOLUME,
    LSB_SELECT_VOLUME,
    ADV_CTL_MSB,
    ADV_CTL_LSB,
    LCD_EN,
};
int AMOUNT_OF_COMMANDS_1 = 7;
int AMOUNT_OF_COMMANDS_2 = 6;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    SetupLCD();
    SetupButtons();
    Clear();
    ShowNumber();
__bis_SR_register(GIE);
  __no_operation();
    return 0;
}

// Настройка дисплея и SPI
void SetupLCD()
{
    ENABLE_BIT(P5DIR, BIT7); // P5.7 (A0) как выход
    DISABLE_BIT(P5OUT, BIT7); ENABLE_BIT(P5OUT, BIT7); // Сброс/установка A0
    Delay(550); // Задержка для стабилизации
    ENABLE_BIT(P7DIR, BIT4); DISABLE_BIT(P7OUT, BIT4); // P7.4 (CS) как выход, сброс
    ENABLE_BIT(P5DIR, BIT6); DISABLE_BIT(P5OUT, BIT6); // P5.6 (Reset) как выход, сброс
    ENABLE_BIT(P4SEL, BIT1 | BIT3); ENABLE_BIT(P4DIR, BIT1 | BIT3); // P4.1, P4.3 для SPI
    ENABLE_BIT(P7DIR, BIT6); ENABLE_BIT(P7OUT, BIT6); // P7.6 (подсветка) включить
    DISABLE_BIT(P7SEL, BIT6); ENABLE_BIT(P7OUT, BIT4); // Отключить P7.6 от периферии, установить CS
    ENABLE_BIT(UCB1CTL1, UCSWRST); // Сброс SPI
    UCB1CTL0 = (UCCKPH & ~UCCKPL | UCMSB | UCMST | UCSYNC | UCMODE_0); // Настройка SPI
    UCB1CTL1 = UCSSEL_2 | UCSWRST; // SMCLK как источник
    UCB1BR0 = 0x01; UCB1BR1 = 0; // Делитель частоты
    DISABLE_BIT(UCB1CTL1, UCSWRST); // Отключить сброс SPI
    DISABLE_BIT(UCB1IFG, UCRXIFG); // Очистить флаг приёма
    Dogs102x6_writeCommand(LCD_INIT_COMMANDS_PART_1, AMOUNT_OF_COMMANDS_1); 
    Delay(12500); // Задержка
    Dogs102x6_writeCommand(LCD_INIT_COMMANDS_PART_2, AMOUNT_OF_COMMANDS_2); 
}

// Настройка кнопок S1 и S2
void SetupButtons()
{
    DISABLE_BIT(P1DIR, BIT7); DISABLE_BIT(P2DIR, BIT2); // P1.7 (S1), P2.2 (S2) как входы
    ENABLE_BIT(P1REN, BIT7); ENABLE_BIT(P2REN, BIT2); // Включить подтягивающие резисторы
    ENABLE_BIT(P1OUT, BIT7); ENABLE_BIT(P2OUT, BIT2); // Подтяжка вверх
    ENABLE_BIT(P1IE, BIT7); ENABLE_BIT(P1IES, BIT7); P1IFG = 0; // Прерывание для S1
    ENABLE_BIT(P2IE, BIT2); ENABLE_BIT(P2IES, BIT2); P2IFG = 0; // Прерывание для S2
}

// Установить адрес страницы и столбца
void __LCD_SetAddress(uchar page, uchar column)
{
    if (page > 7) page = 7; // Ограничить страницу
    if (column > 101) column = 101; // Ограничить столбец
    uchar cmd[1] = {PAGE_ADR + (7 - page)}; // Установить страницу
    uchar column_address[2];
    column_address[0] = COLUMN_ADR_LSB + (column & 0x0F); // Младший байт адреса
    column_address[1] = COLUMN_ADR_MSB + ((column & 0xF0) >> 4); // Старший байт адреса
    Dogs102x6_writeCommand(cmd, 1); // Отправить адрес страницы
    Dogs102x6_writeCommand(column_address, 2); // Отправить адрес столбца
}

// Отправить команду на дисплей
void Dogs102x6_writeCommand(uchar *sCmd, uchar i)
{
    DISABLE_BIT(P7OUT, BIT4); DISABLE_BIT(P5OUT, BIT6); // Сбросить CS и A0 (режим команд)
    while (i) {
        while (!IS_BIT_SET(UCB1IFG, UCTXIFG)); // Ожидать готовности
        UCB1TXBUF = *sCmd++; i--; // Отправить байт
    }
    while (UCB1STAT & UCBUSY); // Ожидать завершения
    UCB1RXBUF; ENABLE_BIT(P7OUT, BIT4); // Очистить буфер, установить CS
}

// Записать данные на дисплей
void Dogs102x6_writeData(uchar *sData, uchar i)
{
    DISABLE_BIT(P7OUT, BIT4); ENABLE_BIT(P5OUT, BIT6); // Сбросить CS, установить A0 (режим данных)
    while (i) {
        while (!IS_BIT_SET(UCB1IFG, UCTXIFG)); // Ожидать готовности
        UCB1TXBUF = *sData++; i--; // Отправить байт
    }
    while (UCB1STAT & UCBUSY); // Ожидать завершения
    UCB1RXBUF; ENABLE_BIT(P7OUT, BIT4); // Очистить буфер, установить CS
}

// Отобразить число
void ShowNumber(void)
{
    volatile int length = 1, i = 10;
    while (number / i != 0) { i *= 10; length++; } // Вычислить длину числа
    int temp = number;
    for (volatile int j = 0; j < length; j++) {
        int digit = (temp % 10); // Получить цифру
        digit = digit < 0 ? -digit : digit; // Обработать отрицательные числа
        if (digit < 10) {
            __LCD_SetAddress(0, column_offset + j * COLUMNS); // Установить адрес
            Dogs102x6_writeData(digits[digit], COLUMNS); // Отобразить цифру
        }
        temp /= 10;
    }
    __LCD_SetAddress(0, column_offset + length * COLUMNS); // Адрес для знака
    Dogs102x6_writeData(number >= 0 ? plus[0] : minus[0], COLUMNS); // Отобразить знак
}

// Очистить дисплей
void Clear(void)
{
    uchar lcd_data[] = {0x00};
    for (uchar page = 0; page < 8; page++) {
        __LCD_SetAddress(page, 0);
        for (uchar column = 0; column < 132; column++) {
            Dogs102x6_writeData(lcd_data, 1); // Очистить столбец
        }
    }
}
// Обработчик прерывания S1
#pragma vector = PORT1_VECTOR
__interrupt void __S1_ButtonHandler(void)
{
    Delay(DELAY); // Антидребезг
    if (GetS1State()) {
        Clear(); // Очистить дисплей
        number -= subtrahend; // Вычесть значение
        if (number < -9999) number += 15000; // Проверка границы
        ShowNumber(); // Обновить дисплей
    }
    DISABLE_BIT(P1IFG, BIT7); // Очистить флаг
}

// Обработчик прерывания S2
#pragma vector = PORT2_VECTOR
__interrupt void __S2_ButtonHandler(void)
{
    Delay(DELAY); // Антидребезг
    if (GetS2State()) {
        Clear(); // Очистить дисплей
        scroll[0] -= 0x20; // Изменить прокрутку
        if (*scroll < 0x40) *scroll = SCROL_CTL - (0x40 - *scroll); // Корректировка
        Dogs102x6_writeCommand(scroll, 1); // Отправить команду прокрутки
        ShowNumber(); // Обновить дисплей
    }
    DISABLE_BIT(P2IFG, BIT2); // Очистить флаг
}

// Задержка для антидребезга
void Delay(long int value)
{
    volatile long int i = 0, temp = 0;
    for (; i < value; i++) temp++;
}

// Состояние кнопки S1
int GetS1State()
{
    return IS_BIT_SET(P1IN, BIT7) ? 0 : 1; // 1, если нажата
}

// Состояние кнопки S2
int GetS2State()
{
    return IS_BIT_SET(P2IN, BIT2) ? 0 : 1; // 1, если нажата
}
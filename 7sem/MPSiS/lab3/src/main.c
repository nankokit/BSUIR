#include <msp430.h>

#define SET_BIT(reg, bit) ((reg) |= (bit))       
#define CLEAR_BIT(reg, bit) ((reg) &= ~(bit))    
#define TOGGLE_BIT(reg, bit) ((reg) ^= (bit))    
#define CHECK_BIT(reg, bit) (((reg) & (bit)) != 0) 

volatile unsigned char state = 0;                        // Состояние последовательности: 0 - ожидание, 1-5 - шаги последовательности
volatile unsigned char use_wdt = 0;                      // Выбор таймера: 0 - TA1, 1 - WDT
volatile unsigned int delay_count = 0;                   // Счетчик интервалов для WDT
volatile unsigned char leds_map[3] = {BIT3, BIT4, BIT5}; // Массив битов для светодиодов P1.3, P1.4, P1.5

void start_delay(double sec); 
void stop_delay(void);        
void advance_state(void);     
void start_debounce(void);    

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Остановить сторожевой таймер (WDT) при старте

    // Настройка мигающего светодиода на P1.2 (выход TA0.1 для ШИМ)
    SET_BIT(P1DIR, BIT2);     // P1.2 как выход
    SET_BIT(P1SEL, BIT2);     // Подключить P1.2 к выходу таймера TA0.1

    // Настройка светодиодов последовательности P1.3, P1.4, P1.5
    SET_BIT(P1DIR, (leds_map[0] | leds_map[1] | leds_map[2])); // Установить как выходы
    CLEAR_BIT(P1OUT, (leds_map[0] | leds_map[1] | leds_map[2])); // Выключить все светодиоды

    // Настройка кнопок: S1 на P1.7, S2 на P2.2 (внутренний подтягивающий резистор, прерывание по спаду)
    SET_BIT(P1REN, BIT7);     // Включить подтягивающий резистор для P1.7
    SET_BIT(P1OUT, BIT7);     // Установить высокий уровень (подтяжка к питанию)
    SET_BIT(P1IE, BIT7);      // Включить прерывание для P1.7
    SET_BIT(P1IES, BIT7);     // Прерывание по спаду сигнала
    CLEAR_BIT(P1IFG, BIT7);   // Сбросить флаг прерывания

    SET_BIT(P2REN, BIT2);     // Аналогично для P2.2
    SET_BIT(P2OUT, BIT2);
    SET_BIT(P2IE, BIT2);
    SET_BIT(P2IES, BIT2);
    CLEAR_BIT(P2IFG, BIT2);

    // Настройка TA0 для ШИМ: период 3с, скважность 1/3 (1с вкл, 2с выкл)
    TA0CCR0 = 12287;          // Период 3с при ACLK/8 (4096 Гц): 3*4096 - 1
    TA0CCR1 = 4095;           // Длительность импульса 1с: 4096 - 1
    TA0CCTL1 = OUTMOD_7;      // Режим сброс/установка для ШИМ
    TA0CTL = TASSEL__ACLK | ID__8 | MC__UP | TACLR; // ACLK, делитель 8, режим UP, сброс

    __bis_SR_register(GIE);   // Включить глобальные прерывания

    while (1)
    {
        __bis_SR_register(LPM3_bits + GIE); // Перейти в режим LPM3 (активен ACLK) с включенными прерываниями
    }
}

// Функция запуска задержки (использует TA1 или WDT)
void start_delay(double sec)
{
    if (use_wdt)
    {
        // Используем WDT для задержки
        if (sec == 0.5)
            delay_count = 256; // 256 интервалов по ~1.95мс = 0.5с
        else // 0.9с
            delay_count = 461; // 461 интервалов по ~1.95мс = 0.9с

        WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL__ACLK | WDTIS__64; // WDT в режиме таймера, ACLK, интервал ~1.95мс
        SET_BIT(SFRIE1, WDTIE); // Включить прерывание WDT
    }
    else
    {
        // Используем TA1 для задержки
        unsigned long ticks = (unsigned long)(sec * 32768.0); // Перевод секунд в тики при ACLK (32768 Гц)
        TA1CCR0 = ticks - 1; // Установить значение для таймера
        SET_BIT(TA1CCTL0, CCIE); // Включить прерывание по совпадению
        TA1CTL = TASSEL__ACLK | MC__UP | TACLR; // ACLK, режим UP, сброс
    }
}

// Функция остановки задержки
void stop_delay(void)
{
    if (use_wdt)
    {
        CLEAR_BIT(SFRIE1, WDTIE); // Отключить прерывание WDT
        WDTCTL = WDTPW | WDTHOLD; // Остановить WDT
    }
    else
    {
        CLEAR_BIT(TA1CCTL0, CCIE); // Отключить прерывание TA1
        TA1CTL = 0; // Остановить TA1
    }
}

// Функция продвижения последовательности светодиодов
void advance_state(void)
{
    double next_delay;
    switch (state)
    {
    case 1: // После 0.5с от включения P1.3
        SET_BIT(P1OUT, leds_map[1]); // Включить P1.4
        next_delay = 0.5;
        state = 2;
        break;
    case 2: // После 0.5с от включения P1.4
        SET_BIT(P1OUT, leds_map[2]); // Включить P1.5
        next_delay = 0.9;
        state = 3;
        break;
    case 3: // После 0.9с от включения P1.5
        CLEAR_BIT(P1OUT, leds_map[2]); // Выключить P1.5
        next_delay = 0.9;
        state = 4;
        break;
    case 4: // После 0.9с от выключения P1.5
        CLEAR_BIT(P1OUT, leds_map[1]); // Выключить P1.4
        next_delay = 0.9;
        state = 5;
        break;
    case 5: // После 0.9с от выключения P1.4
        CLEAR_BIT(P1OUT, leds_map[0]); // Выключить P1.3
        state = 0;
        return; // Завершить последовательность
    default:
        return;
    }
    start_delay(next_delay); // Запустить следующую задержку
}

// Функция запуска таймера дебойсинга (~50мс)
void start_debounce(void)
{
    TA2CTL = TACLR;           // Сбросить TA2
    TA2CCR0 = 1637;           // ~50мс при 32768 Гц
    SET_BIT(TA2CCTL0, CCIE);  // Включить прерывание по совпадению
    TA2CTL = TASSEL__ACLK | MC__UP; // ACLK, режим UP
}

// Обработчик прерываний для порта 1 (кнопка S1 на P1.7)
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    if (CHECK_BIT(P1IFG, BIT7))
    {
        CLEAR_BIT(P1IFG, BIT7);   // Сбросить флаг прерывания
        CLEAR_BIT(P1IE, BIT7);    // Отключить прерывание P1.7 до дебойсинга
        start_debounce();         // Запустить таймер дебойсинга
    }
}

// Обработчик прерываний для порта 2 (кнопка S2 на P2.2)
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    if (CHECK_BIT(P2IFG, BIT2))
    {
        CLEAR_BIT(P2IFG, BIT2);   // Сбросить флаг прерывания
        CLEAR_BIT(P2IE, BIT2);    // Отключить прерывание P2.2 до дебойсинга
        start_debounce();         // Запустить таймер дебойсинга
    }
}

// Обработчик прерывания TA1 CCR0 (для задержек последовательности, если не WDT)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    stop_delay();                 // Остановить задержку
    advance_state();              // Продвинь последовательность
    __bic_SR_register_on_exit(LPM3_bits); // Выход из LPM3
}

// Обработчик прерывания WDT (для задержек, если выбран WDT)
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    if (--delay_count == 0)       // Уменьшить счетчик, проверить окончание
    {
        stop_delay();             // Остановить задержку
        advance_state();          // Продвинь последовательность
        __bic_SR_register_on_exit(LPM3_bits); // Выход из LPM3
    }
}

// Обработчик прерывания TA2 CCR0 (дебойсинг)
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
{
    CLEAR_BIT(TA2CCTL0, CCIE);    // Отключить прерывание TA2
    TA2CTL = 0;                   // Остановить TA2

    // Проверка кнопки S1
    if (!CHECK_BIT(P1IN, BIT7))   // Если кнопка все еще нажата
    {
        if (state == 0)           // Если в состоянии ожидания
        {
            CLEAR_BIT(P1OUT, (leds_map[0] | leds_map[1] | leds_map[2])); // Выключить все светодиоды
            SET_BIT(P1OUT, leds_map[0]); // Включить P1.3
            start_delay(0.5);         // Запустить задержку 0.5с
            state = 1;                // Перейти к первому шагу
        }
    }
    SET_BIT(P1IE, BIT7);          // Включить прерывание P1.7

    // Проверка кнопки S2
    if (!CHECK_BIT(P2IN, BIT2))   // Если кнопка все еще нажата
    {
        TOGGLE_BIT(use_wdt, 1);   // Переключить режим задержки (TA1/WDT)
        CLEAR_BIT(P1OUT, (leds_map[0] | leds_map[1] | leds_map[2])); // Выключить все светодиоды
    }
    SET_BIT(P2IE, BIT2);          // Включить прерывание P2.2

    __bic_SR_register_on_exit(LPM3_bits); // Выход из LPM3
}
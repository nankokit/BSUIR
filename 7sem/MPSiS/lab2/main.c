#include <msp430f5529.h>

typedef unsigned char uchar;

#define SET_BIT(reg, bit) ((reg) |= (bit))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= (bit))
#define CHECK_BIT(reg, bit) (((reg) & (bit)) != 0)

#define DEBOUNCE_COUNT 10

#define BLINK                      \
  {                                \
    SET_BIT(P8OUT, BIT1 | BIT2);   \
    __delay_cycles(200000);        \
    CLEAR_BIT(P8OUT, BIT1 | BIT2); \
    __delay_cycles(200000);        \
  }

typedef struct
{
  uchar prev;
  uchar reading;
  unsigned int debounce_cnt;
} ButtonState;

uchar in_sleep_mode = 0;

#pragma vector = PORT1_VECTOR
__interrupt void buttonS1()
{
  if (CHECK_BIT(P1IFG, BIT7))
  {
    __delay_cycles(10000);
    uchar current_s1 = CHECK_BIT(P1IN, BIT7) ? 1 : 0;
    if (current_s1)
    {
      if (in_sleep_mode)
      {
        __bic_SR_register_on_exit(CPUOFF | SCG0 | SCG1);
        in_sleep_mode = 0;
      }
      else
      {
        __bis_SR_register_on_exit(CPUOFF | SCG0 | SCG1);
        in_sleep_mode = 1;
      }
    }
    CLEAR_BIT(P1IFG, BIT7);
  }
}

uchar readButton(const volatile unsigned char *port_reg, unsigned char bit,
                 ButtonState *state)
{
  uchar current = CHECK_BIT(*port_reg, bit) ? 1 : 0;

  if (current != state->reading)
  {
    state->reading = current;
    state->debounce_cnt = 0;
  }
  else if (state->debounce_cnt < DEBOUNCE_COUNT)
  {
    state->debounce_cnt++;
  }
  else
  {
    state->prev = current;
  }
  return state->prev;
}

uchar readButtonS2()
{
  static ButtonState s2_state = {1, 1, 0};
  return readButton(&P2IN, BIT2, &s2_state);
}

// DCOCLK = 98 кГц (MCLK = DCOCLK)
// DCOCLK = FLLREFCLK / FLLREFDIV * (FLLN + 1) * FLLD
void setDcoclk98kHz()
{
  __bis_SR_register(SCG0); // отключение FLL
  UCSCTL0 = 0x0000;        // сброс DCO и MOD

  CLEAR_BIT(UCSCTL1, DCORSEL_7); 
  SET_BIT(UCSCTL1, DCORSEL_0); // диапазон DCO 0.07–1.7 МГц

  CLEAR_BIT(UCSCTL2, FLLD_7 | 0x3ff); // дел  множ
  SET_BIT(UCSCTL2, FLLD__1 | FLLN8); // FLLD = 1, FLLN = 2

  CLEAR_BIT(UCSCTL3, SELREF_7 | FLLREFDIV_7); // ист_fll делитель
  SET_BIT(UCSCTL3, SELREF__XT1CLK | FLLREFDIV__1); // FLLREFCLK = XT1CLK, FLLREFDIV = 1
  __bic_SR_register(SCG0); // включение FLL

  do
  {
    CLEAR_BIT(UCSCTL7, XT2OFFG | XT1LFOFFG | DCOFFG);
  } while (!CHECK_BIT(UCSCTL7, DCOFFG)); // ожидание стабилизации

  CLEAR_BIT(UCSCTL4, SELS_7);
  SET_BIT(UCSCTL4, SELM__DCOCLK | SELS__DCOCLK); // источник mclk | smclk

  CLEAR_BIT(UCSCTL5, DIVM_7);          // очищение
  SET_BIT(UCSCTL5, DIVM__16 | DIVS__1); // делитель для MCLK = 1

  CLEAR_BIT(P8OUT, BIT2);
  SET_BIT(P8OUT, BIT1);
}

// MCLK = DCOCLK/2 с REFOCLK
void setMclkRefoclk()
{
  __bis_SR_register(SCG0); // отключение FLL
  UCSCTL0 = 0x0000;        // сброс DCO и MOD

  CLEAR_BIT(UCSCTL1, DCORSEL_7);
  SET_BIT(UCSCTL1, DCORSEL_2); // DCO = 2 (от 0,32 до 7,38 МГц)

  CLEAR_BIT(UCSCTL2, FLLD_7 | 0x3ff);
  SET_BIT(UCSCTL2, FLLD__1 | FLLN3); // FLLD = 1, FLLN = 8

  CLEAR_BIT(UCSCTL3, SELREF_7 | FLLREFDIV_7);
  SET_BIT(UCSCTL3, SELREF__REFOCLK | FLLREFDIV__1);  // FLLREFCLK = REFOCLK, FLLREFDIV = /1

  __bic_SR_register(SCG0);
  do
  {
    CLEAR_BIT(UCSCTL7, XT2OFFG | XT1LFOFFG | DCOFFG);
  } while (!CHECK_BIT(UCSCTL7, DCOFFG));

  CLEAR_BIT(UCSCTL4, SELM_7 | SELS_7);
  SET_BIT(UCSCTL4, SELM__DCOCLK | SELS__DCOCLK);  // источник mclk | smclk

  CLEAR_BIT(UCSCTL5, DIVM_7);
  SET_BIT(UCSCTL5, DIVM__2 | DIVS_2); // делители MCLK и SMCLK = 2

  CLEAR_BIT(P8OUT, BIT1);
  SET_BIT(P8OUT, BIT2);
}

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  SET_BIT(P8DIR, BIT1 | BIT2);
  CLEAR_BIT(P8OUT, BIT1 | BIT2);
  CLEAR_BIT(P1DIR, BIT7);
  SET_BIT(P1REN, BIT7);
  SET_BIT(P1OUT, BIT7);
  CLEAR_BIT(P2DIR, BIT2);
  SET_BIT(P2REN, BIT2);
  SET_BIT(P2OUT, BIT2);
  CLEAR_BIT(P1IES, BIT7);
  CLEAR_BIT(P1IFG, BIT7);
  SET_BIT(P1IE, BIT7);
  SET_BIT(P7DIR, BIT7);
  SET_BIT(P7SEL, BIT7);

  __enable_interrupt();

  BLINK
  setDcoclk98kHz();


  uchar prev_s2 = 1, current_s2;
  uchar clk = 0;
  while (1)
  {
    current_s2 = readButtonS2();
    if (prev_s2 && !current_s2)
    {
      clk = !clk;
      if (clk)
      {
        setDcoclk98kHz();
      }
      else
      {
        setMclkRefoclk();
      }
    }
    prev_s2 = current_s2;
  }
  return 0;
}

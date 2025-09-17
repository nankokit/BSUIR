#include <msp430.h>

#define ENABLE_BIT(reg, bit) ((reg) |= (bit))      
#define DISABLE_BIT(reg, bit) ((reg) &= ~(bit))     
#define FLIP_BIT(reg, bit) ((reg) ^= (bit))        
#define IS_BIT_SET(reg, bit) (((reg) & (bit)) != 0) 

void setup_leds(void) 
{
    ENABLE_BIT(P8DIR, BIT1 | BIT2);    // LED2 (8.1) and LED3 (8.2) out
    DISABLE_BIT(P8OUT, BIT1 | BIT2);   // leds off
}

void setup_buttons(void) 
{
    DISABLE_BIT(P1DIR, BIT7);   // button 1 (1.7) in
    DISABLE_BIT(P2DIR, BIT2);   // button 2 (2.2) in
    ENABLE_BIT(P1REN, BIT7);    // resistor enable
    ENABLE_BIT(P2REN, BIT2);           
    ENABLE_BIT(P1OUT, BIT7);    // pull-up
    ENABLE_BIT(P2OUT, BIT2);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  
    setup_leds();
    setup_buttons();

    unsigned char prev_button1_state = IS_BIT_SET(P1IN, BIT7);
    unsigned char prev_button2_state = IS_BIT_SET(P2IN, BIT2);

    while(1)
    {
        unsigned char curr_button1_state = IS_BIT_SET(P1IN, BIT7);
        unsigned char curr_button2_state = IS_BIT_SET(P2IN, BIT2);

        if(curr_button1_state != prev_button1_state)
        {
            if (!curr_button1_state)   // pressed (1 -> 0)
            {
                if (!curr_button2_state) // button2 pressed
                {
                    DISABLE_BIT(P8OUT, BIT2); // led3 off
                }
                if (curr_button2_state) // button2 released
                {
                    ENABLE_BIT(P8OUT, BIT1); // led2 on
                }
            }
            else 
            {
                if (!curr_button2_state) // button2 pressed
                {
                    ENABLE_BIT(P8OUT, BIT2); //led3 on
                }
            }
        }

        if (curr_button2_state != prev_button2_state) 
        {
            if (curr_button2_state) // button2 released (0 -> 1)
            { 
                DISABLE_BIT(P8OUT, BIT1); // led2 off
            }
        }

        prev_button1_state = curr_button1_state;
        prev_button2_state = curr_button2_state;

        __delay_cycles(10000);
    }
}
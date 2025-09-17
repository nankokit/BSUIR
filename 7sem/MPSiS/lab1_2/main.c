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

void setup_interrupts(void) {
    ENABLE_BIT(P1IES, BIT7);    // interrup by back
    ENABLE_BIT(P2IES, BIT2);
    ENABLE_BIT(P1IE, BIT7);
    ENABLE_BIT(P2IE, BIT2);
    DISABLE_BIT(P1IFG, BIT7);   //clear
    DISABLE_BIT(P2IFG, BIT2);
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  
    setup_leds();
    setup_buttons();
    setup_interrupts();
    
    __bis_SR_register(GIE);  
    
    while(1) {

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void button_1(void)
{
    if (P1IFG & BIT7) {  
        if (IS_BIT_SET(P1IES, BIT7)) {  
            if (!IS_BIT_SET(P2IN, BIT2)) {  
                DISABLE_BIT(P8OUT, BIT2);   
            }
            if (IS_BIT_SET(P2IN, BIT2)) {   
                ENABLE_BIT(P8OUT, BIT1);    
            }
        } else { 
            if (!IS_BIT_SET(P2IN, BIT2)) {  
                ENABLE_BIT(P8OUT, BIT2);    
            }
        }
        FLIP_BIT(P1IES, BIT7);  
        DISABLE_BIT(P1IFG, BIT7);  
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void button_2(void)
{
    if (P2IFG & BIT2) {  
        if (IS_BIT_SET(P2IES, BIT2)) {  
        } else {  
            DISABLE_BIT(P8OUT, BIT1); 
        }
        FLIP_BIT(P2IES, BIT2);  
        DISABLE_BIT(P2IFG, BIT2);  
    }
}
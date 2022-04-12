#include <msp430.h> 
#include <stdint.h>

void AllumerLed1(uint8_t state) {
    if(state)
        P4OUT |= BIT6;
    else
        P4OUT &= ~BIT6;
}

void AllumerLed2(uint8_t state) {
    if(state)
        P1OUT |= BIT0;
    else
        P1OUT &= ~BIT0;
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	// Set leds as output
	P1DIR |= BIT0;              // P1.1 as output
	P4DIR |= BIT6;              // P4.6 as output

	// Set S1 as input and pull-up resistor
	P4DIR &= ~BIT5;             // P4.5 as input
	P4REN |= BIT5;              // P4.5 resistor enable
	P4OUT |= BIT5;              // P4.5 pull-up

    // Set S2 as input and pull-up resistor
    P1DIR &= ~BIT1;             // P4.5 as input
    P1REN |= BIT1;              // P4.5 resistor enable
    P1OUT |= BIT1;              // P4.5 pull-up

	PM5CTL0 &= ~0x01;           // Unlock IOs
	
	while(1) {
	    AllumerLed1(!(P4IN & BIT5));   // Si S1 pressé
        AllumerLed2(!(P1IN & BIT1));   // Si S2 pressé
	}

	return 0;
}

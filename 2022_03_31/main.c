#include <msp430.h> 


/**
 * main.c
 */

void delay(int i) {

    for(i; i > 0; i--){
        __delay_cycles(1000);
    }
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	
	P1DIR |= 0x01;

	while(1) {
	    P1OUT ^= 0x01;
	    delay(1000);
	}

	return 0;
}

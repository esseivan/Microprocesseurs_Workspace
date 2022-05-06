#include <msp430.h> 
#include <stdint.h>

#define S1_PRESSED  (!(P4IN & BIT5))
#define S2_PRESSED  (!(P1IN & BIT1))
#define LED1_ON()   (P4OUT |= BIT6)
#define LED1_OFF()   (P4OUT &= ~BIT6)
#define LED2_ON()   (P1OUT |= BIT0)
#define LED2_OFF()   (P1OUT &= ~BIT0)

void Delay_100c(uint16_t delay) {
    for(; delay != 0; delay--) {
        // Wait 100 cycles (100us)
        _delay_cycles(100);
    }
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

    const int8_t T = 100;       // 100 * 0.1ms = 10ms
    int8_t t1 = T/2;
    int8_t t2;

    while(1) {
        if(t1 != 0) // do not turn on if on time is 0
            LED1_ON();

        if(S1_PRESSED)
            t1--;
        if(S2_PRESSED)
            t1++;

        if(t1 > T) {
            t1 = T;
            LED2_ON();
        }
        else if(t1 < 0) {
            t1 = 0;
            LED2_ON();
        }
        else
            LED2_OFF();


        t2 = T - t1;

        Delay_100c(t1);
        LED1_OFF();
        Delay_100c(t2);
    }

	return 0;
}

#include <msp430.h> 
#include <stdint.h>

/**
 * main.c
 */

#define NB_VAL 16

int16_t sumArray(int8_t* array, uint8_t index, uint8_t max) {
    int16_t sum = 0;
    for(;index < max; index++) {
        sum += (*array);
        array++;
    }
    return sum;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= BIT0;
	P4DIR |= BIT6;
	PM5CTL0 &= ~BIT0;

	while(1) {

	int16_t Sum = 0;
	uint8_t Index = 0;
	int8_t Values[NB_VAL] =
	{ 122,23,31,27,54,-2,-17,
	 -5,112,13,56,65,21,87,
	 -32,-54 };

	/*
	do {
	    Sum += Values[Index++];
	} while(Index < NB_VAL);
    */
	Sum = sumArray(Values, 0, NB_VAL);

	uint8_t Mean = Sum / NB_VAL;

	Index = 2*Mean;
	P1OUT |= BIT0;
	P4OUT &= ~BIT6;
	do {
	    P1OUT ^= BIT0;
	    P4OUT ^= BIT6;
	    _delay_cycles(100000);
	    Index--;
	} while(Index > 0);
    P1OUT &= ~BIT0;
    P4OUT &= ~BIT6;


    _delay_cycles(1000000);
	}

	while(1);

	return 0;
}

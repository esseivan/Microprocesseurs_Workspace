//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  Texas Instruments, Inc
//  July 2013
//***************************************************************************************

#include <msp430.h>
#include <stdint.h>

#define DEFAULT_COUNTER (500)
#define DEFAULT_VALUE_ON (500)
#define DEFAULT_VALUE_OFF (500)

void delay_ms(uint16_t delay_ms) {
    for(; delay_ms != 0; delay_ms--){
        __delay_cycles(1000);
    }
}

void BlinkTypeA(void) {
    uint16_t t0 = DEFAULT_VALUE_OFF, t1 = DEFAULT_VALUE_ON;
    for(;;) {

        t0 = DEFAULT_VALUE_OFF; t1 = DEFAULT_VALUE_ON;
        if(!(P1IN & BIT1)) {                // If P1.1 pressed
            t0 = 900; t1 = 100;             // toff = 900ms, ton = 100ms
        }
        if(!(P4IN & BIT5)) {
            t0 = 100; t1 = 900;             // toff = 100ms, ton = 900ms
        }

        P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
        if(P1OUT & BIT0)                    // Led allumée
            delay_ms(t1);
        else                                // Led éteinte
            delay_ms(t0);
    }
}

void BlinkTypeB(void) {
    uint16_t Period = DEFAULT_COUNTER;

    for(;;) {
        P1OUT ^= BIT0;
        delay_ms(Period);
        if(!(P1IN & BIT1)) {
            uint16_t lp = Period;
            Period = (uint16_t)(Period * 11 / 10);
            if(Period == lp)
                Period++;
        }
        else
            Period = (uint16_t)(Period * 9 / 10);
        if(Period <= 0) {
            Period = DEFAULT_COUNTER;
        }
        if(Period > DEFAULT_COUNTER) {
            Period = 1;
        }
    }
}

void BlinkTypeC(void) {
    uint16_t Period = DEFAULT_COUNTER;

    for(;;) {
        P1OUT ^= BIT0;
        delay_ms(Period);
        if(!(P1IN & BIT1)) {
            uint16_t lp = Period;
            Period = (uint16_t)(Period * 11 / 10);
            if(Period == lp)
                Period++;
        }
        else
            Period = (uint16_t)(Period * 9 / 10);
        if(Period <= 0) {
            Period = DEFAULT_COUNTER;
        }
        if(Period > DEFAULT_COUNTER) {
            Period = 1;
        }
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output, P1.1 to input
    P1REN |= BIT1;                           // Set P1.1 to pull-up/pull-down
    P1OUT |= BIT1;                          // Set P1.1 to pull-up

    P4REN |= BIT5;
    P4OUT |= BIT5;

    BlinkTypeA();
}

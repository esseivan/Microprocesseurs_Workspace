
/**
  ****************************************************************************
  * @file    Exemple
  * @author
  * @date
  * @brief
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>


int main(void)
{
        WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode...

        P1DIR  |= BIT2; // Set P1.2 for TA1.1
        P1SEL1 &=~BIT2;
        P1SEL0 |=BIT2;

        P1DIR  |= BIT3; // Set P1.3 for TA1.2
        P1SEL1 &=~BIT3;
        P1SEL0 |=BIT3;

        P1DIR  |= BIT7; // Set P1.7 for TA1.0
        P1SEL1 |=BIT7;
        P1SEL0 |=BIT7;

        P2DIR  |=BIT0;  // Set P2.0 for ACLK
        P2SEL0 |=BIT0;
        P2SEL1 |=BIT0;

        CSCTL0_H = CSKEY_H;                     // Unlock CS registers
        CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz, as DCORSEL=0
        CSCTL2 = SELA__VLOCLK                   // Set ACLK=VLOCLK = 10kHz
                            | SELS__DCOCLK      // Set SMCLK = MCLK = DCO
                            | SELM__DCOCLK;     // Set MCLK = DCO
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 (register cleared)
        CSCTL0_H = 0;                           // Lock CS registers again

        TA1CTL = MC__STOP;                      // Stop timer first
        TA1CCR0 = 10; //
        TA1CCR1 = 4;
        TA1CCR2 = 6;
        TA1CTL |= ID__1                         // do not divide input frequency
               | TASSEL__ACLK                   // ACLK as clock source
               //| MC__UP                       // mode up
               | MC__UPDOWN                     // mode up-down
               | TACLR; // clear TAxR

        // Output mode. Modes 2, 3, 6, and 7 are not
        // useful for TAxCCR0 because EQUx= EQU0.
        TA1CCTL0 |=OUTMOD_4; // Toggle
        TA1CCTL1 |=OUTMOD_4; // Toggle
        TA1CCTL2 |=OUTMOD_4; // Toggle

        __bis_SR_register(LPM0_bits); // Enter LPM0
        __no_operation(); // For debugger
}


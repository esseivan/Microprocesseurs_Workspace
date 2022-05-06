/**
  ****************************************************************************
  * @file    MP_5-20.c
  * @author  Scherwey Roland
  * @date    24.04.2015
  * @brief   Initialization and use of Timer
  *
  *          Following ULP's deactivated: 1.1, 2.1, 3.1, 5.1, 5.3
  *

            MSP430FR5969
         -----------------
      ->|RST              |
        |                 |
 S1   ->| P4.5            |
        |            P1.0 |-->LED2
        |                 |
         -----------------

  ****************************************************************************/

#include <msp430.h>

#include "HAL/HAL_MSP430.h"


//----------------------------------------------------------------------------
// Local functions
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main(void)
{
  initChip();

  __bis_SR_register(GIE);     // enable interrupts

  for (;;)
      __bis_SR_register(LPM0_bits);

};

// Timer_A TACCR0 interrupt vector handler
#pragma vector= TIMER0_A0_VECTOR
__interrupt void timerA0_ISR(void)
{
    P1OUT ^= BIT0;      // Toggles P1.0 (LED2)
}

// Timer_A TA interrupt vector handler
#pragma vector= TIMER0_A1_VECTOR
__interrupt void timerA1_ISR(void)
{
    switch(TA0IV)
    {
    case TA0IV_TAIFG:
        TA0CTL &= ~TAIFG;   // Clear flag
        P4OUT ^= BIT6;      // Toggles P4.6 (LED1)
    default:
        break;
    }
}

/**
  ****************************************************************************
  * @file    MP_5-1.c
  * @author  Scherwey Roland
  * @date    12.04.2015
  * @brief   GPIO initialization
  *
  *
  * @note    Following ULP's deactivated: 1.1, 4.1, 5.1, 5.2, 5.3, 12.2, 13.1
  *          Optimization level: Off
  *          Level of printf_support : nofloat
  *          Heap size increased to 400bytes
  *          see http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdio.h>
#include <stdbool.h>

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings


  // a) Define P1.0 as output
  P1DIR |= BIT0;

  // b) Define P1.1 as input
  P1DIR &= ~BIT1;
  P1REN |= BIT1;
  P1OUT |= BIT1;

  // c) Output SMCLK
  P3DIR |= BIT4;
  P3SEL1 &= ~BIT4;

  // d) use of eUSCI-A1
  // TBD

  printf("\nMP_5-1 - GPIO initialization\n");

  while (1)
  {
    // get status of P1.1 (S2)
    bool const cS2_status = !(P1IN & BIT1);

    // show status of P1.1 (S2) on P1.0 (LED2)
    if (true == cS2_status)
    {
      P1OUT |= BIT0;
    }
    else
    {
      P1OUT &= ~BIT0;
    };

    // show status of P1.1 (S2) on CCS console
    printf("Switch S2 (P1.1) is : %s", (true == cS2_status) ? "set (1)\n"
                                                            : "reset (0)\n");
    __delay_cycles(1000000);
  };
}

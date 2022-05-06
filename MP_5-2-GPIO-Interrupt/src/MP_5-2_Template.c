/**
  ****************************************************************************
  * @file    MP_5-2.c
  * @author  Scherwey Roland
  * @date    17.04.2015
  * @brief   GPIO Interrupt handling
  *
  *
  * @note    Following ULP's deactivated: 1.1, 4.1, 5.1, 5.2, 5.3, 12.2, 13.1
  *          Optimization level: Off
  *
  ****************************************************************************/

#include <msp430.h>

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

  // Configure GPIO to avoid warnings
  PAOUT = PBOUT = PJOUT = 0;
  PADIR = PBDIR = 0;



  // a) Define P1.0 and P4.6 as output
  P1DIR |= BIT0;
  /*P4DIR |= BIT6;*/

  // b) Define P1.1 (S2) and P4.5 (S1) as input
  P1DIR &= ~BIT1;
  P1REN |= BIT1;
  P1OUT |= BIT1;

  /*P4DIR &= ~BIT5;
  P4REN |= BIT5;
  P4OUT |= BIT5;*/

  // c) Configure P1.1 and P4.5 to generate interrupt on falling edge
  P1IES |= BIT1;    // Set pour high to low transistion (falling edge)
  P1IFG &= ~BIT1;    // Remove potential pending interrupt
  P1IE |= BIT1;     // Activer interruption
  
  /*P4IES |= BIT5;
  P4IFG &= ~BIT5;
  P4IE |= BIT5;*/

  __bis_SR_register(GIE);     // Enable interrupts


  for (;;);
}


// Port 1 interrupt service routine
// PORT1_VECTOR définit l'adresse du vecteur interruption du port 1
// #pragma vector définit que la prochaine méthode est un vecteur interruption
// Voir table 6-4 du datasheet
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_isr(void)
{
    // d) toggle LED2
    P1IFG &= ~BIT1;

    __bis_SR_register(GIE);

    P1OUT ^= BIT0;
}

/*
// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4_isr(void)
{
    // d) toggle LED1
    P4OUT ^= BIT6;

    // Debounce
    while(!(P4IN & BIT5)) // Attendre bouton relâché
        _delay_cycles(10000); // 10ms delay


    _delay_cycles(50000); // 50ms delay
    P4IFG &= ~BIT5;

}
*/

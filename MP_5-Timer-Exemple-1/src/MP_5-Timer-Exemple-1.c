/**
  ****************************************************************************
  * @file    MP_5-Timer-Exemple.c

  * @brief   Initialization and use of Timer
  ****************************************************************************/

#include "msp430.h"
#include <stdint.h>

#define NUMBER_TIMER_CAPTURES 400

volatile uint16_t timerAcaptureValues[NUMBER_TIMER_CAPTURES]; // variable volatile: variable sur laquelle aucune optimisation de compilation n'est appliquée
int16_t timerAcapturePointer = 0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  // Configure GPIO
  P1OUT &= ~0x01;                           //
  P1DIR |= 0x01;                            // P1.0 as output

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Clock System Setup
  // CSCTL0_H = CSKEY_H;
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS ctl registers
  CSCTL2 &= ~SELA_7;
  CSCTL2 |= SELA__VLOCLK;                   // select ACLK = VLOCLK
  CSCTL0_H = 0x00;                          // Lock CS registers ?

  __delay_cycles(1000);                     // Allow clock system to settle

  // Timer0_A3 Setup
  TA0CCTL2 = CM_1 | CCIS_1 | SCS | CAP | CCIE;
                                            // CM_1 : Capture mode : flanc positif
                                            // CCIS_1 : Input CCI2B : ACLK
                                            // SCS : Mode synchrone
                                            // CAP : Mode capture
                                            // CCIE : activation interruptions

  TA0CTL = TASSEL__SMCLK | MC__CONTINUOUS;  // clock TA0 : Sub-Master clock
                                            // Mode continu

  __bis_SR_register(LPM0_bits | GIE);       // Activation GIE et mise mode veille
  __no_operation();
}

float CalculerMoyenne(void) {
    volatile float MeanVal = 0;
    int16_t i;
    for(i = 1; i < NUMBER_TIMER_CAPTURES; i++) {
        MeanVal += (timerAcaptureValues[i] - timerAcaptureValues[i-1])
                / (float)NUMBER_TIMER_CAPTURES;
    }

    _no_operation();
}

// Timer0_A3 CC1-4, TA Interrupt Handler
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
  switch (__even_in_range(TA0IV, TA0IV_TAIFG)) {
    case TA0IV_TA0CCR1:
      break;
    case TA0IV_TA0CCR2:
      timerAcaptureValues[timerAcapturePointer++] = TA0CCR2; // Sauvegarde du timestamp de la mesure
      if (timerAcapturePointer >= NUMBER_TIMER_CAPTURES) {     // Nombre de mesure terminé
          CalculerMoyenne();
        while (1) {                         // Boucle infinie
          P1OUT ^= 0x01;                    // Toggle P1.0 (LED)
          __delay_cycles(100000);
        }
      }
      break;
    case TA0IV_TA0IFG:
      break;
    default:
      break;
  }
}

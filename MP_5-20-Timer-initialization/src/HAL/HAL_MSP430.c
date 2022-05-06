/**
  ****************************************************************************
  * @file    HAL_MSP430.c
  * @author  Scherwey Roland
  * @date    25.04.2015
  * @brief   Implementation of MSP430 HAL (Hardware Abstraction Layer)
  *
  ****************************************************************************/

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "HAL_MSP430.h"


#include <msp430.h>


//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

/// @brief  This method initializes the Clock System
/// @param  -
void initCS(void)
{
  // Configure one FRAM waitstate as required by the device datasheet for MCLK
  // operation beyond 8MHz _before_ configuring the clock system.
  FRCTL0 = FRCTLPW | NWAITS_0;

  CSCTL0_H = CSKEY_H;                       // Unlock CS registers

  CSCTL1   = DCOFSEL_0;                     // Set DCO to 1MHz
  CSCTL2   = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set ACLK=VLOCLK and SMCLK = MCLK = DCO
  CSCTL3   = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 (register cleared to 0...)

  CSCTL0_H = 0;                             // Lock CS registers

}

/// @brief  This method initializes the GPIO's - also check Timer usage
/// @param  -
void initGPIO(void)
{
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Configure GPIO
  PAOUT = PBOUT = PJOUT = 0;
  PADIR = PBDIR = 0;

  P4DIR &= ~BIT5; // P4.5 (S1) input (is already configured as input after reset...)
  P4REN |=  BIT5; // Enable pull-up/pull-down on P4.5 (S1)
  P4OUT |=  BIT5; // Configure pull-up on P4.5 (S1) (see MSP430FR5969 schematics)

  // LED handling
  P1DIR |= BIT0;        // P1.0 output
  P4DIR |= BIT6;        // P4.6 output
}



void initTimer(void)
{
  TA0CCTL0  &= ~CCIE;             // TA0CCR0 interrupt disabled

  TA0CTL   =  MC__STOP            // Eteindre le timer
           |  TACLR;              // et reset
                                  // Désactiver (clear) l'interruption TAIFG


  TA0CCTL0 |= CCIE;               // TA0CCR0 interrupt enabled
  TA0CTL   |= TAIE;               // Timer A0 interrupt enabled

#warn TA0CTL to be configured according exercice
#define ex_D2 // Choix de l'exercice
#if defined(ex_B) // b) ; f=16Hz
  TA0CTL = MC__CONTINUOUS + TASSEL__SMCLK;

#elif defined(ex_C) // c) ; f=512Hz
  TA0CTL = MC__UP + TASSEL__SMCLK;
  TA0CCR0 = 2047;

#elif defined(ex_D) // d) ; f=2Hz
  TA0CCR0 = 4999;
  TA0CTL |= MC__UP + TASSEL__ACLK;
#elif defined(ex_D2) // d, 2) ; f=2Hz
  TA0CCR0 = 4999;
  TA0CTL |= MC__UP + TASSEL__ACLK;
#endif

}


//----------------------------------------------------------------------------
// Implementations
//----------------------------------------------------------------------------

void initChip(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Stop WDT

  initGPIO(); // Configure GPIO

  initCS();   // Startup clock system with max DCO setting ~8MHz

  initTimer(); // used for periodically wake-up from Low Power

}



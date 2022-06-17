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


// Modifié par Nicolas Esseiva

#include <msp430.h>
#include <stdint.h>

#define LED_HALF_PERIOD         5000 // 0.1 ms per Unit
#define STEP                    (LED_HALF_PERIOD / 20)  // Step pour petites valeurs
#define START_CCR1              (LED_HALF_PERIOD / 5) // Valeur de départ pour TA0CCR1


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    // Configure GPIO
    // P4.6 : led1
    P4OUT &= ~BIT6; // Clear P4.6 output latch for a defined power-on state
    P4DIR |= BIT6;  // Set P4.6 to output direction
    // P1.0 : led2
    P1OUT &= ~BIT0; // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;  // Set P1.0 to output direction
    P1SEL1 &= ~BIT0;    // Output : TA0.CCI1A
    P1SEL0 |= BIT0;
    // P4.5 : S1
    P4DIR &= ~BIT5;
    P4REN |= BIT5;
    P4OUT |= BIT5;
    // P1.1 : S2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    // Interrupts P1.1
    P1IES |= BIT1;    // Set pour high to low transistion (falling edge)
    P1IFG &= ~BIT1;    // Remove potential pending interrupt
    P1IE |= BIT1;     // Activer interruption
    // Interrupts P4.5
    P4IES |= BIT5;    // Set pour high to low transistion (falling edge)
    P4IFG &= ~BIT5;    // Remove potential pending interrupt
    P4IE |= BIT5;     // Activer interruption

    // Configuer ACLK comme VLOCLK
    CSCTL0 = CSKEY;    // Déverouiller les registres CS
    CSCTL2 |= SELA__VLOCLK; // ACLK = VLOCLK

    // Configure CC1
    // Valeur on : % = x/5000
    TA0CCR1 = START_CCR1;       // 1000 => 20%
    TA0CCTL1 = OUTMOD_7;        // Mode RESET / SET

    // Configure CC0
    // fclk = 10kHz ; f = 2Hz => val = 5'000
    TA0CCR0 = LED_HALF_PERIOD;             // Valeur d'arret

    // Configure Timer A0
    TA0CTL = TASSEL__ACLK |     // Timer clock = ACLK
            MC__UP |            // Timer mode UP
            TACLR;              // Reset du timer

    _bis_SR_register(GIE);      // Enable Global Interrupts

    /* Modèle par défaut
    for(;;) {
        volatile unsigned int i;            // volatile to prevent optimization

        P4OUT ^= BIT6;                      // Toggle P1.0 using exclusive-OR

        i = 10000;                          // SW Delay
        do i--;
        while(i != 0);
    }

    // Calculé : 110'007 cycles pour faire une fois la boucle.
    // Ceci correspond à 1/2 période, donc 1 période donne 220'014 cycles
    // Avec une fréquence d'horloge de 1MHz, on obtient une fréquence de :
    // 4.545 Hz
    // L'analyseur logique nous donne une fréquence de 4.513 Hz


     */

    while(1);

}

// P4 ISR
#pragma vector=PORT4_VECTOR
__interrupt void P4_ISR(void) {
    if(P4IV == P4IV_P4IFG5) {
        // Bouton S1, diminue intensité => diminuer CCR1
        if (TA0CCR1 <= STEP)
            TA0CCR1 = 1;
        else
            TA0CCR1 -= STEP;
    }
}

// P1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
    if(P1IV == P1IV_P1IFG1) {
        // Bouton S2, augmente intensité => augmenter CCR1
        if ((TA0CCR1 + STEP) >= TA0CCR0)
            TA0CCR1 = TA0CCR0;
        else if (TA0CCR1 == 1)
            TA0CCR1 += (STEP - 1);
        else
            TA0CCR1 += STEP;
    }
}


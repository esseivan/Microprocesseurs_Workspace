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
#define STEP_LOW                (LED_HALF_PERIOD / 50)  // Step pour petites valeurs
#define STEP_LOW_THRESHOLD      (LED_HALF_PERIOD / 5) // Valeur limite ou le STEP_LOW est utilisé
#define STEP_HIGH               (LED_HALF_PERIOD / 10)  // Step pour grandes valeurs
#define START_CCR1              (STEP_LOW_THRESHOLD) // Valeur de départ pour TA0CCR1


#define DEBOUNCE_S1             0x01 // Debounce sur S1
#define DEBOUNCE_S2             0x02 // Deboucne sur S2
#define SW_ON_STATE             0    // switch actif a 0

#define DEBOUNCE_COUNT          50   // 1ms per unit


int count=0,    // Nombre d'etats identiques
    io_state,   // Numero de l'entrée pour rebond
    state;      // Etat de l'entrée initial

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

    // Setup debouncer on Timer A1
    TA1CCR0=1000;                   // Make the timer count from 0 to 10000,
                                    // which will take  ~0.001 seconds

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

// S1 appuyé
void S1_Press(void) {
    // Bouton S1, diminue intensité => diminuer CCR1
    uint16_t step = 0;
    if(TA0CCR1 <= STEP_LOW_THRESHOLD)
        step = STEP_LOW;
    else
        step = STEP_HIGH;

    if (TA0CCR1 <= step)
        TA0CCR1 = 1;
    else
        TA0CCR1 -= step;

    // Indication avec LED1
    P4OUT ^= BIT6;
}

// S2 appuyé
void S2_Press(void) {
    // Bouton S2, augmente intensité => augmenter CCR1
    uint16_t step = 0;
    if(TA0CCR1 <= STEP_LOW_THRESHOLD)
        step = STEP_LOW;
    else
        step = STEP_HIGH;

    if ((TA0CCR1 + step) >= TA0CCR0)
        TA0CCR1 = TA0CCR0;
    else if (TA0CCR1 == 1)
        TA0CCR1 += (step - 1);
    else
        TA0CCR1 += step;

    // Indication avec LED1
    P4OUT ^= BIT6;
}

// P4 ISR
#pragma vector=PORT4_VECTOR
__interrupt void P4_ISR(void) {
    if(P4IV == P4IV_P4IFG5) {
        // Bouton S1

        // Désactiver interruptions et activer timer1
        count=0;                    // Remettre compteur à 0
        TA1CTL|=TASSEL_2 |          // Start timer1 with SMCLK
                MC__UP |            // Up mode
                TAIE;               // Enable interrupt
        io_state = DEBOUNCE_S1;     // Switch S2
        state= (P4IN&BIT5) ? 1 : 0; // Etat initial du switch
        P4IE&=~BIT5;                // Desactiver interruption sur S1
    }
}

// P1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
    if(P1IV == P1IV_P1IFG1) {
        // Bouton S2

        // Désactiver interruptions et activer timer1
        count=0;                    // Remettre compteur à 0
        TA1CTL|=TASSEL_2 |          // Start timer1 with SMCLK
                MC__UP |            // Up mode
                TAIE;               // Enable interrupt
        io_state = DEBOUNCE_S2;     // Switch S2
        state= (P1IN&BIT1) ? 1 : 0; // Etat initial du switch
        P1IE&=~BIT1;                // Desactiver interruption sur S2
    }
}

// Debouncer ISR
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TMR1_ISR()
{
    if(TA1IV == TA1IV_TAIFG) {
        int8_t val;
        // Timer A1 interrupt
        if(io_state == DEBOUNCE_S1) {
            // vérifier les états sur S1
            val = (P4IN & BIT5) ? 1 : 0;
            if(state == val) {
                // Si les deux sont de même état
                count++;
            } else {
                // Etat changé
                count = 0;
                // Changer l'état initial
                state = val;
            }
        } else if (io_state == DEBOUNCE_S2) {
            // vérifier les états sur S2
            val = (P1IN & BIT1) ? 1 : 0;
            if(state == val) {
                // Si les deux sont de même état
                count++;
            } else {
                // Etat changé
                count = 0;
                // Changer l'état initial
                state = val;
            }
        }

        if(count == DEBOUNCE_COUNT) {
            TA1CTL = TACLR;   // Reset du timer
            // Réactiver les interruptions des pins
            // On réactive les deux car on suppose que les deux boutons ne serront pas pressés en même temps
            P1IE |= BIT1;
            P4IE |= BIT5;

            if(state == SW_ON_STATE) // Si bouton pressé, executer les méthodes
                if(io_state == DEBOUNCE_S1)
                    S1_Press();
                else if (io_state == DEBOUNCE_S2)
                    S2_Press();
        }
    }
}

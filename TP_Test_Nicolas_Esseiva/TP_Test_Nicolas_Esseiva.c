#include <msp430.h> 
#include <stdint.h>

volatile uint8_t TimerState = 1;

void InitCS(void) {
    // La fréquence de base du CPU de 1 MHz est gardée.

    // Configuer ACLK comme VLOCLK
    CSCTL0 = CSKEY;    // Déverouiller les registres CS
    CSCTL2 |= SELA__VLOCLK; // ACLK = VLOCLK
    CSCTL3 |= DIVA__32;     // ACLK : Diviseur 32
    //CSCTL0 = 0; // Vérouiller les registres CS
}

void InitTimer(void) {

    // Timer B
    TB0CTL = TBSSEL__ACLK   // Source : ACLK
            | MC__UP        // Mode : UP jusqu'à CCR0
            | TBIE          // Interrupt enable
            | ID__8         // Diviseur par 8
            | TBCLR;        // Reset du compteur
    TB0EX0 = TBIDEX_7;     // Diviseur par 8

    // Fclk = 10/32 kHz = 312.5 Hz
    // Compte jusqu'à 16 en 10 s => f 1.6 Hz
    // On doit diviser par 195.3125

    // Je n'ai pas trouver comment arriver à avoir la fréquence correcte
    // J'ai donc pris la plus petite que j'arrivais en divisant VLOCLK (10kHz)
    // par 32*8*8 = 2048

    // CCR0 : limite du compteur
    TB0CCR0 = 15;       // Compte jusqu'à 15

    // Timer A0 comme séquence de comptage pour les sorties sur les pins
    TA0CTL = TASSEL__ACLK   // Source : ACLK
                | MC__UP        // Mode : UP jusqu'à CCR0
                | TAIE          // Interrupt enable
                | ID__4         // Diviseur par 4
                | TACLR;        // Reset du compteur
    TA0EX0 = TAIDEX_7;      // Diviseur par 8
    TA0CCR0 = 1;            // Compte jusqu'à 1
}

void InitGPIO(void) {
    // Configure GPIO
    PAOUT = PBOUT = PJOUT = 0;
    PADIR = PBDIR = 0;

    // P1.0 : sortie led
    P1DIR |= BIT0;

    // P1.4
    P1DIR |= BIT4;

    // P1.5
    P1DIR |= BIT5;

    // P1.6
    P1DIR |= BIT6;

    // P1.7
    P1DIR |= BIT7;

    // P2.0 : sortie ACLK
    P2DIR |= BIT0;
    P2SEL1 |= BIT0;
    P2SEL0 |= BIT0;


    // P1.1 : S2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;  // Pull-up
    P1OUT |= BIT1;
        // Interruptions
    P1IES |= BIT1;    // Set pour high to low transistion (falling edge)
    P1IFG &= ~BIT1;    // Remove potential pending interrupt
    P1IE |= BIT1;     // Activer interruption



}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	InitGPIO();
	InitCS();
	InitTimer();

    _bis_SR_register(GIE);      // Enable Global Interrupts



	while(1){
	    __bis_SR_register(LPM0_bits); // Enter LPM0
	    __no_operation();
	}

}

// Timer B ISR
#pragma vector=TIMER0_B1_VECTOR
__interrupt void timerB_ISR(void) {
    switch (TBIV) {
        case TB0IV_TB0IFG: // Overflow du timer : toggle led
            P1OUT ^= BIT0;
            break;
        default: // Tous les autres : rien
            break;
    }


}

// Timer A0 ISR
#pragma vector= TIMER0_A1_VECTOR
__interrupt void timerA1_ISR(void)
{
    switch(TA0IV)
    {
    case TA0IV_TAIFG:
        // P1.4 change tout le temps
        P1OUT ^= BIT4;
        // P1.5 change quand P1.4 passe à 0
        if((P1OUT & BIT4) == 0)
            P1OUT ^= BIT5;
        // etc... jusqu'à P1.7
        if((P1OUT & (BIT5 | BIT4)) == 0)
            P1OUT ^= BIT6;
        if((P1OUT & (BIT6 | BIT5 | BIT4)) == 0)
            P1OUT ^= BIT7;

        __no_operation(); // pour debugger
        break;
    default:
        break;
    }
}

// P1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
    switch (P1IV) {
        case P1IV_P1IFG1:
            // Bouton S2
            if(TimerState == 1) {
                TimerState = 0;
                TB0CTL &= ~MC_3; // Eteindre le timer
            } else if (TimerState == 0) {
                TimerState = 1;
                TB0CTL |= MC__UP; // Allumer le timer
            }
            break;
        default:
            break;
    }
}

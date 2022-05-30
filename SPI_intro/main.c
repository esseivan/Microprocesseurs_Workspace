#include <msp430.h> 
#include <stdint.h>

// Introduction au SPI
// Esseiva Nicolas
// 25.05.2022

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    // SPI config (p. 816)
    UCB0CTLW0 = UCSWRST;    // Reset
    UCB0CTLW0 |= UCMSB |    // MSB first
            UCMST |         // Master mode
            UCMODE_2 |      // Mode 2 : 4 wire spi, active low CS
            UCSYNC |        // Synchronous mode (SPI)
            UCSTEM |        // ste pin is used to generate the enable signal for 4-wire slave
            UCSSEL__SMCLK;  // SMCLK clock : 1 MHz
    UCB0BRW = 10;           // Diviseur de fréquence : 10 => 100 kHz
    UCB0STATW |= UCLISTEN;  // Internal feedback : TX <-> RX

    // P1.6 : UCB0MOSI
    P1SEL1 |= BIT6;
    P1SEL0 &= ~BIT6;

    // P1.7 : UCB0MISO
    P1SEL1 |= BIT7;
    P1SEL0 &= ~BIT7;

    // P2.2: UCB0CLK
    P2SEL1 |= BIT2;
    P2SEL0 &= ~BIT2;

    PM5CTL0 &= ~LOCKLPM5; // Unlock GPIOs
    __delay_cycles(1000); // Attendre 1ms


    UCB0CTLW0 &= ~UCSWRST;  // Activer SPI


    volatile uint8_t dataRX;
    while(1) {
        // Envoi SPI
        UCB0TXBUF = 0x2A;     // Données à envoyer (8 bits)

        //while(!(UCB0IFG & UCTXIFG)); // Attendre envoi complète
        //UCB0IFG &= ~UCTXIFG;

        while(!(UCB0IFG & UCRXIFG)); // Attendre réception complète

        UCB0IFG &= ~(UCRXIFG | UCTXIFG);
        dataRX = UCB0RXBUF;

        __delay_cycles(1000);
    }
	



	return 0;
}

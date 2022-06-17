#include <msp430.h> 
#include <stdint.h>

/*****
 * Author: Esseiva Nicolas
 * Date: 04.05.2022
 * Description : Algorithme min,max,mean + tri de tableau
 * Fichier : main.c
 */

#define NB_VAL 32
#define DELAY_X_US 250000 // 250 * 1000us = 250ms

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~BIT0;           // Unlock gpios
	P1OUT = P2OUT = P3OUT = P4OUT = 0; // default values

	// P1.0 as output
	P1DIR |= BIT0;

	// https://onlineintegertools.com/create-integer-array
	// 32 valeurs
	int16_t cArray[NB_VAL] = {-245, 115, 127, 234, 53, 166, -219, 71, 153,
	                          51, 109, -79, -37, 39, -62, 108, -173, 25, -3,
	                          122, -57, -131, -60, -157, 227, 123, 142, 146,
	                          -196, -152, -96, -103};

	// i : min, max, mean
    int16_t min = cArray[0];
    int16_t max = cArray[0];
    int16_t sum = cArray[0];
	uint8_t i = 1;
	while(i < NB_VAL) {
	    int16_t k = cArray[i];      // Element place i
	    if(k < min)                 // Si k < min => min = k
	        min = k;
	    if(k > max)                 // Si k > max => max = k
	        max = k;
	    sum += k;                   // Sum = Sum + k
	    i++;
	}
	float mean = (float)sum / NB_VAL;
	// min = -245
	// max = 234
	// mean = 7.53


	// ii : clignotement
	int8_t n = (uint8_t)mean;
	while(n > 0) {
	    P1OUT |= BIT0;              // Led on
	    _delay_cycles(DELAY_X_US);  // Delay
        P1OUT &= ~BIT0;             // Led on
        _delay_cycles(DELAY_X_US);  // Delay
        n--;                        // Décrementer n
	}

	// iii : tri décroissant du tableau
	uint8_t fini;
	do {
        fini = 1;
        i = 1;

        do {
            if(cArray[i-1] < cArray[i]) {   // Si élément de gauche plus petit
                int16_t temp = cArray[i-1]; // Intervertir les deux éléments
                cArray[i-1] = cArray[i];
                cArray[i] = temp;
                fini = 0;                   // Indiquer que pas fini
            }

            i++;
        } while (i < NB_VAL);               // Parcourir le tableau
	} while (fini == 0);    // Recomencer jusqu'à aucun changement effectué

	while(1);
}

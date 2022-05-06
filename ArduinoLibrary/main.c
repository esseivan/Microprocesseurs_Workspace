#include <arduino.h>
#include <msp430.h> 

/**
 * main.c
 */

#define PIN_LED 10
#define PIN_BTN 11

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initialise();

	pinMode(PIN_LED, OUTPUT);   // led output
	digitalWrite(PIN_LED, LOW); // éteinte au debut
	
	pinMode(PIN_BTN, INPUT);    // btn input pull up
	pinResistor(PIN_BTN, PULL_UP);

	while(1) {
	    while(digitalRead(PIN_BTN) == IN_OFF); // Tant que bouton non pressé

        digitalToggle(PIN_LED);

        _delay_cycles(10000); // Attendre 10ms
        while(digitalRead(PIN_BTN) == IN_ON); // Tant que bouton pressé
	}

	return 0;
}

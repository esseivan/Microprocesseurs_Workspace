/*
 * pinmode.h
 *
 *  Created on: 14 Apr 2022
 *      Author: nicolas
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#define BTN_ACTIVE_LOW

// IO modes
#define INPUT   0
#define OUTPUT  1
// Output values
#define HIGH    1
#define LOW     0
// Resistor values
#define NONE    0
#define PULL_UP 1
#define PULL_DOWN 2

// Output state when set
#define DEFAULT_OUTPUT LOW

// Input values
#ifdef BTN_ACTIVE_LOW
#define IN_ON   0
#define IN_OFF  1
#elif defined (BTN_ACTIVE_HIGH)
#define IN_ON   1
#define IN_OFF  0
#endif

#include <stdint.h>

#define GET_PORT_IN(x)    (x + 0)
#define GET_PORT_OUT(x)   (x + 2)
#define GET_PORT_DIR(x)   (x + 4)
#define GET_PORT_REN(x)   (x + 6)

void initialise(void);
void pinMode(uint8_t index, uint8_t state);
void pinResistor(uint8_t index, uint8_t state);
void digitalWrite(uint8_t index, uint8_t value);
uint8_t digitalRead(uint8_t index);
void digitalToggle(uint8_t index);
uint8_t readInvert(uint8_t value);


#endif /* ARDUINO_H_ */

/*
 * pinmode.c
 *
 *  Created on: 14 Apr 2022
 *      Author: nicolas
 */

#include <arduino.h>
#include <msp430.h>

// Initialisation
void initialise(void) {
    PM5CTL0 &= ~BIT0;           // Unlock IOs
}

// Récupère le registre pour la pin
uint8_t* getReg(uint8_t index) {
    uint8_t port = index / 10;
    uint8_t* regPtr = ((void *) 0);

    switch (port) {
        case 1:
            regPtr = (uint8_t*)&P1IN;
            break;
        case 2:
            regPtr = (uint8_t*)&P2IN;
            break;
        case 3:
            regPtr = (uint8_t*)&P3IN;
            break;
        case 4:
            regPtr = (uint8_t*)&P4IN;
            break;
        default:
            break;
    }

    return regPtr;
}

// Etat de la pin (OUTPUT, INPUT)
void pinMode(uint8_t index, uint8_t state) {
    uint8_t* regPtr = getReg(index);
    uint8_t bit = index % 10;
    uint8_t mask = 0x01 << bit;
    *GET_PORT_OUT(regPtr) &= ~mask;
    if (state == OUTPUT) {
        // Set default output
        if (DEFAULT_OUTPUT == HIGH)
            *GET_PORT_OUT(regPtr) |= mask;  // Set to 1
        else
            *GET_PORT_OUT(regPtr) &= ~mask;  // Set to 0

        *GET_PORT_DIR(regPtr) |= mask;  // Output : set to 1
    }
    else
        *GET_PORT_DIR(regPtr) &= ~mask;  // Intput : set to 0
}

// Appliquer le mode de résistance (aucune, pullup, pulldown)
void pinResistor(uint8_t index, uint8_t state) {
    uint8_t* regPtr = getReg(index);
    uint8_t bit = index % 10;
    uint8_t mask = 0x01 << bit;

    if (state == PULL_UP) {
        *GET_PORT_REN(regPtr) |= mask;
        *GET_PORT_OUT(regPtr) |= mask;
    }
    else if (state == PULL_DOWN) {
        *GET_PORT_REN(regPtr) |= mask;
        *GET_PORT_OUT(regPtr) &= ~mask;

    }
    else {
        *GET_PORT_REN(regPtr) &= ~mask;
    }
}

// Ecriture de la pin
void digitalWrite(uint8_t index, uint8_t value) {
    uint8_t* regPtr = getReg(index);
    uint8_t bit = index % 10;
    uint8_t mask = 0x01 << bit;
    if (value == LOW) // == 0
        *GET_PORT_OUT(regPtr) &= ~mask;  // Set to 0
    else
        *GET_PORT_OUT(regPtr) |= mask;  // Set to 1
}

// Lecture de la pin
uint8_t digitalRead(uint8_t index) {
    uint8_t* regPtr = getReg(index);
    uint8_t bit = index % 10;
    uint8_t mask = 0x01 << bit;
    uint8_t value = (*GET_PORT_IN(regPtr) & mask);
    return (value == 0) ? 0 : 1;
}

// Change l'état de la sortie indiquée
void digitalToggle(uint8_t index) {
    uint8_t* regPtr = getReg(index);
    uint8_t bit = index % 10;
    uint8_t mask = 0x01 << bit;
    uint8_t value = (*GET_PORT_IN(regPtr) & mask);
    if (value == HIGH)
        *GET_PORT_OUT(regPtr) &= ~mask;  // Set to 0
    else
        *GET_PORT_OUT(regPtr) |= mask;  // Set to 1
}

// Retourne la valeur inversée de la pin sur 1 bit
uint8_t readInvert(uint8_t value) {
    if(value == HIGH) return LOW;
    else return HIGH;
}




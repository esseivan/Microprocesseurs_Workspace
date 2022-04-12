/**
  ****************************************************************************
  * @file    MP_4_1.c
  * @author  Esseiva Nicolas
  * @date    01.04.2022
  * @brief   Opérateurs de base (addition, soustraction, multiplication,
  *          division, module), opérateurs d'accumulation et opérateurs
  *          binaires en C
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

void executeAdditions_a(void) {
    uint16_t var1 = 33;
    uint8_t var2 = 200;
    var1 += var2;
    var2 -= 50;
}

void executeMultiplications_b(void) {
    uint16_t var1 = 33;
    uint8_t var2 = 11;
    var1 *= var2;
    var2 *= 5;
    var2 /= 3;
}

void executeBinaryOperators_c(void) {
    int a=10;
    int b=20;
    int c;
    c = a | b; // c = 30 = 0x1E
    c = a & b; // c = 0 = 0x00
    c = a ^ b; // c = 30 = 0x1E
    c = ~a; // c = -11 = 0xFFF5
}

void executeShiftOperatorsd_d(void) {
    uint16_t a = 0x1C;
    uint16_t b;
    b = a << 2; ; // b = 112 = 0x70, a = 0x1C
    b = a >> 4; // b = 1 = 0x01, a = 0x1C
    a <<= 3; // b = 0x01, a = 224 = 0xE0
}

void executeShiftOperators_e(void) {
    uint16_t a = 0x2A;
    uint16_t b;
    b = a << 1; // b = 84 = 0x54
    b = b >> 3; // b = 10 = 0x0A
    a <<= b; // a = 43008 = 0xA800
}

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  executeAdditions_a();
  executeMultiplications_b();
  executeBinaryOperators_c();
  executeShiftOperatorsd_d();
  executeShiftOperators_e();


  return 0;
}

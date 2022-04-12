/**
  ****************************************************************************
  * @file    MP_4-1_template.c
  * @author  Esseiva Nicolas
  * @date    31.03.2022
  * @brief   Opérateurs de base (addition, soustraction, multiplication,
  *          division, module), opérateurs d'accumulation et opérateurs
  *          binaires en C
  *
  ****************************************************************************/

#include <msp430.h> 
#include <stdint.h>

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	float var1 = 733.26;

	uint16_t var2 = (uint16_t)var1;
    uint16_t var3 = var1;


	return 0;
}

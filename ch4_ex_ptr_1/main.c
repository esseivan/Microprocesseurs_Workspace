#include <msp430.h> 
#include <stdint.h>


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	char var1 = 'A';
	char *var2_p;

	var2_p = &var1;
	*var2_p = 'B';

	uint16_t tbl[3]={5,6,7};
	uint16_t *tbl_p;

    tbl_p = &tbl[2];
    tbl_p = &tbl;       // Ces deux lignes sont identiques
	tbl_p = tbl;
    *var2_p = tbl[-1]; // correspond a *(tbl_p - 1)
    *var2_p = tbl[2];  // correspond a *(tbl_p + 2)


    float *var3_p;

    var3_p = &var1;

    uint16_t v1 = 1;
    uint16_t v2 = 2;
    uint16_t *p1;
    uint16_t *p2;

    p1 = &v1;
    p2 = &v2;

    const uint16_t *p3 = &v1; // => p3 = &v1
    // ou :
    uint16_t *p4;
    p4 = &v1; // => p4 = &v1

    p2 = &p1;
    p2 = *p1;
    p2 = p1;

    p4 = p4 + 1;



	return 0;
}

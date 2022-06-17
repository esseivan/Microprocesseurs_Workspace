/**
  ****************************************************************************
  * @file    MP_4-22.c
  * @author  Nicolas Esseiva
  * @date    10.06.2022
  * @brief   Use of different averaging algorithms
  *
  *          see http://en.wikipedia.org/wiki/Moving_average
  *
  *
  * @note    Following ULP's deactivated: 1.1, 4.1, 5.1, 5.2, 5.3, 12.2, 13.1
  *          Optimization level: Off
  *          Level of printf_support : nofloat
  *          Heap size increased to 400bytes
  *          see http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------

#define eMAX_BUFFER  16
#define S_COUNT 4
// Le static ici restreint cette variable à ce fichier
static int16_t myData[eMAX_BUFFER];

/*En C, dans une fonction, on utilise static pour
déclarer des variables avec une durée de vie statique.
Ces variables gardent leur valeur au cours des multiples appels de la fonction.
Ces variables sont initialisées une seule fois au moment de la compilation.
Leur durée de vie correspond à la durée de vie de notre programme.

static <variable type> <variable name>
<variable type> static <variable name>

*/

//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------

/// @brief  Adds a new sample to the buffer which is used as ring buffer
/// @note   buffer and index are declared on module scope
/// @param  sample - sample to be added
void addSample(int16_t sample)
{
    static uint16_t i = 0;
    myData[i++] = sample;
    // Mettre la constante à gauche
    // Si on oublie un '=', on a une erreur au lieu d'un warning
    // if(eMAX_BUFFER = i) : erreur de compilation
    if(eMAX_BUFFER == i)
        i = 0;
}

/// @brief  Compute average over all buffer entries
/// @note   buffer is declared on module scope
/// @return computed average using all values in buffer
int16_t getAverage(void)
{
    // 16 * int16_t => maximum 32 bits
    int32_t sum = 0;

    #warn b) getAverage() to be implemented...
    for(uint16_t i = 0;i < eMAX_BUFFER; i++) {
        sum += myData[i];
    }

    return sum / eMAX_BUFFER;
}

/// @brief  Filter algorithm with rolling average
///         using local basic FIR ring buffer
/// @return computed average using all values in buffer
int16_t getRollingAverage(int16_t sample)
{
    // on a une 'fenetre' qui fait la moyenne de x valeurs de l'échantillon total
    // cette fenetre se deplace sur les échantillons en gardant les 4 nouveaux

#warn c) getRollingAverage() to be implemented...


    static int16_t buffer[S_COUNT];
    static uint16_t index = 0;
    static int32_t sum = 0;

    sum += sample - buffer[index];
    buffer[index++] = sample;

    if(S_COUNT == index)
        index = 0;

    return sum / S_COUNT;
}

/// @brief  Filter algorithm with running average
///         without local ring buffer
/// @return computed running average
int16_t getRunningAverage(int16_t sample)
{
    static int32_t sum = 0;

    #warn d) getRunningAverage() to be implemented...
    sum -= sum / eMAX_BUFFER;
    sum += sample;

    return sum / eMAX_BUFFER;
}

/// @brief  Print the values available in the array
/// @param  arr - pointer to array with values to be printed
/// @param  nbrOfArrayValues number of values in the table
/// @return -
void showArray(int16_t arr[], uint16_t nbrOfArrayValues)
{
  printf("Actual array values are [");
  uint16_t i;
  for (i=0; i<nbrOfArrayValues-1; i++)
  {
    printf(" %02i,", arr[i]);
  };
  printf(" %02i]", arr[i]);
}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings

  // Initialization for random number generator, set the seed
  srand(0xFA18); // should be parametrized with "random" number...

  while (1)
  {
    // generate random sample value between 32..63
    const int16_t cNbr = (int16_t)(rand() % 32 + 32);
    printf("cNbr: %02i ", cNbr);

    // a) add random value in table
    addSample(cNbr);
    showArray(myData,eMAX_BUFFER);

    // b) get average value calculating average over all buffer entries
    const int16_t cAverage = getAverage();
    printf(" - %02i", cAverage);

    // c) get average value using rolling average mechanism
    const int16_t cRollingAverage = getRollingAverage(cNbr);
    printf(" - %02i", cRollingAverage);

    // d) get average value using running average mechanism
    const int16_t cRunningAverage = getRunningAverage(cNbr);
    printf(" - %02i\n", cRunningAverage);
  }
}

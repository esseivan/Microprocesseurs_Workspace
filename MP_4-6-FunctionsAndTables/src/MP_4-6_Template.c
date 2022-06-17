/**
  ****************************************************************************
  * @file    MP_4-6.c
  * @author
  * @date
  * @brief   Table processing using pointers and function usage
  *
  * @note    Following ULP's deactivated: 1.1, 4.1, 5.1, 5.2, 5.3, 13.1
  *          Optimization level: Off
  *          Level of printf_support : nofloat
  *          Heap size increased to 400bytes
  *          see http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#define NBR_OF_VALUES 12


// Le tableau définit ci-dessous à ses valeurs constantes. On ne peut pas les modifiers
const int16_t cArray [NBR_OF_VALUES] = {-10, 5, 8, 10, 55, 33, 44, 55, 44, 33, 0, 6};
int16_t newArray[NBR_OF_VALUES];


/// @brief  Search the highest value in an array using for loop
/// @param  arr - pointer to array with values to search for max value
/// @param  nbrOfArrayValues number of values in the table
/// @return largest value
int16_t findLargestValue(const int16_t * const arr_ptr, uint16_t nbrOfArrayValues)
{
#warn 1) Implement findLargestValue
    int16_t max = *(arr_ptr);
    uint16_t i = 1;
    for(;i < nbrOfArrayValues; i++) {
        if(*(arr_ptr + i) > max)
            max = *(arr_ptr + i);
    }

    return max;
}

/// @brief  Search the number of occurrences of a value in an array
/// @param  arr_ptr - pointer to array with values to search for max value
/// @param  nbrOfArrayValues - number of values in the table
/// @param  value - value to search for
/// @return largest value
uint16_t nbrOfOccurrences(const int16_t *arr_ptr, uint16_t nbrOfArrayValues, int16_t value)
{
    #warn 2) Implement nbrOfOccurrences
    uint16_t count = 0;
    uint16_t i = 0;
    for(; i < nbrOfArrayValues; i++) {
        if(*(arr_ptr + i) == value)
            count++;
    }

    return count;
}

/// @brief  initialize an array of signed integers with inverted order of
///         existing array
/// @param  dest_ptr - pointer to destination array
/// @param  src_ptr  - pointer to array with source values
/// @param  nbrOfArrayValues number of values in the table
/// @return -
void inverseArray(int16_t * const dest_ptr, const int16_t * const src_ptr, uint16_t nbrOfArrayValues)
{
    #warn 3) Implement inverseArray
    uint16_t i = 0;
    for(; i < nbrOfArrayValues; i++) {
        *(dest_ptr + i) = *(src_ptr + nbrOfArrayValues - 1 - i);
    }
}

/// @brief  Print the values available in the array
/// @param  arr_ptr - pointer to array with values to be printed
/// @param  nbrOfArrayValues number of values in the table
/// @return -
void showArray(const int16_t * const arr_ptr // adresse ET contenu const
               , uint16_t nbrOfArrayValues)
{
  printf("at address 0x%p is [", arr_ptr);
  uint16_t i=0;
  for (; i<nbrOfArrayValues-1; i++)
  {
    printf(" %i,", *(arr_ptr+i));
  }
  printf(" %i]\n", *(arr_ptr+i)); // Pour ne pas avoir la virgule de fin
}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings

  // a) find largest value in an array of signed integers
  const int16_t cLargestValue = findLargestValue(cArray, NBR_OF_VALUES);
  printf("Array to search for ");
  showArray(cArray,NBR_OF_VALUES);
  printf("Largest value found is : %i\n", cLargestValue);   // %i pour int

  // b) find number of occurrences of a largest value in an array of signed integers
  const uint16_t cNbrOfOccurrences = nbrOfOccurrences(cArray, NBR_OF_VALUES, cLargestValue);
  printf("Number of occurrences of value %i in array is: %u\n", cLargestValue, cNbrOfOccurrences);

  // c) initialize a new array of signed integers with inverted order of existing array
  printf("Array before ");
  showArray(cArray, NBR_OF_VALUES);
  inverseArray(newArray, cArray, NBR_OF_VALUES);
  printf("Array after ");
  showArray(newArray,NBR_OF_VALUES);

  for (;;);
}

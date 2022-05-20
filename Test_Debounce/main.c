

/*
 * This code illustrates a method to debounce a
 * button effectively with Timers and Interrupt */

/* https://github.com/alfy7/MSP430-Launchpad-Examples/blob/master/12_Button_Proper_Debouncing.c */

int count=0,state; //Declare required variables

#include <msp430.h> */

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    P1DIR &=~BIT1;                  // Set P1.1 as input
    P1REN |=BIT1;                   // Enable pullup/pulldown resistors for P1.1
    P1OUT |=BIT1;                   // Set P1.3 to have pull up resistors

    P1IE |=BIT1;                    // Enable interrupt on P1.1
    P1IES |=BIT1;                   // Set interrupt flag on the falling edge of logic level on P1.3
    P1IFG&=~BIT1;                   // Reset Port1 interrupt flag

    TA0CCR0=1000;                   // Make the timer count from 0 to 10000,
                                    // which will take  ~0.001 seconds
    __bis_SR_register(GIE);         // Enable maskable interrupts

    P1DIR |=BIT0;                   // Set P1.0 as output
    P1OUT &=~BIT0;                  // Initially turn off the LED2

    P4DIR |=BIT6;                   // Set  P4.6 as output
    P4OUT &=~BIT6;                  // Initially turn off the LED1
    PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default ...

   while(1) //Run code forever
   {
       P1OUT ^=BIT0;                //Toggle LED2 on P1.0
       __delay_cycles(2000);        //Delay for a while

   }
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_Function()
{
    count=0; //Reset count
    TA0CTL|=TASSEL_2+MC_1+TAIE; // Start Timer0 with SMCLK clock source, UP mode and enable overflow interrupt
    state=(P1IN&BIT1)>>1;       // Save the state of the switch
    P1IE&=~BIT1;                // Disable interrupt on P1.1, now the Timer will take care of Debouncing
    P1IFG&=~BIT1;               // Reset Port1 interrupt flag
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TMR0()
{
    if(TA0IV==TA0IV_TAIFG)              // Check if Timer overflow caused the interrupt
                                        // the same interrupt vector. Here it is only optional.
        {
            if(state==((P1IN&BIT1)>>1)) // If the state of the switch is the same
                count++;                // Increment the counter variable
            else
            {
                count=0;                // If not same, reset the counter variable
                state=((P1IN&BIT1)>>1); // And save the present state of the switch
            }
        if(count==10)                   // If the state has been consistently the same
            {
            if(state==0)                // If the switch was pressed
                P4OUT^=BIT6;            // Toggle the LED1
            P1IE|=BIT1;                 // We have handled the debouncing,
                                        // now we again enable interrupt on P1.1,
                                        // for it to again detect switch bounce
            TA0CTL=0;                   // Stop the Timer
            TA0CTL|=TACLR;              // Clear the Timer counter
            }

            TA0CTL&=~(TAIFG);           //Reset the interrupt flag
        }
}

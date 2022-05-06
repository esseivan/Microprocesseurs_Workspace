;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack

            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

            bic.w 	#LOCKLPM5,&PM5CTL0 		;Unlock I/O Pins. Disable lock

			bis.b 	#BIT0,&P1DIR 				; P1.0 output (LED2 verte)
			bic.b 	#BIT0,&P1OUT 				; initialize LED2 (verte) to off

			bis.b 	#BIT6,&P4DIR 				; P4.6 output (LED1 rouge)
			bic.b 	#BIT6,&P4OUT 				; initialize LED1 (Rouge) to off

		    bic.b 	#BIT1,&P1DIR 				; P1.1 (S2) as input with pullup resistor
		    bis.b 	#BIT1,&P1REN 				;
		    bis.b 	#BIT1,&P1OUT 				;

		    bic.b 	#BIT5,&P4DIR 				; P4.5 (S2) as input with pullup resistor
		    bis.b 	#BIT5,&P4REN 				;
		    bis.b 	#BIT5,&P4OUT 				;

            mov.b   #BIT1,&P1IES            	; P1.1 interrupt enabled
            mov.b   #BIT1,&P1IE          	    ; P1.1 interrupt enabled

            ;mov.b   #BIT0,&P1IES         	    ; P1.0 interrupt enabled
            ;mov.b   #BIT0,&P1IE          	    ; P1.0 interrupt enabled

            mov.b   #BIT5,&P4IES         	    ; P4.5 interrupt enabled
            mov.b   #BIT5,&P4IE          	    ; P4.5 interrupt enabled


            clr.b   &P1IFG                  ; Clear all P1 interrupt flags
            clr.b   &P4IFG                  ; Clear all P1 interrupt flags

            nop
            bis.w   #GIE,SR
            nop

loop: 		bic.b #0x01,&P1OUT 				; clear P1.0 LED OFF
			nop
			call #delay
			bis.b #0x01,&P1OUT 				; set   P1.0 LED ON
			call #delay

			jmp loop

delay:
			mov #0xFFFF, r10				; load r10 with 0xFFFF
wloop1: 	dec r10							; Decrement r10
			jnz wloop1
			ret

            
;------------------------------------------------------------------------------
P1_ISR ;    Port 1 Interrupt latency 6
;------------------------------------------------------------------------------
;Interrupt handler for P1 Cycles

			ADD &P1IV,PC 				 ; Add offset to Jump table 3
			reti 						 ; Vector 0: No interrupt 5
			JMP P1_0_HND 				 ; Vector 2: Port 1 bit 0 2
			JMP P1_1_HND 				 ; Vector 4: Port 1 bit 1 2
			JMP P1_2_HND 				 ; Vector 6: Port 1 bit 2 2
			JMP P1_3_HND 				 ; Vector 8: Port 1 bit 3 2
			JMP P1_4_HND 				 ; Vector 10: Port 1 bit 4 2
			JMP P1_5_HND 				 ; Vector 12: Port 1 bit 5 2
			JMP P1_6_HND 				 ; Vector 14: Port 1 bit 6 2
			JMP P1_7_HND 				 ; Vector 16: Port 1 bit 7 2
P1_7_HND								 ; Vector 16: Port 1 bit 7
			 							 ; Task starts here
			reti 						 ; Back to main program 5
P1_6_HND 					 			 ; Vector 14: Port 1 bit 6
			 						     ; Task starts here
			reti 						 ; Back to main program 5
P1_5_HND 					             ; Vector 12: Port 1 bit 5
								         ; Task starts here
			reti 						 ; Back to main program 5
P1_4_HND 					 			 ; Vector 10: Port 1 bit 4
										 ; Task starts here
			reti 						 ; Back to main program 5
P1_3_HND 					 			 ; Vector 8: Port 1 bit 3
			       					     ; Task starts here
			reti 						 ; Back to main program 5
P1_2_HND 					             ; Vector 6: Port 1 bit 2
									     ; Task starts here
			reti						 ; Back to main program 5
P1_1_HND 								 ; Vector 4: Port 1 bit 1
			bic.b   #BIT1,&P1IFG         ; Clear P1.1 IFG
            xor.b   #BIT6,&P4OUT
			reti                         ; Back to main program 5
P1_0_HND 					 			 ; Vector 2: Port 1 bit 0
								 		 ; Task starts here
			reti						 ; Back to main program 5


;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   PORT1_VECTOR            ; Port 1 Vector
            .short  P1_ISR                  ;
            .end

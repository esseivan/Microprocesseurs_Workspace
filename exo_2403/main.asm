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

			bic.w #LOCKLPM5,&PM5CTL0		; Unlock GPIOs
			bis.b #0x01, &P1DIR				; Set pin0 as output
			bic.b #BIT1, &P1DIR				; Set pin1 as input
			bis.b #0x01, &P1OUT				; Turn led on
			mov.w #0xFFFF, r10

MainLoop:	bic.b #0x01, &P1OUT				; Set output low
			call #Delay						; Delay
			bis.b #0x01, &P1OUT				; Set output high
			call #Delay						; Delay

			mov.b &P1IN, r4
			and.b #BIT1, R4					; Read input
			jz MainLoop
			sub.w #0x0100, r10
			jmp MainLoop					; Loop



											; Fcyc = 1MHz
Delay:		mov.w r10, r9 					; Load r9 with 0xFFFF
Loop:		dec.w r9						; Decrement r9
			jnz Loop
			ret

			; 3 instructions par boucle
			; 1 MIPS => 1us par instruction => 2us
			; pour 0xFFFF répétition : 196'605 us = 0.196s
			; + 1 instruction (mov) => 196'606 us

			nop
eof:		jmp eof
			nop




;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            

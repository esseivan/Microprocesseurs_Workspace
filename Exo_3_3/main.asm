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
NB_VAL:		.equ 16 ; number of Values constant
;-------------------------------------------------------------------------------
			.sect .data ; section of initialized data
Values: 	.byte 122,23,31,27,54,-2,-17 ; table of 8-bit signed numbers
			.byte -5,112,13,56,65,21,87
			.byte -32,-54
;-------------------------------------------------------------------------------
			.sect .bss ; section of uninitialized data
			.align 2 ; specify a word alignement
Sum: 		.space 2 ; reserve 1 word for variable Sum
Mean: 		.space 1 ; reserve 1 byte for variable Mean


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
; Main loop
;-------------------------------------------------------------------------------

; a)
; Sum table
			mov.w #0, Sum
			mov.b #0, r15 ; Index
			mov.w #Values, r4	; Copy target address
Loop:		mov.b @r4+, r6 ; Get the byte, .b must be used
			sxt r6 ; Converti la valeur signée 8bit en 16bit
			add.w r6, Sum ; Add the value
			inc.b r15 ; Increment index

			; Check index value
			cmp.w #NB_VAL, r15
			jlo Loop ; Skip when NB_VALUES - index = 0

; b)

			mov.w Sum, r7 ; Copy Sum to r7
			mov.b #NB_VAL, r5 ; Copy NB_VALUES to r5
			rra.b r5 ; First roll of r5
Loop2:		rra.w r7 ; Divide r7 by 2
			rra.b r5 ; Divide r5 by 2
			jnz Loop2 ; Skip when r5 = 0
			nop
			mov.b r7, Mean ; Copy r7 to mean
			mov.w Sum, r8 ; Sum can be read on R8, Mean can be read on R7

            nop				;

EndLoop:	jmp EndLoop
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


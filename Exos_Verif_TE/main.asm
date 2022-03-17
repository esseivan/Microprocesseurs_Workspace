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

; i)
			mov.w 	#0x7FF7, 	R10
			mov.w 	#10,		R11
			add.w 	R11,		R10

; j)
			mov.w 	#91, 		R10
			mov.w 	#54,		R11
			add.b 	R11,		R10
			addc.b	#010,		R10

; k)
			mov.w 	#0x009B, 	R10
			mov.w 	#0xFF65,	R11
			add.b 	R11,		R10

; l)
			bic.w	#0x107,		SR
			mov.w 	#-2, 		R10
			rra.w	R10

; m)
			mov.w	#0x99,		R10
			mov.w 	#0x11, 		R11
			sub.w	R11,		R10

			nop
			nop


; Tests :
; 1) Nombre négatif + Nombre positif dont résultat positif (ou 0) => carry tout le temps

			mov.w	#-25,		R10
			mov.w 	#36, 		R11
			add.w	R11,		R10 ; 36 + (-25) = 11 mais génère un carry !


; 1) Nombre négatif + Nombre positif dont résultat négatif (!= 0) => jamais de carry

			mov.w	#-36,		R10
			mov.w 	#25, 		R11
			add.w	R11,		R10 ; 25 + (-36) = -11, aucun carry


			nop
			nop


; exo 3.4 : désassembler les instructions
			bis.b #1, &PADIR_L ; D3D2 0204
			bis.b #1, 0x204(SR) ; D3D2 0204


; Test Constant Generator
			clr.w	r4
			clr.w	r5
			mov.b 	r2,			r4 ; Register mode used => r2 is SR
			mov.b	r3,			r5 ; Register mode used => r3 is 0
			mov.b 	0(r2),			r4 ; Indexed mode (01) used => r2 is the address origin and constant as (0)
			mov.b	0(r3),			r5 ; Indexed mode (01) used => r3 is +1
			mov.b 	@r2,		r4 ; Indirect register mode (10) used => r2 is +4
			mov.b	@r3,		r5 ; Indirect register mode (10) used => r3 is +2
			mov.b 	#8,			r4 ; Immediate (11) used => r2 is used and is +8
			mov.b	#-1,		r5 ; Immediate (11) used => r3 is used and is -1


			nop
			nop


			mov.w #0x2006, r1
			mov.w @SP, r4
			push #0xFC47
			push #0xD936
			push #0x22FA

			mov.w #0x34A3, r5
L1:			rra r5
			jc L1	; Carry is the LSB of the  value. JC tests the carry. 0 to skip
			add.w &0x2002, r5



			nop
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
            

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

Main:		bis.b		#BIT0, &P1DIR		; P1.0 sortie
			bis.b		#BIT6, &P4DIR		; P4.6 sortie

			bic.b		#BIT1, &P1DIR		; P1.1 en entrée (par défaut)
			bis.b		#BIT1, &P1REN		; P1.1 pull-up/down enable
			bis.b		#BIT1, &P1OUT		; P1.1 pull-up

			bic.b		#BIT5, &P4DIR		; P4.5 en entrée (par défaut)
			bis.b		#BIT5, &P4REN		; P4.5 pull-up/down enable
			bis.b		#BIT5, &P4OUT		; P4.5 pull-up

			bic.w		#BIT0, &PM5CTL0		; Unlock IOs
                                            

Loop:										; Lecture bouton 1
S1Check:	mov.b	&P4IN, r4				;
			and.b	#BIT5, r4				; And pour isoler le bit
											; Vaut 0 si pressé, sinon 1
			jz 		S1ON					; Jump si S1 pressé
S1OFF:		bic.b	#BIT6, &P4OUT			; Led1 off
			jmp 	S2Check
S1ON:		bis.b	#BIT6, &P4OUT			; Led1 on

S2Check:	mov.b	&P1IN, r4				;
			and.b	#BIT1, r4				; And pour isoler le bit
											; Vaut 0 si pressé, sinon 1
			jz 		S2ON					; Jump si S1 pressé
S2OFF:		bic.b	#BIT0, &P1OUT			; Led1 off
			jmp 	Loop
S2ON:		bis.b	#BIT0, &P1OUT			; Led1 on
			jmp 	Loop					; Restart loop

EOF:		jmp EOF							; Boucle de fin de programme
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
            

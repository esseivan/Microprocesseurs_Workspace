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

;-------------------------------------------------------------------------------
; Format I: Double operandes exemples
;-------------------------------------------------------------------------------
Loop:
			mov.w   #8, r5	; copie src -> dest
			; copie la valeur immédiate 8 dans r5
			and.w   #0, r6	; AND bit-à-bit entre src et dest -> dest
			; r6 = 0 AND r6 => r6 = 0
			add.w   #3, r6	; additionne src et dest -> dest
			; r6 = r6 + 3 => r6 = 3
			sub.w   r6, r5	; soustrait src de dest -> dest
			; r5 = r5 – r6 => r5 = 8 – 3 = 5
			; Génère un carry car -3 = 0bFFFD et 8 = 0x0008 => 0x0008 + 0xFFFD génère un carry
			xor.w   r6, r5	; XOR bit-à-bit source et dest -> dest
			; r5 = r6 XOR r5 => r5 = 0011b XOR 0101b = 0110b = 6
			bic.w   #3, r5	; mets à zéro les bits src dans dest
			; dest = NOT(src) AND dest
			; r5 = NOT(0011b) AND 0110b = 0100b = 4
			bis.w   #8, r5	; mets à 1 les bits src dans dest
			; dest = src OR dest
			; r5 = 1000b OR 0100b = 1100b = 12
			bit.w   #8, r5	; test les bits src dans dest, affecte le flag Z du SR
			; Z = 1 si (dest AND src) = 0
			; 1000b AND 1100b = 1000b => Z = 0
			cmp.w   r6, r5	; compare src avec dest
			; Assigne les flags avec le résultat de dest – src
			; r5 – r6 = 12 – 3 = 9 supérieur à 0 (donc r5 > r6)
			nop

;-------------------------------------------------------------------------------
; Format II: Simple operande exemples
;-------------------------------------------------------------------------------
            bic.w   #107h, SR
            ; clear V, Z, N, C glags ins SR register
            mov.w   #0xF009,r5
 			; r5 contient la valeur 0xF009 ( 1111'0000'0000'1001b )

			rra.w   r5	 ; roll dest à droite et copie LSB dans le flag C (carry)
		 	; 1111'0000'0000'1001b -> 1111'1000'0000'0100b et C = 1

			rrc.w   r5	 ; roll dest à droite à travers le flag C
		 	; 1111'1000'0000'0100b, C = 1 -> 1111'1100'0000'0010b et C = 0

			swpb    r5  ; échange les bytes dest (swap)
			; 1111'1100'0000'0010b -> 0000'0010'1111'1100b

			sxt	    r5  ; étend le bit de signe du byte dest sur un word
		    ; 0000'0010'1111'1100b , bit 7 = 1 => bit 8 à bit 15 mis à 1
		    ; r5 = 1111'1111'1111'1100b = 0xFFFC
		    nop

;-------------------------------------------------------------------------------
; Format III: Sauts exemples
;-------------------------------------------------------------------------------



            bic.w   #107h, SR
            ; clear V, Z, N, C glags ins SR register
            mov.w   #0xF009,r5
			; r5 contient la valeur 0xF009 ( 1111'0000'0000'1001b )
            mov.w   #0x0002,r6
			; r6 contient la valeur 0x0002 ( 0000'0000'0000'0010b )

			cmp.w   r6, r5	; compare r6 avec r5  (flags = r5-r6)
		 	; SR flags => Z = 0, C = 1, N = 1, V = 0

			jeq     label_1	; saute à label_1 si r5 = r6 (ce qui n'est pas le cas)

			jlo     label_2	; saute à label_2 si r5 < r6 (nombres non-signés)
			; (ce qui n'est pas le cas)

			jl	    label_3	; saute à label_3 si r5 < r6 (nombres signés)
			; effectue le saut car 0xF009 représente -4087 < r6
			; la prochaine instruction exécutée sera celle de label_3
            nop

label_1: 	nop
label_2:    nop
label_3:    nop

            jmp Loop		; jump to current location '$'
            nop				;


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
            
            .end


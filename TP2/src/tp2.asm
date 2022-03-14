; ****************************************************************************
; * @file    Micro-TP2_template.asm
; * @author  The Ghost...
; * @date    dd.mm.yyyy
; * @brief   TP2 - Arithm�tique et assembleur
; *
; ****************************************************************************/
;-------------------------------------------------------------------------------

        .cdecls C,LIST,"msp430.h"       ; Include device header file
        .global RESET

;-------------------------------------------------------------------------------
        .text                           ; Assemble into program memory
        .retain                         ; Override ELF conditional linking
                                        ; and retain current section
        .retainrefs                     ; Additionally retain any sections
                                        ; that have references to current
                                        ; section
;-------------------------------------------------------------------------------

RESET:   mov.w   #__STACK_END,SP        ; Initialize stackpointer
StopWDT: mov.w   #WDTPW|WDTHOLD,&WDTCTL ; Stop watchdog timer

; d�but du programme pour le TP2
loop:
; - TP2: 4.2 Move
        mov.b  #55h, R5                  ; Move byte
        mov.w  #0xAA55, R10              ; Move word

; - TP2: 4.3 Addition
        mov.b  #11h, R6                  ; byte sans d�passement
        add.b  R5, R6                    ;
        addc.b R6, R6                    ;
        addc.b R6, R6                    ; d�passement...
        addc.b R6, R6                    ; byte avec d�passement
        nop                             ; to show result before next instr.

        ; a) word sans d�passement

        ; b) word avec d�passement

; - TP2: 4.4 Soustraction
        ; a) byte sans d�passement et sans r�port

        ; b) byte sans d�passement, avec r�port

        ; c) word sans d�passement  et sans r�port

        ; d) word sans d�passement, avec r�port

; - TP2: 4.5 Comparaison

; - TP2: 4.6 Test

; - TP2: 4.7 Instr. Logiques

; - TP2: 4.8 Additions/Soustractions sign� 16bits
        ; a) Addition

        ; b) Soustraction

        ; c) Limitation de l'addition au valeur max

        ; d) Limitation la soustraction au min value

        ; e) Op�rations sur 32bits




; fin du programme pour le TP2

        jmp loop                        ; jump to current location '$'
                                        ; (endless loop)
        nop                             ; necessary for MSP430FR5969


;-------------------------------------------------------------------------------
;           Stack Pointer definition
;-------------------------------------------------------------------------------
        .global __STACK_END
        .sect 	.stack

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
        .sect   ".reset"                ; MSP430 RESET Vector
        .short  RESET
        .end

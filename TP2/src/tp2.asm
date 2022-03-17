; ****************************************************************************
; * @file    tp2.asm
; * @author  Esseiva Nicolas
; * @date    16.03.2022
; * @brief   TP2 - Arithmétique et assembleur
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

; début du programme pour le TP2
loop:
; - TP2: 4.2 Move
        mov.b  #55h, R5                  ; Move byte
        mov.w  #0x5555, R10              ; Move word

; - TP2: 4.3 Addition
        mov.b  #11h, R6                  ; byte sans dépassement
        add.b  R5, R6                    ;
        addc.b R6, R6                    ;
        addc.b R6, R6                    ; dépassement...
        addc.b R6, R6                    ; byte avec dépassement
        nop                              ; to show result before next instr.
        								 ; r6 = 0x31

		; Partie word : 16bits
        ; a) word sans dépassement
		mov.w	#0x1111,	r11
		add.w	r10,		r11			; word sans dépassement, ni carry
        nop

        ; b) word avec dépassement
		add.w	r10,		r11			; dépassement
		addc.w	r10,		r11			; carry
		addc.w	#0xF700,	r11			; carry
        nop

; - TP2: 4.4 Soustraction
        ; a) byte sans dépassement et sans réport
        mov.b	#0x22,		r6			;
        sub.b	r5,			r6			; 0x22 - 0x55 :
		nop

        ; b) byte sans dépassement, avec réport
        sub.b	#0x22,		r6			;
		nop

        ; c) word sans dépassement  et sans réport
        mov.w	#0x7777,	r11			;
        clrc
        subc.w	#0x2222,	r11			;
		nop

        ; d) word sans dépassement, avec réport
        subc.w	#0x2222,	r11			;
		nop

; - TP2: 4.5 Comparaison
		mov.w	#30,	r6
		cmp.w	#25, 	r6				; r6 > 25
		cmp.w	#35,	r6				; r6 < 35
		cmp.w	#30,	r6				; r6 = 30
		nop

; - TP2: 4.6 Test
		tst.w	r6						; r6 > 0
		tst.w	r3						; r3 = 0 = 0
		mov.w	#-1,	r6
		tst.w	r6						; -1 < 0
		nop

; - TP2: 4.7 Instr. Logiques
		mov.b	#0xA5,		r6
		mov.w	#0xAB56,	r11
		and.b	#0xF0,		r6			; mettre à 0 les 4 derniers bits
		bic.b	#0xF0,		r6			; mettre à 0 les 4 premiers bits
		xor.w	#0xFFFF,	r11			; Inverser tous les bits
		bis.w	#0xF000,	r11			; mettre à 1 les 4 premiers bits
		mov.w	#0xFFEE,	r11
		bit.w	#0x8000,	r11			; Tester le bit 15
		nop

; - TP2: 4.8 Additions/Soustractions signé 16bits
        ; a) Addition
		mov.w	#0x5555,	r11
		mov.w	#0x7000,	r10
		add.w	r10,		r11			; Dépassement
		nop

        ; b) Soustraction
		mov.w	#0xC555,	r11
		mov.w	#0x7000,	r10
        sub.w	r10,		r11			; Dépassement
		nop

        ; c) Limitation de l'addition au valeur max

		mov.w	#0x5555,	r11
		mov.w	#0x7000,	r10
		add.w	r10,		r11			; Dépassement
		; Verification du dépassement. Pour cela on test le flag V avec BIT
		; Ensuite on effectue un jump si zero (V = 0) pour eviter le code suivant
		bit.w	#0x0100,	SR			; Vérification de depassement (V)
		jz		Skip1					; Jump if V = 0 (pas de depassement)
		; On effectue cette instruction s'il y a depassement
		mov.w	#0xFFFF,	r11
Skip1:	nop
		nop

        ; d) Limitation la soustraction au min value
		mov.w	#0xC555,	r11
		mov.w	#0x7000,	r10
        sub.w	r10,		r11			; Dépassement
        bit.w	#0x0100,	SR
        jz		Skip2
        mov.w	#0xFFFF,	r11
Skip2:	nop
		nop

        ; e) Opérations sur 32bits
		; 65'539 + 65'540 = 131'079 = 0x20007
		; A = 65'539 = 0x10003
		; B = 65'540 = 0x10004
		mov.w	#0x0003, r4				; A_L : Input A, Less Significant Word
		mov.w	#0x0001, r5				; A_H : Input A, Most Significant Word
		mov.w	#0x0004, r6				; B_L : Input B, Less Significant Word
		mov.w	#0x0001, r7				; B_H : Input B, Most Significant Word

		; A + B : On peut décomposer en addition de LSB et MSB, comme pour les adders.
		add.w	r4, r6					; Z_L = A_L + B_L
		addc.w	r5, r7					; Z_H = A_H + B_H + Carry
		nop

		; 3 - 65'544 = 65'541 = 0x10005
		; A = 3 = 0x3
		; B = 65'544 = 0x10008
		mov.w	#0x0003, r4				; A_L : Input A, Less Significant Word
		mov.w	#0x0000, r5				; A_H : Input A, Most Significant Word
		mov.w	#0x0008, r8				; B_L : Input B, Less Significant Word
		mov.w	#0x0001, r9				; B_H : Input B, Most Significant Word

		; A - B :
		sub.w	r4, r8					; Z_L = A_L - B_L
		subc.w	r5, r9					; Z_H = A_H + B_H + Carry
		nop

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

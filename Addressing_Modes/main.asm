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

;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| As | Ad | Addressing Mode        | Syntax | Description                                                                                                                                                                                             |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 00 | 0  | Register               | Rn     | Register contents are operand.                                                                                                                                                                          |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 01 | 1  | Indexed                | X(Rn)  | (Rn + X) points to the operand. X is stored in the next word, or stored in combination of the preceding extension word and the next word.                                                               |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 01 | 1  | Symbolic               | ADDR   | (PC + X) points to the operand. X is stored in the next word, or stored in combination of the preceding extension word and the next word. Indexed mode X(PC) is used.                                   |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 01 | 1  | Absolute               | &ADDR  | The word following the instruction contains the absolute address. X is stored in the next word, or stored in combination of the preceding extension word and the next word. Indexed mode X(SR) is used. |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 10 | -  | Indirect Register      | @Rn    | Rn is used as a pointer to the operand.                                                                                                                                                                 |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 11 | -  | Indirect Autoincrement | @Rn+   | Rn is used as a pointer to the operand. Rn is incremented afterwards by 1 for .B instructions, by 2 for .W instructions, and by 4 for .A instructions.                                                  |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
;| 11 | -  | Immediate              | #N     | N is stored in the next word, or stored in combination of the preceding extension word and the next word. Indirect autoincrement mode @PC+ is used.                                                     |
;+----+----+------------------------+--------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

Loop:

;-------------------------------------------------------------------------------
; Mode imm�diat:  (#n)
;-------------------------------------------------------------------------------
			;mov.w LoopCtr, r6
			mov.w   #100, r5	 ; move constant word 100 to r6

;-------------------------------------------------------------------------------
; Mode par registre: (Rn)
;-------------------------------------------------------------------------------
			mov.w r5,r6 	     ; move word from r5 to r6

;-------------------------------------------------------------------------------
;  Mode par registre index�:  x(Rn)
;-------------------------------------------------------------------------------
			mov.b 5(r5),r6     ; load byte from address 3+(R5)=7 into R6

;-------------------------------------------------------------------------------
; Mode symbolique:  x(PC)
;-------------------------------------------------------------------------------
			mov.w     Loop,r6      ; move word from memory adresse Loop to r6

;-------------------------------------------------------------------------------
; Mode absolu:  (&Label)
;-------------------------------------------------------------------------------
			mov.w     &Loop,r7      ; move word from memory adresse Loop to r7

;-------------------------------------------------------------------------------
; Mode registre indirect:  (@Rn)
;-------------------------------------------------------------------------------
			mov.w     @r5,r6      ; move word from memory address in r5 to r6

;-------------------------------------------------------------------------------
; Mode registre indirect auto-incr�ment�:  (@Rn+)
;-------------------------------------------------------------------------------
			mov.w     @r5+,r8      ; move word from memory address in r5 to r8 	and after increment r5 by 2


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
            

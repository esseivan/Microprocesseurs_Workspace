/*
 * exo.asm
 *
 *  Created on: 05 Mar 2022
 *      Author: nicolas
 */


;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  

            .text                           
            .retain                         
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  
                                            
            mov.w   #0x1234,&MPY32L         
            mov.w   #0x1234,&MPY32H         

            mov.w   #0x5678,&OP2L           
            mov.w   #0x5678,&OP2H           
                                            
            mov.w   #0x1234,&MACS32L        
            mov.w   #0x1234,&MACS32H        

            mov.w   #0x5678,&OP2L           
            mov.w   #0x5678,&OP2H           

            mov.w   #25,R15
Delay       dec.w   R15
            jnz     Delay

            bis.w   #LPM0,SR               
            nop                            

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end

#include <msp430.h>

;-------------------------------------------------------------------------------
        .section .text,"ax",@progbits
;-------------------------------------------------------------------------------
_start:
        ;; disable watchdog and set stack to highest RAM addr
        mov.w   #__stack,SP                     ; gcc ldscripts compute __stack based on mmcu
        mov.w   #WDTPW|WDTHOLD,&WDTCTL
        jmp _libmsp430fr5994plat_entry

        ;; initialize clock,gpio,timer


;------------------------------------------------------------------------------
; UNEXPECTED_ISR - default handler for unhandled interrupt
;-------------------------------------------------------------------------------
UNEXPECTED_ISR:
        reti                    		; cycles: 5

;------------------------------------------------------------------------------
; Interrupt Vectors - see the datasheet for your chip
;                    *msp430g2553 vectors described below
;------------------------------------------------------------------------------

.section ".resetvec","ax",@progbits
.word _start            ;0xfffe (RESET_VECTOR)
.end

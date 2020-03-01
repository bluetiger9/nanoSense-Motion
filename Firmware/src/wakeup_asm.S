/* ----------------------------------------------------------------------------
 * Copyright (c) 2017 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * wakeup_asm.S
 * - Initialize stack pointer and continue execution
 * ----------------------------------------------------------------------------
 * $Revision: 1.1 $
 * $Date: 2017/08/11 14:38:48 $
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Place this code in the usual .text segment for code
 * ------------------------------------------------------------------------- */
    .thumb_func

    .section .app_wakeup_asm
    .globl Wakeup_From_Sleep_Application_asm
    .extern __stack, 4

/* ----------------------------------------------------------------------------
 * Function      : void Wakeup_From_Sleep_Application_asm()
 * ----------------------------------------------------------------------------
 * Description   : Initialize stack pointer and continue execution
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
Wakeup_From_Sleep_Application_asm:
    movw r0, #:lower16:__stack
    movt r0, #:upper16:__stack
    msr msp, r0

/* ----------------------------------------------------------------------------
 * File end
 * ------------------------------------------------------------------------- */
    .end

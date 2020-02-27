//-----------------------------------------------------------------------------
// Copyright (c) 2018 Semiconductor Components Industries LLC
// (d/b/a "ON Semiconductor").  All rights reserved.
// This software and/or documentation is licensed by ON Semiconductor under
// limited terms and conditions.  The terms and conditions pertaining to the
// software and/or documentation are available at
// http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf ("ON Semiconductor Standard
// Terms and Conditions of Sale, Section 8 Software") and if applicable the
// software license agreement.  Do not use this software and/or documentation
// unless you have carefully read and you agree to the limited terms and
// conditions.  By using this software and/or documentation, you agree to the
// limited terms and conditions.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#ifndef APP_SLEEP_H_
#define APP_SLEEP_H_

#include <stdbool.h>
#include <stdint.h>

#define RTC_CLK_SRC                     RTC_CLK_SRC_XTAL32K

/* XTAL32K ITRIM value set point */
#define XTAL32K_ITRIM_VALUE             0xF

/* XTAL32K ITRIM value set point */
#define XTAL32K_CLOAD_TRIM_VALUE        0x38

/* DMA channel used to save/restore RF registers in each sleep/wake-up cycle */
#define DMA_CHAN_SLP_WK_RF_REGS_COPY    1

extern void Sleep_Mode_Configure(struct sleep_mode_env_tag *sleep_env);

extern void Wakeup_From_Sleep_Application_asm(void);

extern void Wakeup_From_Sleep_Application(void) __attribute__ ((section(".app_wakeup")));

extern void Main_Loop(void);


#endif /* APP_SLEEP_H_ */

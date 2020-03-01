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
//! \file EventCallback.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "BDK.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void BDK_InitializeFreq(enum HAL_ClockConfiguration clk_conf)
{
    HAL_Init(clk_conf);

    BDK_TaskInit();

    Kernel_Init(0);
    ke_timer_init();
}

void BDK_Schedule(void)
{
    BDK_TaskStart();

    Kernel_Schedule();

    Sys_Watchdog_Refresh();
}

void BDK_ScheduleNoWdt(void)
{
    BDK_TaskStart();

    Kernel_Schedule();
}

//! \}

#ifndef RTE_BDK_LIBRARIES_OUTPUT_REDIRECTION

int __attribute__((weak)) _read(int file, char* ptr, int len)
{
    return -1;
}

int __attribute__((weak)) _write(int file, char* ptr, int len)
{
    return -1;
}

#endif /* RTE_BDK_LIBRARIES_OUTPUT_REDIRECTION */

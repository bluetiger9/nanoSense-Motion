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

#include "app_trace.h"

#include <rsl10.h>
#include "HAL_UART.h"

void trace_init(void)
{
    HAL_UART_Init();
    HAL_UART_SetBaudRate(230400);
}

void trace_deinit(void)
{
    HAL_UART_DeInit();
}

void assert_error(const char *file, const int line, const char *msg)
{
    TRACE_PRINTF("%s: %d: %s\r\n", file, line, msg);

    __disable_irq();
    while (1)
    {
        Sys_Watchdog_Refresh();
    }
}

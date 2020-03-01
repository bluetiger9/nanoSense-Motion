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
//! \file HAL_error.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <HAL.h>
#include <LedNames.h>
#include <app_trace.h>

#if defined RTE_DEVICE_BDK_OUTPUT_REDIRECTION && RTE_DEVICE_BDK_OUTPUT_REDIRECTION == SEGGER_RTT
#include <stdio.h>
#endif


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

void HAL_Failed(const char* f, int l, const char* e)
{
	// Configure LED DIO pin
	Sys_DIO_Config(LED_FAILURE, DIO_MODE_GPIO_OUT_1);

	while (1)
	{
		TRACE_PRINTF("%s: %d: %s\r\n", f, l, e);

		Sys_GPIO_Set_High(LED_FAILURE);
		Sys_Delay_ProgramROM(SystemCoreClock);

		Sys_GPIO_Set_Low(LED_FAILURE);
		Sys_Delay_ProgramROM(SystemCoreClock * 2);
	}
}

//! }
//! }

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
//! \file led_api.c
//!
//! Implementation of LED API for BDK-GEVK evaluation board.
//!
//-----------------------------------------------------------------------------

#include <BDK.h>
#include <api/led_api.h>

void LED_Initialize(LedName led)
{
    Sys_DIO_Config(led, DIO_6X_DRIVE | DIO_NO_PULL | DIO_MODE_GPIO_OUT_0);
}

void LED_On(LedName led)
{
    Sys_GPIO_Set_High(led);
}

void LED_Off(LedName led)
{
    Sys_GPIO_Set_Low(led);
}

void LED_Toggle(LedName led)
{
    Sys_GPIO_Toggle(led);
}

bool LED_IsOn(LedName led)
{
    return (DIO->CFG[led] & 0x1) == 1;
}

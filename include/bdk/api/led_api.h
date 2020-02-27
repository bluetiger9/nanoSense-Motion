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
//! \file led_api.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup API
//! \{
//! \addtogroup LED_API LED
//!
//! \brief API for the controlling of on-board LEDs.
//!
//! This API is implemented by the Board Support Package (BSP) to provide basic
//! functionality for all on-board LEDs.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef LED_API_H
#define LED_API_H

#include <stdbool.h>
#include <stdint.h>

// Board specific LED names provided by evaluation board BSP
#include <LedNames.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Initializes the DIO pad for given LED. */
extern void LED_Initialize(LedName led);

/** \brief Turns on given LED. */
extern void LED_On(LedName led);

/** \brief Turns off given LED. */
extern void LED_Off(LedName led);

/** \brief Toggles selected LED */
extern void LED_Toggle(LedName led);

/** \brief Returns true if given LED is ON */
extern bool LED_IsOn(LedName led);

#ifdef __cplusplus
}
#endif

#endif /* LED_API_H */

//! \}
//! \}
//! \}

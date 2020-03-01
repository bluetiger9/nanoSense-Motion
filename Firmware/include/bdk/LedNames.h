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
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup TGT_GRP Target
//! \{
//! \addtogroup TGT_RSL10_SENSE_GRP
//! \{
//!----------------------------------------------------------------------------

#ifndef LED_NAMES_H_
#define LED_NAMES_H_

#include <PinNames.h>

/** LED names accepted by LED API when HB-GEVB board is used. */
typedef enum {
    LED0 = PIN_LED_RED,
	LED1 = PIN_LED_GREEN,
	LED2 = PIN_LED_BLUE,

    /* Aliases. */
	LED_RED = LED0,
    LED_GREEN = LED1,
	LED_BLUE = LED2,

    LED_FAILURE = LED0 /**< LED to use by HAL_Failure function when assertion fails. Alias to \ref LED0 */
} LedName;

#endif /* LED_NAMES_H_ */

//! \}
//! \}
//! \}

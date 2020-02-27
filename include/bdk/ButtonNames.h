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

#ifndef BUTTON_NAMES_H_
#define BUTTON_NAMES_H_

#include <PinNames.h>

/** Button names accepted by Button API when BDK-GEVB board is used. */
typedef enum {
    BTN0 = PIN_BUTTON0,
	BTN1 = PIN_BUTTON1,

    /* Aliases */
    BUTTON0 = BTN0, /**< Alias for \ref BTN0 */
	BUTTON1 = BTN1, /**< Alias for \ref BTN1 */
} ButtonName;

#endif /* BUTTON_NAMES_H_ */

//! \}
//! \}
//! \}

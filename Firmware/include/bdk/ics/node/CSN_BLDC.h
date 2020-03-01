// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------

#ifndef ICS_NODE_BLDC_H_
#define ICS_NODE_BLDC_H_

#include "ics/CS.h"
#include "stdbool.h"


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

extern bool CSN_BLDC_CheckAvailability();

extern bool CSN_BLDC_IsAvailable();

extern struct CS_Node_Struct* CSN_BLDC_Create();

#endif /* ICS_NODE_BLDC_H_ */

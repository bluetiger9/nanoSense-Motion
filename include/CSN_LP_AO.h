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

#ifndef ICS_NODE_LP_AO_H_
#define ICS_NODE_LP_AO_H_

#include "ics/CS.h"
#include <stdbool.h>

#include "RTE_app_config.h"

// Number of samples per second for enabled sensors
#define CSN_LP_AO_SAMPLE_RATE          RTE_APP_ICS_AO_REPORT_RATE

//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

extern struct CS_Node_Struct* CSN_LP_AO_Create(void);

#endif /* ICS_NODE_LP_AO_H_ */

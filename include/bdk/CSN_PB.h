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
//!
//! This is an example for implementing a Custom Service Node.
//!
//! This node can report to the client whether on-board push buttons are
//! currently pressed / released and also if it was pressed since last status
//! request.
//!
//! This node has single property called \b S which reports all button states.
//!
//! Below example requests state of on-board buttons.
//! The reply is formatted as string with two button states separated by comma.
//! First character of button state contains button number, second current
//! button status and third if it was pressed since last PB/S request.
//! \code
//! >> d/PB/S
//! << d/s/0P_,1RI
//! \endcode
//!
//! Possible button state characters:
//! * 'P' - Button is pressed right now.
//! * 'R' - Button is released right now.
//! * 'I' - Button was pressed at least once since last PB/S request.
//! * '_' - Button was not pressed since last PB/S request.
//!
// ----------------------------------------------------------------------------
#ifndef ICS_NODE_PB_H_
#define ICS_NODE_PB_H_

#include <ics/CS.h>
#include <stdbool.h>


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

extern bool CSN_PB_CheckAvailability();

extern bool CSN_PB_IsAvailable();

extern struct CS_Node_Struct* CSN_PB_Create();

#endif /* ICS_NODE_PB_H_ */

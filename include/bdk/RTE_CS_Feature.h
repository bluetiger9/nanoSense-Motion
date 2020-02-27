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

#ifndef _CS_FEATURES_H_
#define _CS_FEATURES_H_


//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

// Maximum number of active nodes including SYS node.
#define CS_MAX_NODE_COUNT ((int)16)

// Maximum length of text page of TEXT datatype property
// 20b - 2b - 2b = 16b
// 20b -> Packet length
// -2b -> Token
// -2b -> Response datatype t/
#define CS_TEXT_PAGE_LEN ((int)16)

// Enable Logging levels
#ifndef APP_TRACE_DISABLED
#define CS_LOG_ERROR_ENABLE 1
#define CS_LOG_WARNING_ENABLE 1
#define CS_LOG_INFO_ENABLE 1
#define CS_LOG_VERBOSE_ENABLE 1
#endif

// Enable coloured output in RTT Terminal
#define CS_LOG_WITH_ANSI_COLORS 0

#endif /* _CS_FEATURES_H_ */

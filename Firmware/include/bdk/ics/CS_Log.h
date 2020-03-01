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

#ifndef _CS_LOG_H_
#define _CS_LOG_H_


//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "RTE_CS_Feature.h"


#ifdef __cplusplus
extern "C" {
#endif


//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

// Prototype for Node error logging macros
#ifdef CS_LOG_ERROR_ENABLE
#define CS_LogError(node, ...) CS_Log(CS_LOG_LEVEL_ERROR, node, __VA_ARGS__)
#else
#define CS_LogError(node, ...)
#endif /* CS_LOG_ERROR_ENABLE */

// Prototype for Node warning logging macros
#ifdef CS_LOG_WARNING_ENABLE
#define CS_LogWarning(node, ...) CS_Log(CS_LOG_LEVEL_WARNING, node, __VA_ARGS__)
#else
#define CS_LogWarning(node, ...)
#endif /* CS_LOG_WARNING_ENABLE */

// Prototype for Node info logging macros
#ifdef CS_LOG_INFO_ENABLE
#define CS_LogInfo(node, ...) CS_Log(CS_LOG_LEVEL_INFO, node, __VA_ARGS__)
#else
#define CS_LogInfo(node, ...)
#endif /* CS_LOG_INFO_ENABLE */

// Prototype for Node verbose logging macros
#ifdef CS_LOG_VERBOSE_ENABLE
#define CS_LogVerbose(node, ...) CS_Log(CS_LOG_LEVEL_VERBOSE, node, __VA_ARGS__)
#else
#define CS_LogVerbose(node, ...)
#endif /* CS_LOG_INFO_ENABLE */


// Shortcuts for logging of CS SYS messages.
#define CS_SYS_Error(...) CS_LogError("SYS", __VA_ARGS__)
#define CS_SYS_Warn(...) CS_LogWarning("SYS", __VA_ARGS__)
#define CS_SYS_Info(...) CS_LogInfo("SYS", __VA_ARGS__)
#define CS_SYS_Verbose(...) CS_LogVerbose("SYS", __VA_ARGS__)


//-----------------------------------------------------------------------------
// EXPORTED DATA TYPES DEFINITION
//-----------------------------------------------------------------------------

enum CS_Log_Level
{
	CS_LOG_LEVEL_ERROR = 0,
	CS_LOG_LEVEL_WARNING = 1,
	CS_LOG_LEVEL_INFO = 2,
	CS_LOG_LEVEL_VERBOSE = 3
};


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

/* \brief Internal logging function. */
extern void CS_Log(enum CS_Log_Level level, const char* module, const char* fmt, ...);


#ifdef __cplusplus
}
#endif


#endif /* _CS_LOG_H_ */

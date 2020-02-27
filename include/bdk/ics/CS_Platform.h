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

#ifndef _CS_PLATFORM_H_
#define _CS_PLATFORM_H_


//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


//-----------------------------------------------------------------------------
// FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

struct CS_Handle_Struct;


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

/** \brief Platform specific initialization implementation. */
extern int CS_PlatformInit();

/** \brief Platform specific callback for sending of data over BLE.
 *
 * \param tx_data_buf
 * Data buffer which will be sent over BLE.
 * \param tx_data_buf_len
 * Number of bytes to be sent. Max 20 bytes are allowed.
 * \returns
 * 0 on success.
 * -1 on failure.
 */
extern int CS_PlatformWrite(const char* tx_data_buf, int tx_data_buf_len);

/** \brief Returns current platform time in milliseconds. */
extern uint32_t CS_PlatformTime();

/** \brief Provides printf like functionality for logging purposes. */
extern void CS_PlatformLogPrintf(const char* fmt, ...);

/** \brief Provides vprintf like functionality for logging purposes. */
extern void CS_PlatformLogVprintf(const char* fmt, va_list args);

/** \brief Locks logging stream.
 *
 * Is only needed if platform shares logging resources.
 */
extern void CS_PlatformLogLock();

extern void CS_PlatformLogUnlock();


#ifdef __cplusplus
}
#endif


#endif /* _CS_PLATFORM_H_ */

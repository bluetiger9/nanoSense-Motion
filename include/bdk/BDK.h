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
//! \file BDK.h
//! \version v1.0.0
//!
//! Main include file for the Bluetooth LE Development kit software package
//! for RSL10.
//!
//! \addtogroup BDK_GRP BDK
//!
//! \brief Abstraction layers for RSL10 Bluetooth Development Kit based
//! applications.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BDK_H_
#define BDK_H_

#include <stdbool.h>
#include <stdint.h>

#include <HAL.h>
#include "BDK_Task.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Initialize clock system of RSL10 and low level HAL layer.
 *
 * This should be the first function called from an BDK application.
 *
 * This function utilizes sleep mode re-flash functionality.
 * Program execution will be halted, preventing entering of sleep mode, as long
 * as DIO pad 12 (\ref HAL_PIN_RECOVERY) is held low.
 *
 * \param clk_conf
 * Device clock configuration to be used for this application.
 * Use one of HAL_CLK_CONF_[ 8MHZ | 16MHZ | 24MHZ | 48MHZ ]
 *
 * \see \ref CLK for details about available clock configurations.
 */
extern void BDK_InitializeFreq(enum HAL_ClockConfiguration clk_conf);

/** \brief Initialize clock system of RSL10 and low level HAL layer.
 *
 * This should be the first function called from an BDK application.
 *
 * This function utilizes sleep mode re-flash functionality.
 * Program execution will be halted, preventing entering of sleep mode, as long
 * as DIO pad 12 (\ref PIN_RECOVERY) is held low.
 *
 * \see BDK_InitializeFreq for equivalent function that allows for different
 * system clock configurations.
 */
__STATIC_INLINE void BDK_Initialize(void)
{
    BDK_InitializeFreq(RTE_HAL_CLK_CONF);
}

/** \brief Schedule Event Kernel events and refresh Watchdog timer in main loop
 * of an BDK application.
 *
 * This function should be called from main loop of an BDK application.
 *
 * Equivalent to calling following functions:
 * \code
 *     BDK_TaskStart();
 *     Kernel_Schedule();
 *     Sys_Watchdog_Refresh();
 * \endcode
 */
extern void BDK_Schedule(void);

/** \brief Schedule Event Kernel events in main loop of an BDK application.
 *
 * This function should be called from main loop of an BDK application.
 * Alternative to BDK_Schedule if application wants control over Watchdog
 * refresh.
 *
 * Equivalent to calling following functions:
 * \code
 *     BDK_TaskStart();
 *     Kernel_Schedule();
 * \endcode
 */
extern void BDK_ScheduleNoWdt(void);


#ifdef __cplusplus
}
#endif

//! \}

#endif /* BDK_H_ */

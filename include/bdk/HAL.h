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
//! \file HAL.h
//!
//! Main include file for the Hardware Abstraction Layer for RSL10 used by
//! BDK applications.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP HAL
//!
//! \brief Peripheral Hardware Abstraction Layer for RSL10.
//!
//! This group of peripheral driver libraries is intended to provide unified
//! interface for all applications build on top of BDK software libraries.
//! Main target for HAL libraries are Arduino / PMOD shield libraries which need
//! to communicate with external Integrated Circuits or need GPIO or PWM
//! signals.
//!
//! HAL libraries for I2C, SPI and UART are utilizing CMSIS Driver compatible
//! driver libraries provided as part of RSL10 SDK.
//! The HAL wrappers for CMSIS drivers provide both synchronous and asynchronous
//! operation calls to meet wide range of use cases.
//! The HAL libraries are <b>not thread safe</b> as the underlying Event Kernel
//! of RSL10 does not support true multi-threading.
//!
//! HAL for other peripherals use the simple System library of RSL10.
//!
//! A HAL wrapper for internal clock configuration is available and provides
//! four different system clock configurations that can be used based on
//! applications processing requirements.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef HAL_H_
#define HAL_H_

/* Load RTE configuration before anything else.
 * This header file has to be found in the global search path.
 */
#ifdef _RTE_
#include "RTE_Components.h"
#include "RTE_BDK.h"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <PinNames.h>

#include "HAL_clock.h"
#include "HAL_error.h"
#include "HAL_I2C.h"
//#include "HAL_SPI.h"
#include "HAL_UART.h"

#include <rsl10.h> /* Cannot be included before RTE_Components.h */
#include <rsl10_ke.h> /* Cannot be included before RTE_Components.h */

#ifdef __cplusplus
extern "C"
{
#endif

#define HAL_TIME_RESOLUTION_US          (1000)
//#define HAL_TICKER_RESOLUTION_US        (1)
//#define HAL_KERNEL_TICKER_RESOLUTION_US (10000)

/** Correct way to compare time stamps that prevents counter overflow issues. */
#define HAL_TIME_ELAPSED_SINCE(start_timestamp) (HAL_Time() - start_timestamp)

/** \brief Initializes system clocks and starts platform timer.
 *
 * This function should not be called directly as it is called from
 * \ref BDK_Initialize.
 *
 * \warning
 * Calling any HAL function before HAL_Init will result in undefined behavior.
 *
 * \param clk_conf
 * Desired clock configuration for this application.
 * Use one of HAL_CLK_CONF_[ 8MHZ | 16MHZ | 24MHZ | 48MHZ ].
 *
 * \see \ref CLK for details about available clock configurations.
 */
extern void HAL_Init(enum HAL_ClockConfiguration clk_conf);

extern void HAL_TICK_Init(void);

/** \brief Delays program execution for defined period of time.
 *
 * This function is meant to be used for short delays.
 *
 * Platform time uses 32 bit counter incremented 1000x per second.
 * This means that the counter will overflow approximately every 49 days.
 *
 * \returns Current platform time in ms.
 */
extern uint32_t HAL_Time(void);

/** \brief Delays program execution for given amount of milliseconds.
 *
 * CPU is entering sleep mode for the duration of delay.
 *
 * \pre This function cannot be called from an interrupt service routine.
 *
 * \warning
 * Watchdog timeout has to be considered when delaying program execution.
 * Watchdog is always active with the only exception being when debugger is
 * active.
 *
 * \param ms Number of milliseconds to delay for.
 */
extern void HAL_Delay(const uint32_t ms);

/** \brief Detects if this function was called from within an ISR.
 *
 * \returns true if this function was called from an interrupt service routine.
 * \returns false otherwise.
 */
extern bool HAL_IsInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_H_ */

//! \}
//! \}

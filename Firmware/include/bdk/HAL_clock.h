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
//! \file HAL_clock.h
//!
//! Defines possible clock configurations that can be used on BDK.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//! \addtogroup CLK_GRP Clock Configurations
//!
//! \brief Defines possible clock configurations for proper operation of BDK.
//!
//! These configuration options provided to allow to select between power
//! efficiency & performance by scaling the system clock frequency for ARM
//! Cortex-M3 core.
//!
//! | Clock Configuration | SYSCLK Source | SYSCLK | SLOWCLK | BBCLK | BBCLK_DIV | DCCLK | CPCLK   | Exact I2C Prescale |
//! | ------------------- | ------------: | -----: | ------: | ----: | --------: | ----: | ------: | -----------------: |
//! | HAL_CLK_CONF_48MHZ  | RFCLK         | 48 MHz | 1 MHz   | 8 MHz | 1 MHz     | 8 MHz | 125 kHz | yes                |
//! | HAL_CLK_CONF_24MHZ  | RFCLK         | 24 MHz | 1 MHz   | 8 MHz | 1 MHz     | 8 MHz | 125 kHz | yes                |
//! | HAL_CLK_CONF_16MHZ  | RFCLK         | 16 MHz | 1 MHz   | 8 MHz | 1 MHz     | 8 MHz | 125 kHz | no                 |
//! | HAL_CLK_CONF_8MHZ   | RFCLK         |  8 MHz | 1 MHz   | 8 MHz | 1 MHz     | 4 MHz | 125 kHz | no                 |
//!
//! The SYSCLK clock frequency can be set either:
//!
//! * <b>Manually</b> - By using \ref BDK_InitializeFreq to initialize system and passing it
//!   desired clock configuration setting.
//! * <b>From RTE Configuration</b> - By using \ref BDK_Initialize the RTE configured mode
//!   will be used.
//!   To change RTE configuration open RTE_BDK.h file in CMSIS Configuration
//!   Wizard editor and set desired clock mode.
//!   RTE configuration is managed on project level.
//!   RTE_BDK.h can be found in the \i RTE/BDK/RTE_BDK.h folder.
//!   \image html rte_bdk.jpg
//!
//!
//! \see Chapter <b>6 Clocking</b> of RSL10 Hardware Reference for more details
//! about clocks.
//!
//! \see Section <b>6.2.2 48 MHz Crystal Oscillator</b> of RSL10 Hardware
//! Reference for integration details of 48 MHz RF oscillator.
//!
//! \see Section <b>6.3.7 Power Supply Clocks</b> of RSL10 Hardware Reference
//! for more information about DCCLK and CPCLK.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef HAL_CLOCK_H_
#define HAL_CLOCK_H_

/** \brief Possible clock configurations supported by all HAL drivers and BLE
 * Baseband.
 */
enum HAL_ClockConfiguration
{
    HAL_CLK_CONF_48MHZ,
    HAL_CLK_CONF_24MHZ,
    HAL_CLK_CONF_16MHZ,
    HAL_CLK_CONF_8MHZ
};

/** \brief Sets clock prescalers to one of predefined configurations.
 */
extern void HAL_SetClockPrescalers(const enum HAL_ClockConfiguration clk);

#endif /* HAL_CLOCK_H_ */

//! \}
//! \}

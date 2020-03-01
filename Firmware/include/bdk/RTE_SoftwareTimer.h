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
//! \file RTE_SoftwareTimer.h
//!
//! This file contains the Run Time Configuration (RTE) options for Software
//! Timer CMSIS Component.
//!
//! These options can be edited by opening this file in CMSIS Configuration
//! Wizard Editor that is part of Eclipse installation.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup SW_TIMER_GRP
//! \{
//-----------------------------------------------------------------------------
#ifndef RTE_SOFTWARE_TIMER_H_
#define RTE_SOFTWARE_TIMER_H_

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Software Timer Configuration

/** \def SW_TIMER_RESOLUTION
 * \brief Defines the smallest period of time that software timer is able to
 * detect.
 *
 * Default value is 100us with maximal resolution of 2us and smallest possible
 * resolution of 33554430us (limited by 24-bit timer register) with step of 2us.
 *
 * \see SwTimer_CTX_Initialize for configuring of timer resolution in program.
 */

//  <o>Timer resolution [us] <2-33554430:2>
//  <i> Defines the smallest period of time that software timer is able to detect.
//  <i> Default: 1000 us
#ifndef SW_TIMER_RESOLUTION
#define RTE_SW_TIMER_RESOLUTION  1000
#endif

//  <o>Hardware Timer Select
//     <0=> TIMER0
//     <1=> TIMER1
//     <2=> TIMER2
//     <3=> TIMER3
//  <i>Which General Purpose Timer of RSL10 to use for Software Timer functionality.
//  <i>Default: TIMER1 (This value will not collide with other BDK components)
#ifndef RTE_SW_TIMER_INSTANCE
#define RTE_SW_TIMER_INSTANCE  1
#endif

// </h>

// <<< end of configuration section >>>

#endif /* RTE_SOFTWARE_TIMER_H_ */

//! \}
//! \}

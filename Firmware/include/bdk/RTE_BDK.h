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
//! \file RTE_BDK.h
//!
//! This file contains the Run Time Configuration (RTE) options for BDK Core
//! CMSIS Component.
//!
//! These options can be edited by opening this file in CMSIS Configuration
//! Wizard Editor that is part of Eclipse installation.
//!
//! \addtogroup BDK_GRP
//! \{
//!
//! \image html rte_bdk.jpg
//-----------------------------------------------------------------------------

#ifndef RTE_BDK_H_
#define RTE_BDK_H_

// <<< Use Configuration Wizard in Context Menu >>>

// <o> SYSCLK Frequency
// <i> Frequency that the Cortex M3 core will be running at.
// <i> Other clocks will be chosen based on selected system clock.
//   <0=> 48 MHz
//   <1=> 24 MHz
//   <2=> 16 MHz
//   <3=> 8 MHz
#define RTE_HAL_CLK_CONF                 3

// <o> APP Task Event Kernel message handler count. <2-64>
// <i> Maximum number of message handlers that can be assigned to application task.
// <i> Depends mainly on number of used BLE profiles.
// <i> Default: 24
#ifndef RTE_APP_TASK_HANDLER_COUNT
#define RTE_APP_TASK_HANDLER_COUNT       24
#endif


#endif /* RTE_BDK_H_ */

//! \}

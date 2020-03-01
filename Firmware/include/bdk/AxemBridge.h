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
//! \file BDK_AxemBridge.h
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup AXEM_GRP BDK Axem SPI to UART Bridge
//!
//! \brief
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BDK_AXEM_BRIDGE_H_
#define BDK_AXEM_BRIDGE_H_

#include <stdbool.h>
#include <stdint.h>

#include <HAL.h>
#include <PinNames.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** \brief Interrupt pin from Axem bridge indicating that there is at least one
 * character available to read.
 */
#define BDK_AXEM_IRQ_PAD               PIN_AXEM_IRQ

/** \brief SPI Chip select pin for Axem Bridge. */
#define BDK_AXEM_CS_PAD                PIN_AXEM_CS

#define BDK_AXEM_MAX_TRANSFER_SIZE     127

/** \brief Initialize DIO pads used by Axem Bridge. */
extern void BDK_AXEM_Initialize(void);

extern int32_t BDK_AXEM_Write(char *buf, uint32_t len);

extern int32_t BDK_AXEM_Read(char *buf, uint32_t len);

/** \brief Checks whether there are any characters available to read. */
extern bool BDK_AXEM_Readable(void);


#ifdef __cplusplus
}
#endif

#endif /* BDK_AXEM_BRIDGE_H_ */

//! \}
//! \}
//! \}

/* ----------------------------------------------------------------------------
 * Copyright (c) 2019 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * RTE_Device.h
 * - RTE Drivers configuration file. This file is used to enable/disable a
 *   certain driver and to configure the pin mapping for each interface.
 * ------------------------------------------------------------------------- */

#ifndef RTE_DEVICE_H
#define RTE_DEVICE_H

#include <RTE_Components.h>

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//

// <e> DMA Configuration
// =======================
//
//   <i> Defines if DMA driver should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA
 #define RTE_DMA     0
#endif
// <e>DMA 0 enabled
// ===============================
//
//   <i> Defines if dma 0 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA0_EN
 #define RTE_DMA0_EN       0
#endif
// <e>DMA 0 auto configuration
// ===============================
//
// <i> Enables dma channel 0 configuration.
// <i> Default: 1
#ifndef RTE_DMA0_AUTO_EN
 #define RTE_DMA0_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA0_SRC_SEL_DEFAULT
 #define RTE_DMA0_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA0_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA0_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA0_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA0_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA0_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA0_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA0_DST_SEL_DEFAULT
 #define RTE_DMA0_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA0_DST_STEP_MODE_DEFAULT
 #define RTE_DMA0_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA0_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA0_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA0_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA0_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA0_DATA_MODE_DEFAULT
 #define RTE_DMA0_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA0_BYTE_ORDER_DEFAULT
 #define RTE_DMA0_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA0_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA0_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA0_INT_PREEMPT_PRI
 #define RTE_DMA0_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA0_INT_SUBGRP_PRI
 #define RTE_DMA0_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 1 enabled
// ===============================
//
//   <i> Defines if dma 1 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA1_EN
 #define RTE_DMA1_EN       0
#endif
// <e>DMA 1 auto configuration
// ===============================
//
// <i> Enables dma channel 1 configuration.
// <i> Default: 1
#ifndef RTE_DMA1_AUTO_EN
 #define RTE_DMA1_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA1_SRC_SEL_DEFAULT
 #define RTE_DMA1_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA1_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA1_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA1_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA1_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA1_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA1_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA1_DST_SEL_DEFAULT
 #define RTE_DMA1_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA1_DST_STEP_MODE_DEFAULT
 #define RTE_DMA1_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA1_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA1_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA1_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA1_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA1_DATA_MODE_DEFAULT
 #define RTE_DMA1_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA1_BYTE_ORDER_DEFAULT
 #define RTE_DMA1_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA1_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA1_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA1_INT_PREEMPT_PRI
 #define RTE_DMA1_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA1_INT_SUBGRP_PRI
 #define RTE_DMA1_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 2 enabled
// ===============================
//
//   <i> Defines if dma 2 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA2_EN
 #define RTE_DMA2_EN       0
#endif
// <e>DMA 2 auto configuration
// ===============================
//
// <i> Enables dma channel 2 configuration.
// <i> Default: 1
#ifndef RTE_DMA2_AUTO_EN
 #define RTE_DMA2_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA2_SRC_SEL_DEFAULT
 #define RTE_DMA2_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA2_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA2_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA2_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA2_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA2_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA2_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA2_DST_SEL_DEFAULT
 #define RTE_DMA2_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA2_DST_STEP_MODE_DEFAULT
 #define RTE_DMA2_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA2_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA2_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA2_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA2_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA2_DATA_MODE_DEFAULT
 #define RTE_DMA2_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA2_BYTE_ORDER_DEFAULT
 #define RTE_DMA2_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA2_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA2_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA2_INT_PREEMPT_PRI
 #define RTE_DMA2_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA2_INT_SUBGRP_PRI
 #define RTE_DMA2_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 3 enabled
// ===============================
//
//   <i> Defines if dma 3 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA3_EN
 #define RTE_DMA3_EN       1
#endif
// <e>DMA 3 auto configuration
// ===============================
//
// <i> Enables dma channel 3 configuration.
// <i> Default: 1
#ifndef RTE_DMA3_AUTO_EN
 #define RTE_DMA3_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA3_SRC_SEL_DEFAULT
 #define RTE_DMA3_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA3_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA3_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA3_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA3_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA3_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA3_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA3_DST_SEL_DEFAULT
 #define RTE_DMA3_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA3_DST_STEP_MODE_DEFAULT
 #define RTE_DMA3_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA3_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA3_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA3_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA3_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA3_DATA_MODE_DEFAULT
 #define RTE_DMA3_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA3_BYTE_ORDER_DEFAULT
 #define RTE_DMA3_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA3_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA3_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA3_INT_PREEMPT_PRI
 #define RTE_DMA3_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA3_INT_SUBGRP_PRI
 #define RTE_DMA3_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 4 enabled
// ===============================
//
//   <i> Defines if dma 4 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA4_EN
 #define RTE_DMA4_EN       0
#endif
// <e>DMA 4 auto configuration
// ===============================
//
// <i> Enables dma channel 4 configuration.
// <i> Default: 1
#ifndef RTE_DMA4_AUTO_EN
 #define RTE_DMA4_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA4_SRC_SEL_DEFAULT
 #define RTE_DMA4_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA4_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA4_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA4_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA4_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA4_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA4_SRC_WORD_SIZE_DEFAULT   2
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA4_DST_SEL_DEFAULT
 #define RTE_DMA4_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA4_DST_STEP_MODE_DEFAULT
 #define RTE_DMA4_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA4_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA4_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA4_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA4_DST_WORD_SIZE_DEFAULT   2
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA4_DATA_MODE_DEFAULT
 #define RTE_DMA4_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA4_BYTE_ORDER_DEFAULT
 #define RTE_DMA4_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA4_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA4_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA4_INT_PREEMPT_PRI
 #define RTE_DMA4_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA4_INT_SUBGRP_PRI
 #define RTE_DMA4_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 5 enabled
// ===============================
//
//   <i> Defines if dma 5 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA5_EN
 #define RTE_DMA5_EN       0
#endif
// <e>DMA 5 auto configuration
// ===============================
//
// <i> Enables dma channel 5 configuration.
// <i> Default: 1
#ifndef RTE_DMA5_AUTO_EN
 #define RTE_DMA5_AUTO_EN       1
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA5_SRC_SEL_DEFAULT
 #define RTE_DMA5_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA5_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA5_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA5_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA5_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA5_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA5_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA5_DST_SEL_DEFAULT
 #define RTE_DMA5_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA5_DST_STEP_MODE_DEFAULT
 #define RTE_DMA5_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA5_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA5_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA5_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA5_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA5_DATA_MODE_DEFAULT
 #define RTE_DMA5_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA5_BYTE_ORDER_DEFAULT
 #define RTE_DMA5_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA5_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA5_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA5_INT_PREEMPT_PRI
 #define RTE_DMA5_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA5_INT_SUBGRP_PRI
 #define RTE_DMA5_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 6 enabled
// ===============================
//
//   <i> Defines if dma 6 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA6_EN
 #define RTE_DMA6_EN       0
#endif
// <e>DMA 6 auto configuration
// ===============================
//
// <i> Enables dma channel 6 configuration.
// <i> Default: 1
#ifndef RTE_DMA6_AUTO_EN
 #define RTE_DMA6_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA6_SRC_SEL_DEFAULT
 #define RTE_DMA6_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA6_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA6_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA6_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA6_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA6_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA6_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA6_DST_SEL_DEFAULT
 #define RTE_DMA6_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA6_DST_STEP_MODE_DEFAULT
 #define RTE_DMA6_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA6_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA6_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA6_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA6_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA6_DATA_MODE_DEFAULT
 #define RTE_DMA6_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA6_BYTE_ORDER_DEFAULT
 #define RTE_DMA6_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA6_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA6_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA6_INT_PREEMPT_PRI
 #define RTE_DMA6_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA6_INT_SUBGRP_PRI
 #define RTE_DMA6_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// <e>DMA 7 enabled
// ===============================
//
//   <i> Defines if dma 7 should be enabled.
//   <i> Default: 1
#ifndef RTE_DMA7_EN
 #define RTE_DMA7_EN       0
#endif
// <e>DMA 7 auto configuration
// ===============================
//
// <i> Enables dma channel 7 configuration.
// <i> Default: 1
#ifndef RTE_DMA7_AUTO_EN
 #define RTE_DMA7_AUTO_EN       0
#endif
// <h>Source configuration
// ===============================
//
//   <o>Source target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the source target
//   <i> Default: MEMORY
#ifndef RTE_DMA7_SRC_SEL_DEFAULT
 #define RTE_DMA7_SRC_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the source address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA7_SRC_STEP_MODE_DEFAULT
 #define RTE_DMA7_SRC_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the source address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA7_SRC_STEP_SIZE_DEFAULT
 #define RTE_DMA7_SRC_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single source word size
//   <i> Default: 8bits
#ifndef RTE_DMA7_SRC_WORD_SIZE_DEFAULT
 #define RTE_DMA7_SRC_WORD_SIZE_DEFAULT   0
#endif
// </h>
// <h>Destination configuration
// ===============================
//
//   <o>Destination target
//       <0x0=> I2C    <0x1=> SPI0
//       <0x2=> SPI1   <0x3=> PCM
//       <0x4=> UART   <0x5=> ASRC
//       <0x6=> PBUS   <0x7=> DMIC
//       <0x8=> MEMORY
//   <i> Defines the destination target
//   <i> Default: MEMORY
#ifndef RTE_DMA7_DST_SEL_DEFAULT
 #define RTE_DMA7_DST_SEL_DEFAULT   8
#endif
//   <o>Step mode
//       <0x0=> Increment
//       <0x1=> Decrement
//       <0x2=> Static
//   <i> Defines if the destination address should be incremented / decremented during the trasnfer
//   <i> Default: Increment
#ifndef RTE_DMA7_DST_STEP_MODE_DEFAULT
 #define RTE_DMA7_DST_STEP_MODE_DEFAULT   0
#endif
//   <o>Step size
//       <0x0=> 1 x 32bits
//       <0x1=> 2 x 32bits
//       <0x2=> 3 x 32bits
//       <0x3=> 4 x 32bits
//   <i> Defines the destination address step size. Valid only if increment / decrement step mode was selected
//   <i> Default: 1 x 32bit
#ifndef RTE_DMA7_DST_STEP_SIZE_DEFAULT
 #define RTE_DMA7_DST_STEP_SIZE_DEFAULT   0
#endif
//   <o>Word size
//       <0x3=> 4bits
//       <0x0=> 8bits
//       <0x1=> 16bits
//       <0x2=> 32bits
//   <i> Defines the single destination word size
//   <i> Default: 8bits
#ifndef RTE_DMA7_DST_WORD_SIZE_DEFAULT
 #define RTE_DMA7_DST_WORD_SIZE_DEFAULT   0
#endif
// </h>
//   <o>Data transfer mode
//       <0x0=> Repeat
//       <0x1=> Single
//   <i> Defines if the transfer should be executed once or repeatedly
//   <i> Default: Single
#ifndef RTE_DMA7_DATA_MODE_DEFAULT
 #define RTE_DMA7_DATA_MODE_DEFAULT   1
#endif
//   <o>Data endianness
//       <0x0=> Little Endian
//       <0x1=> Big endian
//   <i> Defines the data bytes order to be used
//   <i> Default: Little Endian
#ifndef RTE_DMA7_BYTE_ORDER_DEFAULT
 #define RTE_DMA7_BYTE_ORDER_DEFAULT   0
#endif
//   <o>Channel priority
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//   <i> Defines the channel priority
//   <i> Default: 0
#ifndef RTE_DMA7_CHANNEL_PRIORITY_DEFAULT
 #define RTE_DMA7_CHANNEL_PRIORITY_DEFAULT   0
#endif
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_DMA7_INT_PREEMPT_PRI
 #define RTE_DMA7_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_DMA7_INT_SUBGRP_PRI
 #define RTE_DMA7_INT_SUBGRP_PRI   0
#endif
// </h>
// </e>
// </e>
// </e>


// <e> GPIO Configuration
// =======================
//
//   <i> Defines if GPIO driver should be enabled.
//   <i> Default: 1
#ifndef RTE_GPIO
 #define RTE_GPIO     1
#endif
// <e>GPIO 0 configure
// ===============================
//
// <i> Enabled GPIO0 configuration
// <i> Default: 0
#ifndef RTE_GPIO0_EN
 #define RTE_GPIO0_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO0_DRIVE_DEFAULT
 #define RTE_GPIO0_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO0_LPF_DEFAULT
 #define RTE_GPIO0_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO0_PULL_DEFAULT
 #define RTE_GPIO0_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03E=> INPUT                 <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO0_IOMODE_DEFAULT
 #define RTE_GPIO0_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 1 configure
// ===============================
//
// <i> Enabled GPIO1 configuration
// <i> Default: 0
#ifndef RTE_GPIO1_EN
 #define RTE_GPIO1_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO1_DRIVE_DEFAULT
 #define RTE_GPIO1_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO1_LPF_DEFAULT
 #define RTE_GPIO1_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO1_PULL_DEFAULT
 #define RTE_GPIO1_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03E=> INPUT                 <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO1_IOMODE_DEFAULT
 #define RTE_GPIO1_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 2 configure
// ===============================
//
// <i> Enabled GPIO2 configuration
// <i> Default: 0
#ifndef RTE_GPIO2_EN
 #define RTE_GPIO2_EN       1
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO2_DRIVE_DEFAULT
 #define RTE_GPIO2_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO2_LPF_DEFAULT
 #define RTE_GPIO2_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO2_PULL_DEFAULT
 #define RTE_GPIO2_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03E=> INPUT                 <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO2_IOMODE_DEFAULT
 #define RTE_GPIO2_IOMODE_DEFAULT   0x101
#endif
// </e>
// <e>GPIO 3 configure
// ===============================
//
// <i> Enabled GPIO3 configuration
// <i> Default: 0
#ifndef RTE_GPIO3_EN
 #define RTE_GPIO3_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO3_DRIVE_DEFAULT
 #define RTE_GPIO3_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO3_LPF_DEFAULT
 #define RTE_GPIO3_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO3_PULL_DEFAULT
 #define RTE_GPIO3_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03E=> INPUT                 <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO3_IOMODE_DEFAULT
 #define RTE_GPIO3_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 4 configure
// ===============================
//
// <i> Enabled GPIO4 configuration
// <i> Default: 0
#ifndef RTE_GPIO4_EN
 #define RTE_GPIO4_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO4_DRIVE_DEFAULT
 #define RTE_GPIO4_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO4_LPF_DEFAULT
 #define RTE_GPIO4_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO4_PULL_DEFAULT
 #define RTE_GPIO4_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the gpio0 io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO4_IOMODE_DEFAULT
 #define RTE_GPIO4_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 5 configure
// ===============================
//
// <i> Enabled GPIO5 configuration
// <i> Default: 0
#ifndef RTE_GPIO5_EN
 #define RTE_GPIO5_EN       1
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO5_DRIVE_DEFAULT
 #define RTE_GPIO5_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO5_LPF_DEFAULT
 #define RTE_GPIO5_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO5_PULL_DEFAULT
 #define RTE_GPIO5_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO5_IOMODE_DEFAULT
 #define RTE_GPIO5_IOMODE_DEFAULT   0x000
#endif
// </e>
// <e>GPIO 6 configure
// ===============================
//
// <i> Enabled GPIO6 configuration
// <i> Default: 0
#ifndef RTE_GPIO6_EN
 #define RTE_GPIO6_EN       1
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO6_DRIVE_DEFAULT
 #define RTE_GPIO6_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO6_LPF_DEFAULT
 #define RTE_GPIO6_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO6_PULL_DEFAULT
 #define RTE_GPIO6_PULL_DEFAULT   0
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO6_IOMODE_DEFAULT
 #define RTE_GPIO6_IOMODE_DEFAULT   0x002
#endif
// </e>
// <e>GPIO 7 configure
// ===============================
//
// <i> Enabled GPIO7 configuration
// <i> Default: 0
#ifndef RTE_GPIO7_EN
 #define RTE_GPIO7_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO7_DRIVE_DEFAULT
 #define RTE_GPIO7_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO7_LPF_DEFAULT
 #define RTE_GPIO7_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO7_PULL_DEFAULT
 #define RTE_GPIO7_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO7_IOMODE_DEFAULT
 #define RTE_GPIO7_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 8 configure
// ===============================
//
// <i> Enabled GPIO8 configuration
// <i> Default: 0
#ifndef RTE_GPIO8_EN
 #define RTE_GPIO8_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO8_DRIVE_DEFAULT
 #define RTE_GPIO8_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO8_LPF_DEFAULT
 #define RTE_GPIO8_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO8_PULL_DEFAULT
 #define RTE_GPIO8_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO8_IOMODE_DEFAULT
 #define RTE_GPIO8_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 9 configure
// ===============================
//
// <i> Enabled GPIO9 configuration
// <i> Default: 0
#ifndef RTE_GPIO9_EN
 #define RTE_GPIO9_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO9_DRIVE_DEFAULT
 #define RTE_GPIO9_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO9_LPF_DEFAULT
 #define RTE_GPIO9_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO9_PULL_DEFAULT
 #define RTE_GPIO9_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO9_IOMODE_DEFAULT
 #define RTE_GPIO9_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 10 configure
// ===============================
//
// <i> Enabled GPIO10 configuration
// <i> Default: 0
#ifndef RTE_GPIO10_EN
 #define RTE_GPIO10_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO10_DRIVE_DEFAULT
 #define RTE_GPIO10_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO10_LPF_DEFAULT
 #define RTE_GPIO10_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO10_PULL_DEFAULT
 #define RTE_GPIO10_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO10_IOMODE_DEFAULT
 #define RTE_GPIO10_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 11 configure
// ===============================
//
// <i> Enabled GPIO11 configuration
// <i> Default: 0
#ifndef RTE_GPIO11_EN
 #define RTE_GPIO11_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO11_DRIVE_DEFAULT
 #define RTE_GPIO11_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO11_LPF_DEFAULT
 #define RTE_GPIO11_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO11_PULL_DEFAULT
 #define RTE_GPIO11_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO11_IOMODE_DEFAULT
 #define RTE_GPIO11_IOMODE_DEFAULT   0x000
#endif
// </e>
// <e>GPIO 12 configure
// ===============================
//
// <i> Enabled GPIO12 configuration
// <i> Default: 0
#ifndef RTE_GPIO12_EN
 #define RTE_GPIO12_EN       1
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO12_DRIVE_DEFAULT
 #define RTE_GPIO12_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO12_LPF_DEFAULT
 #define RTE_GPIO12_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO12_PULL_DEFAULT
 #define RTE_GPIO12_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO12_IOMODE_DEFAULT
 #define RTE_GPIO12_IOMODE_DEFAULT   0x000
#endif
// </e>
// <e>GPIO 13 configure
// ===============================
//
// <i> Enabled GPIO13 configuration
// <i> Default: 0
#ifndef RTE_GPIO13_EN
 #define RTE_GPIO13_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO13_DRIVE_DEFAULT
 #define RTE_GPIO13_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO13_LPF_DEFAULT
 #define RTE_GPIO13_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO13_PULL_DEFAULT
 #define RTE_GPIO13_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO13_IOMODE_DEFAULT
 #define RTE_GPIO13_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 14 configure
// ===============================
//
// <i> Enabled GPIO14 configuration
// <i> Default: 0
#ifndef RTE_GPIO14_EN
 #define RTE_GPIO14_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO14_DRIVE_DEFAULT
 #define RTE_GPIO14_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO14_LPF_DEFAULT
 #define RTE_GPIO14_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO14_PULL_DEFAULT
 #define RTE_GPIO14_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO14_IOMODE_DEFAULT
 #define RTE_GPIO14_IOMODE_DEFAULT   0x03F
#endif
// </e>
// <e>GPIO 15 configure
// ===============================
//
// <i> Enabled GPIO15 configuration
// <i> Default: 0
#ifndef RTE_GPIO15_EN
 #define RTE_GPIO15_EN       0
#endif
//   <o>Drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the GPIO output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO15_DRIVE_DEFAULT
 #define RTE_GPIO15_DRIVE_DEFAULT   3
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO15_LPF_DEFAULT
 #define RTE_GPIO15_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the gpio pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO15_PULL_DEFAULT
 #define RTE_GPIO15_PULL_DEFAULT   1
#endif
//   <o>IO mode
//   <0x000=> GPIO_IN_0             <0x001=> GPIO_IN_1             <0x002=> GPIO_OUT_0            <0x003=> GPIO_OUT_1
//   <0x004=> USRCLK_OUT            <0x005=> SLOWCLK_OUT           <0x006=> SYSCLK_OUT            <0x007=> PCM_SERO_OUT
//   <0x100=> PCM_SERI_IN           <0x103=> PCM_FRAME_OUT         <0x101=> PCM_FRAME_IN          <0x102=> PCM_CLK_IN
//   <0x009=> SPI0_SERO_OUT         <0x200=> SPI0_SERI_IN          <0x00A=> SPI0_CS_OUT           <0x00B=> SPI0_CLK_OUT
//   <0x201=> SPI0_CS_IN            <0x202=> SPI0_CLK_IN           <0x00C=> SPI1_SERO_OUT         <0x203=> SPI1_SERI_IN
//   <0x00D=> SPI1_CS_OUT           <0x00E=> SPI1_CLK_OUT          <0x204=> SPI1_CS_IN            <0x205=> SPI1_CLK_IN
//   <0x300=> UART_RX_IN            <0x00F=> UART_TX_OUT
//   <0x400=> I2C_SCL               <0x401=> I2C_SDA               <0x012=> PWM0_OUT              <0x013=> PWM0_INV_OUT
//   <0x014=> PWM1_OUT              <0x015=> PWM1_INV_OUT          <0x016=> LPDSP32_TDO_OUT       <0x500=> LPDSP32_TDI_IN
//   <0x501=> LPDSP32_TMS_IN        <0x502=> LPDSP32_TCK_IN        <0x017=> RFCLK_OUT             <0x018=> RCCLK_OUT
//   <0x019=> JTCK_DIV_OUT          <0x01A=> EXTCLK_DIV_OUT        <0x01B=> STANDBYCLK_OUT        <0x600=> AUDIOSINK_IN
//   <0x700=> NMI_IN                <0x01C=> BB_TX_DATA_OUT        <0x01D=> BB_TX_DATA_VALID_OUT  <0x800=> BB_RX_CLK_IN
//   <0x801=> BB_RX_DATA_IN         <0x802=> BB_SYNC_P_IN          <0x01E=> BB_SYNC_P_OUT         <0x01F=> BB_AUDIO0_SYNC_P_OUT
//   <0x020=> BB_AUDIO1_SYNC_P_OUT  <0x021=> BB_AUDIO2_SYNC_P_OUT  <0x022=> BB_SPI_CSN_OUT        <0x023=> BB_SPI_CLK_OUT
//   <0x024=> BB_SPI_MOSI_OUT       <0x900=> BB_SPI_MISO_IN        <0x025=> BB_DBG0_0_OUT         <0x026=> BB_DBG0_1_OUT
//   <0x027=> BB_DBG0_2_OUT         <0x028=> BB_DBG0_3_OUT         <0x029=> BB_DBG0_4_OUT         <0x02A=> BB_DBG0_5_OUT
//   <0x02B=> BB_DBG0_6_OUT         <0x02C=> BB_DBG0_7_OUT         <0x02D=> RF_SPI_MISO_OUT       <0xA00=> RF_SPI_MOSI_IN
//   <0xA01=> RF_SPI_CSN_IN         <0xA02=> RF_SPI_CLK_IN         <0x02E=> RF_GPIO0_OUT          <0x02F=> RF_GPIO1_OUT
//   <0x030=> RF_GPIO2_OUT          <0x031=> RF_GPIO3_OUT          <0x032=> RF_GPIO4_OUT          <0x033=> RF_GPIO5_OUT
//   <0x034=> RF_GPIO6_OUT          <0x035=> RF_GPIO7_OUT          <0x036=> RF_GPIO8_OUT          <0x037=> RF_GPIO9_OUT
//   <0xB00=> RF_GPIO0_IN           <0xB01=> RF_GPIO1_IN           <0xB02=> RF_GPIO2_IN           <0xB03=> RF_GPIO3_IN
//   <0xB04=> RF_GPIO4_IN           <0xB05=> RF_GPIO5_IN           <0xB06=> RF_GPIO6_IN           <0xB07=> RF_GPIO7_IN
//   <0xB08=> RF_GPIO8_IN           <0xB09=> RF_GPIO9_IN           <0x038=> AUDIOCLK_OUT          <0x039=> AUDIOSLOWCLK_OUT
//   <0x03A=> OD_P_OUT              <0x03B=> OD_N_OUT              <0xC00=> DMIC_CLK_AUDIOCLK     <0xC01=> DMIC_CLK_AUDIOSLOWCLK
//   <0xC02=> DMIC_DATA_IN          <0x03C=> AUDIO_SYNC_PULSE_OUT  <0x03D=> AUDIO_SYNC_MISSED_OUT <0x03E=> ADC_IN
//   <0x03F=> DISABLED
//   <i> Defines the io mode
//   <i> Default: DISABLED
#ifndef RTE_GPIO15_IOMODE_DEFAULT
 #define RTE_GPIO15_IOMODE_DEFAULT   0x03F
#endif
// </e>
//   <o>Drive strengths
//       <0x0=> regular drive strengths
//       <0x1=> increased drive strengths
//   <i> Configures the drive strengths
//   <i> Default: increased drive strengths
#ifndef RTE_GPIO_DRIVE_STRENGTHS_DEFAULT
 #define RTE_GPIO_DRIVE_STRENGTHS_DEFAULT   0
#endif
// <e>DIO 0 interrupt enable
// ===============================
//
// <i> Enables DIO0 interrupt.
// <i> Default: 1
#ifndef RTE_GPIO_DIO0_INT_EN
 #define RTE_GPIO_DIO0_INT_EN       0
#endif
//   <o>Interrupt source
//       <0x0=> GPIO 0   <0x1=> GPIO 1   <0x2=> GPIO 2   <0x3=> GPIO 3
//       <0x4=> GPIO 4   <0x5=> GPIO 5   <0x6=> GPIO 6   <0x7=> GPIO 7
//       <0x8=> GPIO 8   <0x9=> GPIO 9   <0xA=> GPIO 10  <0xB=> GPIO 11
//       <0xC=> GPIO 12  <0xD=> GPIO 13  <0xE=> GPIO 14  <0xF=> GPIO 15
//   <i> Defines the DIO0 interrupt source
//   <i> Default: GPIO 0
#ifndef RTE_GPIO_DIO0_SRC_DEFAULT
 #define RTE_GPIO_DIO0_SRC_DEFAULT   5
#endif
//   <o>Trigger event
//       <0x0=> none           <0x1=> high level      <0x2=> low level
//       <0x3=> rising edge    <0x4=> falling edge    <0x5=> transition
//   <i> Defines the interrupt trigger event
//   <i> Default: rising edge
#ifndef RTE_GPIO_DIO0_TRG_DEFAULT
 #define RTE_GPIO_DIO0_TRG_DEFAULT   2
#endif
//   <o>Debounce filter
//       <0x0=> disabled
//       <0x1=> enabled
//   <i> Defines if debounce filter should be enabled
//   <i> Default: disabled
#ifndef RTE_GPIO_DIO0_DBC_DEFAULT
 #define RTE_GPIO_DIO0_DBC_DEFAULT   1
#endif
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO0_INT_PREEMPT_PRI
 #define RTE_GPIO_DIO0_INT_PREEMPT_PRI   1
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO0_INT_SUBGRP_PRI
 #define RTE_GPIO_DIO0_INT_SUBGRP_PRI   1
#endif
// </e>
// <e>DIO 1 interrupt enable
// ===============================
//
// <i> Enables DIO1 interrupt.
// <i> Default: 1
#ifndef RTE_GPIO_DIO1_INT_EN
 #define RTE_GPIO_DIO1_INT_EN       0
#endif
//   <o>Interrupt source
//       <0x0=> GPIO 0   <0x1=> GPIO 1   <0x2=> GPIO 2   <0x3=> GPIO 3
//       <0x4=> GPIO 4   <0x5=> GPIO 5   <0x6=> GPIO 6   <0x7=> GPIO 7
//       <0x8=> GPIO 8   <0x9=> GPIO 9   <0xA=> GPIO 10  <0xB=> GPIO 11
//       <0xC=> GPIO 12  <0xD=> GPIO 13  <0xE=> GPIO 14  <0xF=> GPIO 15
//   <i> Defines the DIO1 interrupt source
//   <i> Default: GPIO 1
#ifndef RTE_GPIO_DIO1_SRC_DEFAULT
 #define RTE_GPIO_DIO1_SRC_DEFAULT   1
#endif
//   <o>Trigger event
//       <0x0=> none           <0x1=> high level      <0x2=> low level
//       <0x3=> rising edge    <0x4=> falling edge    <0x5=> transition
//   <i> Defines the interrupt trigger event
//   <i> Default: rising edge
#ifndef RTE_GPIO_DIO1_TRG_DEFAULT
 #define RTE_GPIO_DIO1_TRG_DEFAULT   3
#endif
//   <o>Debounce filter
//       <0x0=> disabled
//       <0x1=> enabled
//   <i> Defines if debounce filter should be enabled
//   <i> Default: disabled
#ifndef RTE_GPIO_DIO1_DBC_DEFAULT
 #define RTE_GPIO_DIO1_DBC_DEFAULT   0
#endif
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO1_INT_PREEMPT_PRI
 #define RTE_GPIO_DIO1_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO1_INT_SUBGRP_PRI
 #define RTE_GPIO_DIO1_INT_SUBGRP_PRI   0
#endif
// </e>
// <e>DIO 2 interrupt enable
// ===============================
//
// <i> Enables DIO1 interrupt.
// <i> Default: 1
#ifndef RTE_GPIO_DIO2_INT_EN
 #define RTE_GPIO_DIO2_INT_EN       0
#endif
//   <o>Interrupt source
//       <0x0=> GPIO 0   <0x1=> GPIO 1   <0x2=> GPIO 2   <0x3=> GPIO 3
//       <0x4=> GPIO 4   <0x5=> GPIO 5   <0x6=> GPIO 6   <0x7=> GPIO 7
//       <0x8=> GPIO 8   <0x9=> GPIO 9   <0xA=> GPIO 10  <0xB=> GPIO 11
//       <0xC=> GPIO 12  <0xD=> GPIO 13  <0xE=> GPIO 14  <0xF=> GPIO 15
//   <i> Defines the DIO2 interrupt source
//   <i> Default: GPIO 2
#ifndef RTE_GPIO_DIO2_SRC_DEFAULT
 #define RTE_GPIO_DIO2_SRC_DEFAULT   2
#endif
//   <o>Trigger event
//       <0x0=> none           <0x1=> high level      <0x2=> low level
//       <0x3=> rising edge    <0x4=> falling edge    <0x5=> transition
//   <i> Defines the interrupt trigger event
//   <i> Default: rising edge
#ifndef RTE_GPIO_DIO2_TRG_DEFAULT
 #define RTE_GPIO_DIO2_TRG_DEFAULT   3
#endif
//   <o>Debounce filter
//       <0x0=> disabled
//       <0x1=> enabled
//   <i> Defines if debounce filter should be enabled
//   <i> Default: disabled
#ifndef RTE_GPIO_DIO2_DBC_DEFAULT
 #define RTE_GPIO_DIO2_DBC_DEFAULT   0
#endif
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO2_INT_PREEMPT_PRI
 #define RTE_GPIO_DIO2_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO2_INT_SUBGRP_PRI
 #define RTE_GPIO_DIO2_INT_SUBGRP_PRI   0
#endif
// </e>
// <e>DIO 3 interrupt enable
// ===============================
//
// <i> Enables DIO3 interrupt.
// <i> Default: 1
#ifndef RTE_GPIO_DIO3_INT_EN
 #define RTE_GPIO_DIO3_INT_EN       0
#endif
//   <o>Interrupt source
//       <0x0=> GPIO 0   <0x1=> GPIO 1   <0x2=> GPIO 2   <0x3=> GPIO 3
//       <0x4=> GPIO 4   <0x5=> GPIO 5   <0x6=> GPIO 6   <0x7=> GPIO 7
//       <0x8=> GPIO 8   <0x9=> GPIO 9   <0xA=> GPIO 10  <0xB=> GPIO 11
//       <0xC=> GPIO 12  <0xD=> GPIO 13  <0xE=> GPIO 14  <0xF=> GPIO 15
//   <i> Defines the DIO3 interrupt source
//   <i> Default: GPIO 3
#ifndef RTE_GPIO_DIO3_SRC_DEFAULT
 #define RTE_GPIO_DIO3_SRC_DEFAULT   3
#endif
//   <o>Trigger event
//       <0x0=> none           <0x1=> high level      <0x2=> low level
//       <0x3=> rising edge    <0x4=> falling edge    <0x5=> transition
//   <i> Defines the interrupt trigger event
//   <i> Default: rising edge
#ifndef RTE_GPIO_DIO3_TRG_DEFAULT
 #define RTE_GPIO_DIO3_TRG_DEFAULT   3
#endif
//   <o>Debounce filter
//       <0x0=> disabled
//       <0x1=> enabled
//   <i> Defines if debounce filter should be enabled
//   <i> Default: disabled
#ifndef RTE_GPIO_DIO3_DBC_DEFAULT
 #define RTE_GPIO_DIO3_DBC_DEFAULT   0
#endif
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO3_INT_PREEMPT_PRI
 #define RTE_GPIO_DIO3_INT_PREEMPT_PRI   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_GPIO_DIO3_INT_SUBGRP_PRI
 #define RTE_GPIO_DIO3_INT_SUBGRP_PRI   0
#endif
// </e>
// <h>Interrupt debounce configuration
// ===============================
//
//   <o>Debounce clock source
//       <0x0=> slow clock / 32
//       <0x1=> slow clock / 1024
//   <i> Defines the debouce filter clock soruce
//   <i> Default: slow clock / 32
#ifndef RTE_GPIO_INT_DBC_CLK_SRC_DEFAULT
 #define RTE_GPIO_INT_DBC_CLK_SRC_DEFAULT   1
#endif
//   <o>Debounce filter count <0-0xFF>
//   <i> Defines debounce filter count
//   <i> Default: 10
#ifndef RTE_GPIO_INT_DBC_CNT_DEFAULT
 #define RTE_GPIO_INT_DBC_CNT_DEFAULT   160
#endif
// </h>
// <e>NMI configuration enable
// ===============================
//
// <i> Enables NMI polarity configuration.
// <i> Default: 0
#ifndef RTE_GPIO_NMI_EN
 #define RTE_GPIO_NMI_EN       0
#endif
//   <o>NMI polarity selection
//       <0x0=> nmi active low
//       <0x1=> nmi active high
//   <i> Configures the NMI polarity
//   <i> Default: nmi active high
#ifndef RTE_GPIO_NMI_POL_DEFAULT
 #define RTE_GPIO_NMI_POL_DEFAULT   1
#endif
// </e>
// <e>External clock configuration enable
// ===============================
//
// <i> Enables JTAG SW PAD configuration.
// <i> Default: 0
#ifndef RTE_GPIO_EXTCLK_EN
 #define RTE_GPIO_EXTCLK_EN       0
#endif
//   <o>Low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled
//   <i> Default: disable
#ifndef RTE_GPIO_EXTCLK_LPF_DEFAULT
 #define RTE_GPIO_EXTCLK_LPF_DEFAULT   0
#endif
//   <o>Pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the external clock pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO_EXTCLK_PULL_DEFAULT
 #define RTE_GPIO_EXTCLK_PULL_DEFAULT   1
#endif
// </e>
// <e>JTAG SW PAD configuration enable
// ===============================
//
// <i> Enables JTAG SW PAD configuration.
// <i> Default: 0
#ifndef RTE_GPIO_JTAG_EN
 #define RTE_GPIO_JTAG_EN       0
#endif
//   <o>JTCK low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled for JTCK
//   <i> Default: disable
#ifndef RTE_GPIO_JTAG_JTCK_LPF_DEFAULT
 #define RTE_GPIO_JTAG_JTCK_LPF_DEFAULT   0
#endif
//   <o>JTMS low pass filter
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if low pass filter should be enabled for JTMS
//   <i> Default: disable
#ifndef RTE_GPIO_JTAG_JTMS_LPF_DEFAULT
 #define RTE_GPIO_JTAG_JTMS_LPF_DEFAULT   0
#endif
//   <o>JTAG data enable
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if data (TDI and TDO) should be available on DIO[14:15]
//   <i> Default: enable
#ifndef RTE_GPIO_JTAG_DATA_DEFAULT
 #define RTE_GPIO_JTAG_DATA_DEFAULT   1
#endif
//   <o>TRST enable
//       <0x0=> disable
//       <0x1=> enable
//   <i> Defines if data (TRST) should be available on DIO13
//   <i> Default: enable
#ifndef RTE_GPIO_JTAG_TRST_DEFAULT
 #define RTE_GPIO_JTAG_TRST_DEFAULT   1
#endif
//   <o>JTCK pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the JTCK pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO_JTAG_JTCK_PULL_DEFAULT
 #define RTE_GPIO_JTAG_JTCK_PULL_DEFAULT   1
#endif
//   <o>JTMS pull control
//       <0x0=> no pull
//       <0x1=> weak pull up
//       <0x2=> weak pull down
//       <0x3=> strong pull up
//   <i> Defines the JTMS pull configuration
//   <i> Default: weak pull up
#ifndef RTE_GPIO_JTAG_JTMS_PULL_DEFAULT
 #define RTE_GPIO_JTAG_JTMS_PULL_DEFAULT   1
#endif
//   <o>JTMS drive strength
//       <0x0=> 2x drive strength
//       <0x1=> 3x drive strength
//       <0x2=> 5x drive strength
//       <0x3=> 6x drive strength
//   <i> Defines the JTMS output strength
//   <i> Default: 6x drive strength
#ifndef RTE_GPIO_JTAG_JTMS_DRIVE_DEFAULT
 #define RTE_GPIO_JTAG_JTMS_DRIVE_DEFAULT   3
#endif
// </e>
// </e>

// <e>I2C Configuration
// =======================
//
//   <i> Defines if I2C driver should be enabled.
//   <i> Default: 1
#ifndef RTE_I2C
 #define RTE_I2C     0
#endif
//   <e>I2C auto configuration
// ===============================
//
//   <i> If enabled, extends the initialize function so all configuration steps are performed at once.
//   <i> Drivers are also powered up during initialization if enabled.
//   <i> Default: enable
#ifndef RTE_I2C0_CFG_EN_DEFAULT
 #define RTE_I2C0_CFG_EN_DEFAULT   1
#endif
//   <o>I2C speed
//       <0x1=> standard speed (100kHz)
//       <0x2=> fast speed (400kHz)
//       <0x3=>  fast+ speed (1MHz)
//   <i> Defines the I2C speed
//   <i> Default: Fast Speed (400kHz)
#ifndef RTE_I2C0_SPEED_DEFAULT
 #define RTE_I2C0_SPEED_DEFAULT   1
#endif
//   <o>Slave address <0-0x7F>
//   <i> Defines the I2C slave address (used when operatin in slave mode).
//   <i> Default: 5
#ifndef RTE_I2C0_SLAVE_ADDR_DEFAULT
 #define RTE_I2C0_SLAVE_ADDR_DEFAULT   5
#endif
// </e>
// <e>DMA control
// ===============================
//
// <i> Enables dma usage for i2c.
// <i> Default: 0
#ifndef RTE_I2C0_DMA_EN_DEFAULT
 #define RTE_I2C0_DMA_EN_DEFAULT       0
#endif
//   <o>I2C dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer.
//   <i> Default: 3
#ifndef RTE_I2C0_DMA_CH_DEFAULT
 #define RTE_I2C0_DMA_CH_DEFAULT   3
#endif
// </e>
// <h>I2C pins configuration
// ===============================
//
//   <o>SDA gpio pad
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as a data line.
//   <i> Default: 0
#ifndef RTE_I2C0_SDA_PIN_DEFAULT
 #define RTE_I2C0_SDA_PIN_DEFAULT  8
#endif
//   <o>SCL gpio pad
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as a clock data line.
//   <i> Default: 0
#ifndef RTE_I2C0_SCL_PIN_DEFAULT
 #define RTE_I2C0_SCL_PIN_DEFAULT  7
#endif
// </h>
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_I2C_INT_PREEMPT_PRI_DEFAULT
 #define RTE_I2C_INT_PREEMPT_PRI_DEFAULT   1
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_I2C_INT_SUBGRP_PRI_DEFAULT
 #define RTE_I2C_INT_SUBGRP_PRI_DEFAULT   0
#endif
// </h>
// </e>



// <e>SAI Configuration
// =======================
//
//   <i> Defines if sai pcm driver should be enabled.
//   <i> Default: 1
#ifndef RTE_SAI_PCM
 #define RTE_SAI_PCM     1
#endif
//   <e>SAI auto configuration
// ===============================
//
//   <i> If enabled, extends the initialize function so all configuration steps are performed at once.
//   <i> Drivers are also powered up during initialization if enabled.
//   <i> Default: enable
#ifndef RTE_SAI_CFG_EN_DEFAULT
 #define RTE_SAI_CFG_EN_DEFAULT   1
#endif
//   <o>Mode selection
//       <0x0=> slave
//       <0x1=> master
//   <i> Defines if driver should be configured as slave or master
//   <i> Default: slave
#ifndef RTE_SAI_MODE_DEFAULT
 #define RTE_SAI_MODE_DEFAULT   0
#endif
//   <o>Protocol selection
//       <0x1=> I2S
//       <0x4=> PCM short
//       <0x5=> PCM long
//   <i> Sets the sai protocol
//   <i> Default: I2S
#ifndef RTE_SAI_PROTOCOL_DEFAULT
 #define RTE_SAI_PROTOCOL_DEFAULT   1
#endif
//   <o>Word size
//       <0x8=> 8bits (not supported in PCM long)
//       <0x10=> 16bits
//       <0x18=> 24bits
//       <0x20=> 32bits
//   <i> Sets the word size. If dma is used, only 32bit mode is supported.
//   <i> Default: 16
#ifndef RTE_SAI_WORD_SIZE_DEFAULT
 #define RTE_SAI_WORD_SIZE_DEFAULT   32
#endif
//   <o>Clock prescale <1-4096>
//   <i> Defines the mclk clock prescale
//   <i> Default: 1
#ifndef RTE_SAI_MCLK_PRESCALE_DEFAULT
 #define RTE_SAI_MCLK_PRESCALE_DEFAULT   64
#endif
// </e>
// <e>SAI DMA control
// ===============================
//
//   <i> Enables dma control of sai module
//   <i> Default: 0
#ifndef RTE_SAI_DMA_EN_DEFAULT
 #define RTE_SAI_DMA_EN_DEFAULT       1
#endif
//   <o>SAI rx dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer (valid only if dma control was selected).
//   <i> Default: 0
#ifndef RTE_SAI_RX_DMA_CH_DEFAULT
 #define RTE_SAI_RX_DMA_CH_DEFAULT   2
#endif
//   <o>SAI tx dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer (valid only if dma control was selected).
//   <i> Default: 1
#ifndef RTE_SAI_TX_DMA_CH_DEFAULT
 #define RTE_SAI_TX_DMA_CH_DEFAULT   3
#endif
// </e>
// <h>SAI PCM pins configuration
// ===============================
//
//   <o>PCM mclk pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the pcm clock line.
//   <i> Default: 0
#ifndef RTE_SAI_PCM_MCLK_PIN_DEFAULT
 #define RTE_SAI_PCM_MCLK_PIN_DEFAULT  8
#endif
//   <o>PCM frame pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the pcm frame line.
//   <i> Default: 1
#ifndef RTE_SAI_PCM_FRAME_PIN_DEFAULT
 #define RTE_SAI_PCM_FRAME_PIN_DEFAULT  7
#endif
//   <o>PCM SERI pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the seri line.
//   <i> Default: 2
#ifndef RTE_SAI_PCM_SERI_PIN_DEFAULT
 #define RTE_SAI_PCM_SERI_PIN_DEFAULT  1
#endif
//   <o>PCM SERO pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the sero line.
//   <i> Default: 3
#ifndef RTE_SAI_PCM_SERO_PIN_DEFAULT
 #define RTE_SAI_PCM_SERO_PIN_DEFAULT  0
#endif
// </h>
// <h>SAI Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_SAI_INT_PREEMPT_PRI_DEFAULT
 #define RTE_SAI_INT_PREEMPT_PRI_DEFAULT   0
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_SAI_INT_SUBGRP_PRI_DEFAULT
 #define RTE_SAI_INT_SUBGRP_PRI_DEFAULT   0
#endif
// </h>
// </e>

// <e>SPI Configuration
// =======================
//
//   <q>SPI auto configuration
//   <i> If enabled, extends the initialize function so all configuration steps are performed at once.
//   <i> Drivers are also powered up during initialization if enabled.
//   <i> Default: enable
#ifndef RTE_SPI_CFG_EN_DEFAULT
 #define RTE_SPI_CFG_EN_DEFAULT   0
#endif
//   <q>SPI0 enabled
//   <i> Defines if SPI0 driver should be enabled.
//   <i> Default: 1
#ifndef RTE_SPI0
 #define RTE_SPI0       1
#endif
//   <h>SPI0 auto configuration
// ===============================
//
//   <o>Mode selection
//       <0x0=> inactive
//       <0x10=> master ss unused
//       <0x11=> master
//       <0x20=> slave ss hw controlled
//       <0x21=> slave ss sw controlled
//   <i> Sets the SPI0 mode
//   <i> Default: master
#ifndef RTE_SPI0_MODE_DEFAULT
 #define RTE_SPI0_MODE_DEFAULT   0x11
#endif
//   <o>Speed selection <1000-10000000>
//   <i> Defines the SPI0 speed.
//   <i> The selected speed may not be achievable. Driver will set the closest speed by diving the sysclk by 2^N (N max = 9).
//   <i> Default: 750000
#ifndef RTE_SPI0_SPEED_DEFAULT
 #define RTE_SPI0_SPEED_DEFAULT   750000
#endif
//   <o>Clock polarity
//       <0x0=> normal
//       <0x2=> inverse
//   <i> Sets the SPI0 clock polarity
//   <i> Default: normal
#ifndef RTE_SPI0_POL_DEFAULT
 #define RTE_SPI0_POL_DEFAULT   0
#endif
//   <o>Word size
//       <0x8=> 8
//       <0x10=> 16
//       <0x18=> 24 (not supported in dma mode)
//       <0x20=> 32
//   <i> Sets the SPI0 word size
//   <i> Default: 8
#ifndef RTE_SPI0_WS_DEFAULT
 #define RTE_SPI0_WS_DEFAULT   8
#endif
// </h>
// <e>SPI0 DMA control
// ===============================
//
//   <i> Enables dma control of spi0 module
//   <i> Default: 0
#ifndef RTE_SPI0_DMA_EN_DEFAULT
 #define RTE_SPI0_DMA_EN_DEFAULT       0
#endif
//   <o>SPI0 rx dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer (valid only if dma control was selected).
//   <i> Default: 0
#ifndef RTE_SPI0_RX_DMA_CH_DEFAULT
 #define RTE_SPI0_RX_DMA_CH_DEFAULT   2
#endif
//   <o>SPI0 tx dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer (valid only if dma control was selected).
//   <i> Default: 1
#ifndef RTE_SPI0_TX_DMA_CH_DEFAULT
 #define RTE_SPI0_TX_DMA_CH_DEFAULT   3
#endif
// </e>
// <h>SPI0 pins configuration
// ===============================
//
//   <o>SCLK pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the sclk line.
//   <i> Default: 0
#ifndef RTE_SPI0_SCLK_PIN_DEFAULT
 #define RTE_SPI0_SCLK_PIN_DEFAULT  9
#endif
//   <o>SSEL pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the ssel line.
//   <i> Default: 1
#ifndef RTE_SPI0_SSEL_PIN_DEFAULT
 #define RTE_SPI0_SSEL_PIN_DEFAULT  10
#endif
//   <o>MISO pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the miso line.
//   <i> Default: 2
#ifndef RTE_SPI0_MISO_PIN_DEFAULT
 #define RTE_SPI0_MISO_PIN_DEFAULT  0
#endif
//   <o>MOSI pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the mosi line.
//   <i> Default: 3
#ifndef RTE_SPI0_MOSI_PIN_DEFAULT
 #define RTE_SPI0_MOSI_PIN_DEFAULT  12
#endif
// </h>
// <h>SPI0 Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_SPI0_INT_PREEMPT_PRI_DEFAULT
 #define RTE_SPI0_INT_PREEMPT_PRI_DEFAULT   1
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_SPI0_INT_SUBGRP_PRI_DEFAULT
 #define RTE_SPI0_INT_SUBGRP_PRI_DEFAULT   0
#endif
// </h>
//   <q>SPI1 enabled
//   <i> Defines if SPI1 driver should be enabled.
//   <i> Default: 1
#ifndef RTE_SPI1
 #define RTE_SPI1       1
#endif
//   <h>SPI1 auto configuration
// ===============================
//
//   <o>Mode selection
//       <0x0=> inactive
//       <0x10=> master ss unused
//       <0x11=> master
//       <0x20=> slave ss hw controlled
//       <0x21=> slave ss sw controlled
//   <i> Sets the SPI1 mode
//   <i> Default: master
#ifndef RTE_SPI1_MODE_DEFAULT
 #define RTE_SPI1_MODE_DEFAULT   0x20
#endif
//   <o>Speed selection <1000-10000000>
//   <i> Defines the SPI1 speed.
//   <i> The selected speed may not be achievable. Driver will set the closest speed by diving the sysclk by 2^N (N max = 9).
//   <i> Default: 750000
#ifndef RTE_SPI1_SPEED_DEFAULT
 #define RTE_SPI1_SPEED_DEFAULT   750000
#endif
//   <o>Clock polarity
//       <0x0=> normal
//       <0x2=> inverse
//   <i> Sets the SPI1 clock polarity
//   <i> Default: normal
#ifndef RTE_SPI1_POL_DEFAULT
 #define RTE_SPI1_POL_DEFAULT   0
#endif
//   <o>Word size
//       <0x8=> 8
//       <0x10=> 16
//       <0x18=> 24 (not supported in dma mode)
//       <0x20=> 32
//   <i> Sets the SPI1 word size
//   <i> Default: 8
#ifndef RTE_SPI1_WS_DEFAULT
 #define RTE_SPI1_WS_DEFAULT   8
#endif
// </h>
// <e>SPI1 DMA control
// ===============================
//
//   <i> Enables dma control of spi1 module
//   <i> Default: 0
#ifndef RTE_SPI1_DMA_EN_DEFAULT
 #define RTE_SPI1_DMA_EN_DEFAULT       1
#endif
//   <o>SPI1 rx dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer (valid only if dma control was selected).
//   <i> Default: 2
#ifndef RTE_SPI1_RX_DMA_CH_DEFAULT
 #define RTE_SPI1_RX_DMA_CH_DEFAULT   4
#endif
//   <o>SPI1 tx dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer (valid only if dma control was selected).
//   <i> Default: 3
#ifndef RTE_SPI1_TX_DMA_CH_DEFAULT
 #define RTE_SPI1_TX_DMA_CH_DEFAULT   5
#endif
// </e>
// <h>SPI1 pins configuration
// ===============================
//
//   <o>SCLK pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the sclk line.
//   <i> Default: 4
#ifndef RTE_SPI1_SCLK_PIN_DEFAULT
 #define RTE_SPI1_SCLK_PIN_DEFAULT  13
#endif
//   <o>SSEL pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the ssel line.
//   <i> Default: 13
#ifndef RTE_SPI1_SSEL_PIN_DEFAULT
 #define RTE_SPI1_SSEL_PIN_DEFAULT  14
#endif
//   <o>MISO pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the miso line.
//   <i> Default: 10
#ifndef RTE_SPI1_MISO_PIN_DEFAULT
 #define RTE_SPI1_MISO_PIN_DEFAULT  1
#endif
//   <o>MOSI pin
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as the mosi line.
//   <i> Default: 15
#ifndef RTE_SPI1_MOSI_PIN_DEFAULT
 #define RTE_SPI1_MOSI_PIN_DEFAULT  15
#endif
// </h>
// <h>SPI1 Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_SPI1_INT_PREEMPT_PRI_DEFAULT
 #define RTE_SPI1_INT_PREEMPT_PRI_DEFAULT   1
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_SPI1_INT_SUBGRP_PRI_DEFAULT
 #define RTE_SPI1_INT_SUBGRP_PRI_DEFAULT   0
#endif
// </h>
// </e>


// <e>USART Configuration
// =======================
//
//   <i> Defines if usart driver should be enabled.
//   <i> Default: 1
#ifndef RTE_USART
 #define RTE_USART     0
#endif
//   <e>USART auto configuration
// ===============================
//
//   <i> If enabled, extends the initialize function so all configuration steps are performed at once.
//   <i> Drivers are also powered up during initialization if enabled.
//   <i> Default: enable
#ifndef RTE_USART_CFG_EN_DEFAULT
 #define RTE_USART_CFG_EN_DEFAULT   1
#endif
//   <o>Baudrate
//       <4800=> 4800     <9600=> 9600
//       <19200=> 19200   <38400=> 38400
//       <57600=> 57600   <115200=> 115200
//       <230400=> 230400 <460800=> 460800
//   <i> Defines the usart baudrate.
//   <i> Default: 115200
#ifndef RTE_USART0_BAUDRATE_DEFAULT
 #define RTE_USART0_BAUDRATE_DEFAULT   115200
#endif
// </e>
// <e>DMA control
// ===============================
//
// <i> Enables dma usage for rx / tx.
// <i> Default: 0
#ifndef RTE_USART0_DMA_EN_DEFAULT
 #define RTE_USART0_DMA_EN_DEFAULT       0
#endif
//   <o>RX dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 2  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer.
//   <i> Default: 3
#ifndef RTE_USART0_RX_DMA_CH_DEFAULT
 #define RTE_USART0_RX_DMA_CH_DEFAULT   3
#endif
//   <o>TX dma channel selection
//       <0x0=> 0  <0x1=> 1
//       <0x2=> 1  <0x3=> 3
//       <0x4=> 4  <0x5=> 5
//       <0x6=> 6  <0x7=> 7
//   <i> Defines which dma channel should be used for data transfer.
//   <i> Default: 4
#ifndef RTE_USART0_TX_DMA_CH_DEFAULT
 #define RTE_USART0_TX_DMA_CH_DEFAULT   4
#endif
// </e>
// <h>USART pins configuration
// ===============================
//
//   <o>RX data gpio pad
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as a rx data line.
//   <i> Default: 0
#ifndef RTE_USART0_RX_PIN_DEFAULT
 #define RTE_USART0_RX_PIN_DEFAULT  2
#endif
//   <o>TX data gpio pad
//       <0x0=> 0
//       <0x1=> 1
//       <0x2=> 2
//       <0x3=> 3
//       <0x4=> 4
//       <0x5=> 5
//       <0x6=> 6
//       <0x7=> 7
//       <0x8=> 8
//       <0x9=> 9
//       <0xA=> 10
//       <0xB=> 11
//       <0xC=> 12
//       <0xD=> 13
//       <0xE=> 14
//       <0xF=> 15
//   <i> Defines gpio pad number to be used as a tx data line.
//   <i> Default: 0
#ifndef RTE_USART0_TX_PIN_DEFAULT
 #define RTE_USART0_TX_PIN_DEFAULT  3
#endif
// </h>
// <h>Interrupt priority configuration
// ===============================
//
//   <o>Pre-empt priority <0-0x7>
//   <i> Defines the pre-empt priority
//   <i> Default: 0
#ifndef RTE_USART_INT_PREEMPT_PRI_DEFAULT
 #define RTE_USART_INT_PREEMPT_PRI_DEFAULT   1
#endif
//   <o>Subgroup priority <0-0x7>
//   <i> Defines the subgroup priority
//   <i> Default: 0
#ifndef RTE_USART_INT_SUBGRP_PRI_DEFAULT
 #define RTE_USART_INT_SUBGRP_PRI_DEFAULT   0
#endif
// </h>
// </e>
//------------- <<< end of configuration section >>> -----------------------

#if (RTE_GPIO)
#ifndef RTECFG_GPIO
	#error "ERROR - GPIO component not selected in RTE configuration"
#endif
#endif

#if (RTE_DMA)
#ifndef RTECFG_DMA
	#error "ERROR - DMA component not selected in RTE configuration"
#endif
#endif

#if (RTE_I2C)
#ifndef RTECFG_I2C
	#error "ERROR - I2C component not selected in RTE configuration"
#endif
#endif

#if (RTE_SAI)
#ifndef RTECFG_SAI
	#error "ERROR - SAI component not selected in RTE configuration"
#endif
#endif

#if (RTE_USART)
#ifndef RTECFG_USART
	#error "ERROR - USART component not selected in RTE configuration"
#endif
#endif

#endif    /* RTE_DEVICE_H */

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
//! \file HAL_I2C.h
//!
//! Contains wrapper functions for I2C CMSIS Driver that allow both
//! synchronous and asynchronous operation of the peripheral.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//! \addtogroup I2C_GRP I2C
//!
//! \brief I2C interface for communication with connected shields.
//!
//! By default the bus operates at standard bus speed (100kHz).
//! The application can increase the bus speed globally if all devices on the
//! bus support faster bus speeds and bus capacitance requirements are met.
//!
//! \warning
//! Due to hardware / CMSIS driver limitations it is not possible to reliably
//! generate repeated start condition on the bus.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#include "stdbool.h"
#include "stdint.h"

#include <Driver_I2C.h>
#include <HAL_error.h>

#ifdef __cplusplus
extern "C" {
#endif


enum HAL_I2C_BusSpeed
{
	HAL_I2C_BUS_SPEED_STANDARD,  /**< Standard Speed (100kHz) */
	HAL_I2C_BUS_SPEED_FAST,      /**< Fast Speed (400kHz) */
	HAL_I2C_BUS_SPEED_FAST_PLUS  /**< Fast+ Speed (1MHz) */
};

/** \brief Provides */
struct HAL_I2C_TransferData
{
    /** \brief 7-bit address of target device. */
	uint32_t addr;

	/** \brief Pointer to data array with read / written data. */
	uint8_t *data;

	/** \brief Actual number of bytes read / written. */
	uint32_t num;

	/** \brief Status of the transfer operation.
	 *
	 * Each bit represents specific event flag.
	 *
	 *
	 * \see [I2C Events](https://arm-software.github.io/CMSIS_5/Driver/html/group__I2C__events.html)
	 * in I2C CMSIS Driver specification for error flag definitions.
	 */
	uint32_t event;
};

/** \brief Prototype for user defined callback that will be called after
 * asynchronous I2C transaction is finished.
 *
 * This callback will be called from within I2C ISR.
 * Any data processing or time-consuming operations should not be handler
 * directly in ISR. See \ref BDK_TaskSchedule for off loading data processing
 * outside of ISR.
 *
 * \param t_data
 * Details about finished I2C transaction.
 */
typedef void (*HAL_I2C_Callback)(struct HAL_I2C_TransferData *t_data);

/** \brief Initializes I2C peripheral and configures DIO pins.
 *
 * \note This function is automatically called from all other HAL_I2C_*
 * functions and does not need to be called explicitly.
 *
 */
extern void HAL_I2C_Init(void);

/** \brief Powers off I2C peripheral and disables DIO pins.
 *
 * User selected bus speed setting is preserved between I2C peripheral power
 * cycles.
 */
extern void HAL_I2C_DeInit(void);

/** \brief Sets bus speed to given speed mode.
 *
 * \param speed
 * Desired I2C speed mode.
 * Use one of HAL_I2C_BUS_SPEED_[ STANDARD | FAST | FAST_PLUS ].
 *
 * \returns
 * HAL_OK - When new speed setting was applied successfully.<br>
 * HAL_ERROR_BUSY - If I2C peripheral is active.<br>
 * HAL_ERROR_* - If an error occurred.
 */
extern int32_t HAL_I2C_SetBusSpeed(const enum HAL_I2C_BusSpeed speed);

/** \brief Returns current I2C bus speed mode.
 *
 * \returns
 * One of HAL_I2C_BUS_SPEED_[ STANDARD | FAST | FAST_PLUS ]
 */
extern int32_t HAL_I2C_GetBusSpeed(void);

/** \brief Performs I2C read transaction.
 *
 * This function is blocking until I2C transaction is completed.
 *
 * \param addr
 * 7-bit I2C address of device.
 *
 * \param data
 * Pointer to data array where read data will be stored.
 * Needs to be at least \p num bytes long.
 *
 * \param num
 * Number of bytes to be read.
 *
 * \param xfer_pending
 * Whether an STOP condition should be generated after transaction is complete.
 *
 * \bug
 * Generating REPEATED START condition by setting of \p xfer_pending to `false`
 * is possible only when using standard bus speed.
 * At higher speeds a stop condition will be generated regardless of
 * \p xfer_pending value.
 */
extern int32_t HAL_I2C_Read(uint32_t addr,
		uint8_t *data,
		uint32_t num,
		bool xfer_pending);

/** \brief Performs an I2C read transaction asynchronously and calls
 * application defined callback when done.
 *
 * This function is not blocking and returns as soon as the I2C transaction is
 * started.
 *
 * \param addr
 * 7-bit I2C address of device.
 *
 * \param data
 * Pointer to data array where read data will be stored.
 * Needs to be at least \p num bytes long.
 * This pointer must remain valid until I2C transaction is finished.
 *
 * \param num
 * Number of bytes to be read.
 *
 * \param xfer_pending
 * Whether an STOP condition should be generated after transaction is complete.
 *
 * \bug
 * Generating REPEATED START condition by setting of \p xfer_pending to `false`
 * is possible only when using standard bus speed.
 * At higher speeds a stop condition will be generated regardless of
 * \p xfer_pending value.
 */
extern int32_t HAL_I2C_ReadAsync(uint32_t addr,
		uint8_t *data,
		uint32_t num,
		bool xfer_pending,
		HAL_I2C_Callback cb);

/** \brief Performs I2C write transaction.
 *
 * This function is blocking until I2C transaction is completed.
 *
 * \param addr
 * 7-bit I2C address of device.
 *
 * \param data
 * Pointer to data array with data that will be written to I2C device.
 * Needs to be at least \p num bytes long.
 *
 * \param num
 * Number of bytes to be write.
 *
 * \param xfer_pending
 * Whether an STOP condition should be generated after transaction is complete.
 *
 * \bug
 * Generating REPEATED START condition by setting of \p xfer_pending to `false`
 * is possible only when using standard bus speed.
 * At higher speeds a stop condition will be generated regardless of
 * \p xfer_pending value.
 */
extern int32_t HAL_I2C_Write(uint32_t addr,
		const uint8_t *data,
		uint32_t num,
		bool xfer_pending);

/** \brief Performs an I2C write transaction asynchronously and calls
 * application defined callback when done.
 *
 * This function is not blocking and returns as soon as the I2C transaction is
 * started.
 *
 * \param addr
 * 7-bit I2C address of device.
 *
 * \param data
 * Pointer to data array with data that will be written to I2C device.
 * Needs to be at least \p num bytes long.
 * This pointer must remain valid until I2C transaction is finished.
 *
 * \param num
 * Number of bytes to be written.
 *
 * \param xfer_pending
 * Whether an STOP condition should be generated after transaction is complete.
 *
 * \bug
 * Generating REPEATED START condition by setting of \p xfer_pending to `false`
 * is possible only when using standard bus speed.
 * At higher speeds a stop condition will be generated regardless of
 * \p xfer_pending value.
 */
extern int32_t HAL_I2C_WriteAsync(uint32_t addr,
		const uint8_t *data,
		uint32_t num,
		bool xfer_pending,
		HAL_I2C_Callback cb);

#ifdef __cplusplus
}
#endif

#endif /* HAL_I2C_H_ */

//! \}
//! \}

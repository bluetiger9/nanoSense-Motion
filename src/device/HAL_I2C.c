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
//! \file HAL_I2C.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//! \addtogroup I2C_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <HAL.h>
#include <I2C_RSLxx.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

struct _HAL_I2C_CONTROL
{
	uint32_t bus_speed : 2;  /**< \private */
	bool enabled : 1;  /**< \private */
	struct HAL_I2C_TransferData transfer_data;  /**< \private */
	HAL_I2C_Callback callback;  /**< \private */
}; /**< \private */

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

extern ARM_DRIVER_I2C Driver_I2C0; /**< \private */

static int32_t HAL_I2C_WaitForTransfer(void);  /**< \private */

/** \private
 * \brief Passes transaction result to user provided callback function.
 *
 * If a bus error event is detected during transfer the peripheral will be
 * powered off until next transaction request.
 *
 * \see HAL_I2C_ReadAsync
 * \see HAL_I2C_WriteAsync
 */
static void HAL_I2C_DriverCallback(uint32_t event);  /**< \private */


//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static struct _HAL_I2C_CONTROL ctrl = {
		ARM_I2C_BUS_SPEED_STANDARD,
		false,
		{0U, NULL, 0U, 0U},
		NULL,
};


//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void HAL_I2C_Init(void)
{
	int32_t retval;

	if (ctrl.enabled == true)
	{
		return;
	}

	ctrl.transfer_data.addr = 0U;
	ctrl.transfer_data.data = NULL;
	ctrl.transfer_data.num = 0U;
	ctrl.transfer_data.event = 0U;
	ctrl.callback = NULL;

	retval = Driver_I2C0.Initialize(&HAL_I2C_DriverCallback);
	ASSERT_DEBUG(retval == ARM_DRIVER_OK);

	retval = Driver_I2C0.PowerControl(ARM_POWER_FULL);
	ASSERT_DEBUG(retval == ARM_DRIVER_OK);

	retval = Driver_I2C0.Control(ARM_I2C_BUS_SPEED, ctrl.bus_speed);
	ASSERT_DEBUG(retval == ARM_DRIVER_OK);

	retval = Driver_I2C0.Control(ARM_I2C_BUS_CLEAR, 0);
	ASSERT_DEBUG(retval == ARM_DRIVER_OK);

	ctrl.enabled = true;
}

void HAL_I2C_DeInit(void)
{
	int32_t retval;

	retval = Driver_I2C0.PowerControl(ARM_POWER_OFF);
	ASSERT_DEBUG(retval == ARM_DRIVER_OK);

	retval = Driver_I2C0.Uninitialize();
	ASSERT_DEBUG(retval == ARM_DRIVER_OK);

	ctrl.enabled = false;
}

int32_t HAL_I2C_SetBusSpeed(const enum HAL_I2C_BusSpeed speed)
{
	HAL_I2C_Init();

	if (Driver_I2C0.GetStatus().busy != 0)
	{
	    return HAL_ERROR_BUSY;
	}

	switch (speed)
	{
	default:
		ctrl.bus_speed = ARM_I2C_BUS_SPEED_STANDARD;
		break;
	case HAL_I2C_BUS_SPEED_FAST:
		ctrl.bus_speed = ARM_I2C_BUS_SPEED_FAST;
		break;
	case HAL_I2C_BUS_SPEED_FAST_PLUS:
		ctrl.bus_speed = ARM_I2C_BUS_SPEED_FAST_PLUS;
		break;
	}

	return Driver_I2C0.Control(ARM_I2C_BUS_SPEED, ctrl.bus_speed);
}

int32_t HAL_I2C_GetBusSpeed(void)
{
    return ctrl.bus_speed;
}

int32_t HAL_I2C_Read(uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending)
{
	int32_t err = HAL_OK;

	ASSERT_ALWAYS(HAL_IsInterrupt() == false);

	while ((err = HAL_I2C_ReadAsync(addr, data, num, xfer_pending, NULL)) != HAL_OK)
	{
		if (err == HAL_ERROR_BUSY)
		{
			SYS_WAIT_FOR_INTERRUPT;
		}
		else
		{
			return err;
		}
	}

	return HAL_I2C_WaitForTransfer();
}

int32_t HAL_I2C_ReadAsync(uint32_t addr,
		uint8_t *data,
		uint32_t num,
		bool xfer_pending,
		HAL_I2C_Callback cb)
{
	HAL_I2C_Init();

	if (Driver_I2C0.GetStatus().busy != 0)
	{
		return HAL_ERROR_BUSY;
	}

	ctrl.transfer_data.addr = addr;
	ctrl.transfer_data.data = data;
	ctrl.transfer_data.num = 0U;
	ctrl.callback = cb;

	return Driver_I2C0.MasterReceive(addr, data, num, xfer_pending);
}

int32_t HAL_I2C_Write(uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending)
{
	int32_t err = HAL_OK;

	ASSERT_ALWAYS(HAL_IsInterrupt() == false);

	while ((err = HAL_I2C_WriteAsync(addr, data, num, xfer_pending, NULL)) != HAL_OK)
	{
		if (err == HAL_ERROR_BUSY)
		{
			SYS_WAIT_FOR_INTERRUPT;
		}
		else
		{
			return err;
		}
	}

	return HAL_I2C_WaitForTransfer();
}


int32_t HAL_I2C_WriteAsync(uint32_t addr,
		const uint8_t *data,
		uint32_t num,
		bool xfer_pending,
		HAL_I2C_Callback cb)
{
	HAL_I2C_Init();

	if (Driver_I2C0.GetStatus().busy != 0)
	{
		return HAL_ERROR_BUSY;
	}

	ctrl.transfer_data.addr = addr;
	ctrl.transfer_data.data = (uint8_t*) data;
	ctrl.transfer_data.num = 0U;
	ctrl.callback = cb;

	return Driver_I2C0.MasterTransmit(addr, data, num, xfer_pending);
}

static int32_t HAL_I2C_WaitForTransfer(void)
{
	ARM_I2C_STATUS status;

	while ((status = Driver_I2C0.GetStatus()).busy != 0)
	{
//		SYS_WAIT_FOR_INTERRUPT;
	}

	if (status.arbitration_lost)
	{
		return HAL_ERROR_I2C_ARBITRATION_LOST;
	}
	else if (Driver_I2C0.GetDataCount() == 0)
	{
		return HAL_ERROR_I2C_NACK;
	}
	else if (status.bus_error)
	{
		return HAL_ERROR_I2C_BUS_ERROR;
	}
	else
	{
		return HAL_OK;
	}
}

static void HAL_I2C_DriverCallback(uint32_t event)
{
	if (event & ARM_I2C_EVENT_BUS_ERROR)
	{
		HAL_I2C_DeInit();
	}

	if (ctrl.callback != NULL)
	{
	    /* data & addr were set up in the async call already */
	    ctrl.transfer_data.num = Driver_I2C0.GetDataCount();
		ctrl.transfer_data.event = event;
		ctrl.callback(&ctrl.transfer_data);
	}
}


//! }
//! }

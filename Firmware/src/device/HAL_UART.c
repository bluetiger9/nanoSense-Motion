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
//! \file HAL_UART.c
//!
//! Contains implementation of wrapper functions for USART CMSIS Driver that
//! allow both synchronous and asynchronous operation of the peripheral in UART
//! mode.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//! \addtogroup UART_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <HAL.h>
#include <USART_RSLxx.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

/** \brief Control structure for UART Wrapper.
 *
 * \private
 * */
struct _HAL_UART_CONTROL
{
    /** \brief Callback to be called after asynchronous TX operation is done. */
    HAL_UART_AsyncCallback tx_cb; /**< \private */

    /** \brief Callback to be called after asynchronous RX operation is done. */
    HAL_UART_AsyncCallback rx_cb; /**< \private */

    char* tx_data_ptr; /**< \private */
    char* rx_data_ptr; /**< \private */

    /** \brief Stores baud rate setting. */
    int32_t baud_rate; /**< \private */

    /** \brief Is UART initialized. */
    bool enabled :1; /**< \private */

    /** \brief Whether async TX is active. */
    bool async_tx :1; /**< \private */

    /** \brief Whether async RX is active. */
    bool async_rx :1; /**< \private */
};

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

/** \brief CMSIS Driver USART object.
 *
 * \private
 */
extern ARM_DRIVER_USART Driver_USART0;

/** \brief Custom HAL Wrapper callback called after RX / TX transfer is
 * finished.
 *
 *  \private
 */
static void HAL_UART_DriverCallback(uint32_t event);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

/** \private
 * \see _HAL_UART_CONTROL
 */
static struct _HAL_UART_CONTROL ctrl =
{
NULL,
NULL,
NULL,
NULL,
HAL_UART_DEFAULT_BAUDRATE,
false,
false,
false };

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void HAL_UART_Init(void)
{
    int32_t retval;

    if (ctrl.enabled == true)
    {
        return;
    }

    ctrl.tx_cb = NULL;
    ctrl.rx_cb = NULL;
    ctrl.tx_data_ptr = NULL;
    ctrl.rx_data_ptr = NULL;
    // ctrl.baud_rate; // do not change
    ctrl.enabled = false;
    ctrl.async_rx = false;
    ctrl.async_tx = false;

    retval = Driver_USART0.Initialize(&HAL_UART_DriverCallback);
    ASSERT_DEBUG(retval == ARM_DRIVER_OK);

    retval = Driver_USART0.PowerControl(ARM_POWER_FULL);
    ASSERT_DEBUG(retval == ARM_DRIVER_OK);

    retval = Driver_USART0.Control(ARM_USART_MODE_ASYNCHRONOUS, ctrl.baud_rate);
    ASSERT_DEBUG(retval == ARM_DRIVER_OK);

    ctrl.enabled = true;
}

void HAL_UART_DeInit(void)
{
    int32_t retval;

    if (ctrl.enabled == false)
    {
        return;
    }

    ctrl.enabled = false;

    retval = Driver_USART0.Uninitialize();
    ASSERT_DEBUG(retval == ARM_DRIVER_OK);

    retval = Driver_USART0.PowerControl(ARM_POWER_OFF);
    ASSERT_DEBUG(retval == ARM_DRIVER_OK);
}

int32_t HAL_UART_SetBaudRate(uint32_t baud)
{
    int32_t retval;

    HAL_UART_Init();

    if (HAL_UART_IsBusy())
    {
        return HAL_ERROR_BUSY;
    }

    retval = Driver_USART0.Control(ARM_USART_MODE_ASYNCHRONOUS, baud);
    if (retval == HAL_OK)
    {
        ctrl.baud_rate = baud;
    }
    return retval;
}

uint32_t HAL_UART_GetBaudRate(void)
{
    return ctrl.baud_rate;
}

extern bool HAL_UART_IsBusy(void)
{
    ARM_USART_STATUS status = Driver_USART0.GetStatus();

    return status.rx_busy || status.tx_busy;
}

int32_t HAL_UART_Receive(char *data, uint32_t num, uint32_t timeout)
{
    int32_t retval = 0;
    ARM_USART_STATUS status;
    uint32_t time_start;

    ASSERT_ALWAYS(HAL_IsInterrupt() == false);
    ASSERT_DEBUG(data != NULL);
    ASSERT_DEBUG(num > 0);

    HAL_UART_Init();

    time_start = HAL_Time();
    do
    {
        retval = Driver_USART0.Receive(data, num);
        switch (retval)
        {
        case ARM_DRIVER_OK:
            ctrl.async_rx = false;
            break;
        case ARM_DRIVER_ERROR_BUSY:
            SYS_WAIT_FOR_INTERRUPT;
            break;
        default:
            return retval;
        }
    } while (retval != ARM_DRIVER_OK);

    for (;;)
    {
        status = Driver_USART0.GetStatus();

        if (status.rx_busy)
        {
            if (timeout != HAL_UART_WAIT_FOREVER
                    && (HAL_Time() - time_start) >= timeout)
            {
                int32_t bytes_received = Driver_USART0.GetRxCount();
                retval = Driver_USART0.Control(ARM_USART_ABORT_RECEIVE, 0);
                if (retval != ARM_DRIVER_OK)
                {
                    return retval;
                }
                else
                {
                    return bytes_received;
                }
            }
            SYS_WAIT_FOR_INTERRUPT;
        }
        else
        {
            return num;
        }
    }

    ASSERT_DEBUG(1); // Unreachable
    return HAL_ERROR;
}

int32_t HAL_UART_Send(const char *data, uint32_t num)
{
    int32_t retval = HAL_OK;
    ARM_USART_STATUS status;

    ASSERT_ALWAYS(HAL_IsInterrupt() == false);
    ASSERT_DEBUG(data != NULL);
    ASSERT_DEBUG(num > 0);

    HAL_UART_Init();

    do
    {
        retval = Driver_USART0.Send(data, num);
        switch (retval)
        {
        case ARM_DRIVER_OK:
            ctrl.async_tx = false;
            break;
        case ARM_DRIVER_ERROR_BUSY:
//            SYS_WAIT_FOR_INTERRUPT;
            break;
        default:
            return retval;
        }
    } while (retval != ARM_DRIVER_OK);

    for (;;)
    {
        status = Driver_USART0.GetStatus();

        if (status.tx_busy)
        {
//            __WFI();
        }
        else
        {
            return HAL_OK;
        }
    }

    ASSERT_DEBUG(1); // Unreachable
    return HAL_ERROR;
}

int32_t HAL_UART_ReceiveAsync(char *data, uint32_t num,
        HAL_UART_AsyncCallback cb)
{
    int32_t retval;

    ASSERT_DEBUG(data != NULL);
    ASSERT_DEBUG(num > 0);

    HAL_UART_Init();

    retval = Driver_USART0.Receive(data, num);
    if (retval == ARM_DRIVER_OK)
    {
        ctrl.rx_data_ptr = data;
        ctrl.rx_cb = cb;

        ctrl.async_rx = true;
    }

    return retval;
}

int32_t HAL_UART_ReceiveAsyncAbort(bool execute_cb)
{
    if (ctrl.async_rx == true)
    {
        Driver_USART0.Control(ARM_USART_ABORT_RECEIVE, 0);
        ctrl.async_rx = false;

        if (execute_cb == true && ctrl.rx_cb != NULL)
        {
            ctrl.rx_cb(ctrl.rx_data_ptr, Driver_USART0.GetRxCount());
        }

        return Driver_USART0.GetRxCount();
    }
    else
    {
        return HAL_ERROR_SPECIFIC;
    }
}

int32_t HAL_UART_SendAsync(const char *data, uint32_t num,
        HAL_UART_AsyncCallback cb)
{
    int32_t retval;

    ASSERT_DEBUG(data != NULL);
    ASSERT_DEBUG(num > 0);

    HAL_UART_Init();

    retval = Driver_USART0.Send(data, num);
    if (retval == ARM_DRIVER_OK)
    {
        ctrl.tx_cb = cb;
        ctrl.tx_data_ptr = (char*) data;

        ctrl.async_tx = true;
    }

    return retval;
}

static void HAL_UART_DriverCallback(uint32_t event)
{
    if ((event & ARM_USART_EVENT_RECEIVE_COMPLETE) && ctrl.async_rx == true)
    {
        ctrl.async_rx = false;

        // Call application callback after receive operation is finished.
        if (ctrl.rx_cb != NULL)
        {

            ctrl.rx_cb(ctrl.rx_data_ptr, Driver_USART0.GetRxCount());
        }
    }

    if ((event & ARM_USART_EVENT_SEND_COMPLETE) && ctrl.async_tx == true)
    {
        ctrl.async_tx = false;

        // Call application callback after send operation is finished.
        if (ctrl.tx_cb != NULL)
        {
            ctrl.tx_cb(ctrl.tx_data_ptr, Driver_USART0.GetTxCount());
        }
    }
}

//! \}
//! \}

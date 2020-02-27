/* ----------------------------------------------------------------------------
 * Copyright (c) 2018 Semiconductor Components Industries, LLC (d/b/a
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
 * USART_RSLXX.c
 * - CMSIS-Driver implementation for RSLXX UART
 * ----------------------------------------------------------------------------
 * $Revision: 1.1 $
 * $Date: 2018/03/15 21:34:38 $
 * ------------------------------------------------------------------------- */
#include <USART_RSLxx.h>

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion =
{
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

#if RTE_USART0

void UART_TX_IRQHandler(void);
void UART_RX_IRQHandler(void);

/* USART0 Run-Time Information */
static USART_INFO USART0_Info = { 0 };
static USART_TRANSFER_INFO USART0_TransferInfo = { 0 };

/* USART0 Resources */
static const USART_RESOURCES USART0_Resources =
{
    {    /* Capabilities */
        1,    /* supports UART (Asynchronous) mode */
        0,    /* supports Synchronous Master mode */
        0,    /* supports Synchronous Slave mode */
        0,    /* supports UART Single-wire mode */
        0,    /* supports UART IrDA mode */
        0,    /* supports UART Smart Card mode */
        0,    /* Smart Card Clock generator available */
        0,    /* RTS Flow Control available */
        0,    /* CTS Flow Control available */
        0,    /* Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE */
        0,    /* Signal rx char timeout evt: \ref ARM_USART_EVENT_RX_TIMEOUT */
        0,    /* RTS Line: 0=not available, 1=available */
        0,    /* CTS Line: 0=not available, 1=available */
        0,    /* DTR Line: 0=not available, 1=available */
        0,    /* DSR Line: 0=not available, 1=available */
        0,    /* DCD Line: 0=not available, 1=available */
        0,    /* RI Line: 0=not available, 1=available */
        0,    /* Signal CTS change event: \ref ARM_USART_EVENT_CTS */
        0,    /* Signal DSR change event: \ref ARM_USART_EVENT_DSR */
        0,    /* Signal DCD change event: \ref ARM_USART_EVENT_DCD */
        0    /* Signal RI change event: \ref ARM_USART_EVENT_RI */
    },
    UART,

    /* USART Pins */
    {
        PIN_UART_TX,
        PIN_UART_RX
    },
    UART_RX_IRQn,
    UART_TX_IRQn,
    &USART0_Info,
    &USART0_TransferInfo
};

#endif    /* if RTE_USART0 */

/* ----------------------------------------------------------------------------
 * Function      : void USART_TX_IRQHandler(USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Called by hardware ISR when a byte is transmitted. Place
 *                 transmit data into the UART, sequentially, byte per byte.
 *                 Manage flags and counters. Application is notified via
 *                 callback function once the transmission is complete
 * Inputs        : usart    - USART instance
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static void USART_TX_IRQHandler(USART_RESOURCES *usart)
{
    if (usart->info->status.tx_busy)
    {
        /* Increment tx count */
        usart->xfer->tx_cnt++;

        /* Check if transfer is complete */
        if (usart->xfer->tx_cnt == usart->xfer->tx_num)
        {
            /* Clear send active flag */
            usart->info->status.tx_busy = 0U;

            /* Disable UART TX interrupt */
            NVIC_DisableIRQ(usart->uart_tx_irqn);

            /* Notify application */
            if (usart->info->cb_event)
            {
                usart->info->cb_event(ARM_USART_EVENT_SEND_COMPLETE);
            }
        }
        else    /* Transfer next byte */
        {
            usart->reg->TX_DATA = usart->xfer->tx_buf[usart->xfer->tx_cnt];
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_RX_IRQHandler(USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Called by hardware ISR when a byte is received. Store
 *                 received data in the buffer, manage flags and counters.
 *                 Application is notified via callback function once the
 *                 receive is complete
 * Inputs        : usart    - USART instance
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static void USART_RX_IRQHandler(USART_RESOURCES *usart)
{
    if (usart->info->status.rx_busy && (usart->xfer->rx_cnt < usart->xfer->rx_num))
    {
        /* Receive byte */
        usart->xfer->rx_buf[usart->xfer->rx_cnt++] = usart->reg->RX_DATA;

        /* Check if receive is complete */
        if (usart->xfer->rx_cnt == usart->xfer->rx_num)
        {
            /* Clear rx busy flag */
            usart->info->status.rx_busy = 0U;

            /* Disable UART TX interrupt */
            NVIC_DisableIRQ(usart->uart_rx_irqn);

            /* Notify application */
            if (usart->info->cb_event)
            {
                usart->info->cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
            }
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void USARTx_GetVersion(void)
 * ----------------------------------------------------------------------------
 * Description   : Driver version
 * Inputs        : None
 * Outputs       : ARM_DRIVER_VERSION
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_DRIVER_VERSION USARTx_GetVersion(void)
{
    return DriverVersion;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_GetCapabilities(USART_RESOURCES* usart)
 * ----------------------------------------------------------------------------
 * Description   : Get USARTX driver capabilities
 * Inputs        : usart    - USART instance
 * Outputs       : ARM_USART_CAPABILITIES
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_USART_CAPABILITIES USART_GetCapabilities(USART_RESOURCES *usart)
{
    return usart->capabilities;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_Initialize(ARM_USART_SignalEvent_t cb_event,
 *                                       USART_RESOURCES* usart)
 * ----------------------------------------------------------------------------
 * Description   : Initialize USART flags, GPIO pins and reference to
 *                 callback function.
 * Inputs        : cb_event - pointer to callback function (optional)
 *                 usart    - USART instance
 * Outputs       : ARM_DRIVER_OK
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t USART_Initialize(ARM_USART_SignalEvent_t cb_event,
                                USART_RESOURCES *usart)
{
    if (usart->info->flags & USART_FLAG_INITIALIZED)
    {
        /* Driver is already initialized */
        return ARM_DRIVER_OK;
    }

    /* Initialize USART Run-time Resources */
    usart->info->cb_event = cb_event;

    /* Clear transfer information */
    memset(usart->xfer, 0, sizeof(USART_TRANSFER_INFO));

    /* Configure UART GPIO pins */
    Sys_UART_DIOConfig(DIO_LPF_DISABLE | DIO_6X_DRIVE | DIO_WEAK_PULL_UP,
                       usart->pins.tx, usart->pins.rx);

    usart->info->flags = USART_FLAG_INITIALIZED;

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_Uninitialize(USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Uninitialize USART flags, GPIO pins and removes reference to
 *                 callback function.
 * Inputs        : usart - USART instance
 * Outputs       : ARM_DRIVER_OK
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t USART_Uninitialize(USART_RESOURCES *usart)
{
    /* Reset TX pin configuration */
    DIO->CFG[usart->pins.tx] = DIO_MODE_DISABLE;

    /* Reset RX pin configuration */
    DIO->CFG[usart->pins.rx] = DIO_MODE_DISABLE;

    /* Reset DIO_UART_SRC configuration */
    DIO->UART_SRC = UART_RX_SRC_CONST_HIGH;

    /* Reset USART status flags */
    usart->info->flags = 0U;

    /* Remove reference to callback function */
    usart->info->cb_event = NULL;

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_PowerControl(ARM_POWER_STATE state,
 *                                         USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Operates the power modes of the USART interface
 * Inputs        : state - ARM_POWER_FULL or ARM_POWER_OFF
 *                 usart - USART instance
 * Outputs       : ARM_DRIVER_OK - if the operation is successful
 *                 ARM_DRIVER_ERROR_UNSUPPORTED - if argument is ARM_POWER_LOW
 *                                                or an invalid value
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t USART_PowerControl(ARM_POWER_STATE state,
                                  USART_RESOURCES *usart)
{
    switch (state)
    {
        case ARM_POWER_OFF:
        {
            /* Disable UART interrupts */
            NVIC_DisableIRQ(usart->uart_rx_irqn);
            NVIC_DisableIRQ(usart->uart_tx_irqn);

            /* Clear pending USART interrupts in NVIC */
            NVIC_ClearPendingIRQ(usart->uart_rx_irqn);
            NVIC_ClearPendingIRQ(usart->uart_tx_irqn);

            /* Disable UART */
            Sys_UART_Disable();

            /* Clear flags */
            usart->info->status.tx_busy          = 0U;
            usart->info->status.rx_busy          = 0U;
            usart->info->flags &= ~USART_FLAG_POWERED;
        }
        break;

        case ARM_POWER_LOW:
        {
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }

        case ARM_POWER_FULL:
        {
            if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U)
            {
                return ARM_DRIVER_ERROR;
            }
            if ((usart->info->flags & USART_FLAG_POWERED) != 0U)
            {
                return ARM_DRIVER_OK;
            }

            /* Clear Status flags */
            usart->info->status.tx_busy = 0U;
            usart->info->status.rx_busy = 0U;

            /* Set powered flag */
            usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;
        }
        break;

        default:
            return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_Control(uint32_t control, uint32_t arg,
 *                                    USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Set mode or execute operation for USART0. Examples:
 *                 ARM_USART_ABORT_RECEIVE, ARM_USART_ABORT_SEND or
 *                 ARM_USART_MODE_ASYNCHRONOUS.
 * Inputs        : arg   - Argument of operation (optional)
 *                 usart - USART instance
 * Outputs       : Execution status
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t USART_Control(uint32_t control, uint32_t arg,
                             USART_RESOURCES *usart)
{
    if ((usart->info->flags & USART_FLAG_POWERED) == 0U)
    {
        /* USART not powered */
        return ARM_DRIVER_ERROR;
    }

    switch (control & ARM_USART_CONTROL_Msk)
    {
        case ARM_USART_MODE_ASYNCHRONOUS:
        {
            Sys_UART_Enable(SystemCoreClock, arg, UART_DMA_MODE_DISABLE);
            usart->info->baudrate = arg;
        }
        break;

        case ARM_USART_ABORT_SEND:
        {
            /* Disable UART TX interrupt */
            NVIC_DisableIRQ(usart->uart_tx_irqn);

            /* Clear Send active flag */
            usart->info->status.tx_busy = 0U;
            return ARM_DRIVER_OK;
        }

        case ARM_USART_ABORT_RECEIVE:
        {
            /* Disable UART RX interrupt */
            NVIC_DisableIRQ(usart->uart_rx_irqn);

            /* Clear RX busy flag */
            usart->info->status.rx_busy = 0U;
            return ARM_DRIVER_OK;
        }

        default:    /* Unsupported operation */
            return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    /* Check USART Data bits */
    if ((control & ARM_USART_DATA_BITS_Msk) != ARM_USART_DATA_BITS_8)
    {
        return ARM_USART_ERROR_DATA_BITS;
    }

    /* Check USART Parity */
    if ((control & ARM_USART_PARITY_Msk) != ARM_USART_PARITY_NONE)
    {
        return ARM_USART_ERROR_PARITY;
    }

    /* Check USART Stop bits */
    if ((control & ARM_USART_STOP_BITS_Msk) != ARM_USART_STOP_BITS_1)
    {
        return ARM_USART_ERROR_STOP_BITS;
    }

    /* Check Flow Control */
    if ((control & ARM_USART_FLOW_CONTROL_Msk) != ARM_USART_FLOW_CONTROL_NONE)
    {
        return ARM_USART_ERROR_FLOW_CONTROL;
    }

    /* Check Polarity */
    if ((control & ARM_USART_CPOL_Msk) != ARM_USART_CPOL0)
    {
        return ARM_USART_ERROR_CPOL;
    }

    /* Check Clock Phase */
    if ((control & ARM_USART_CPHA_Msk) != ARM_USART_CPHA0)
    {
        return ARM_USART_ERROR_CPHA;
    }

    /* Set configured flag */
    usart->info->flags |= USART_FLAG_CONFIGURED;

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t USART_Send(const void *data, uint32_t num,
 *                                    USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Non-blocking send function. If a callback was previously
 *                 registered using the Initialize function, the caller is
 *                 notified when 'num' bytes are sent.
 * Inputs        : data                  - pointer to data buffer
 *                 num                   - number of bytes to send
 *                 usart                 - USART instance
 * Outputs       : ARM_DRIVER_OK         - if it can successfully start the
 *                                         send operation
 *                 ARM_DRIVER_ERROR_BUSY - if it's already busy handling
 *                                         other send operation
 * Assumptions   : The data buffer argument points to a memory area with 'num'
 *                 bytes of size
 * ------------------------------------------------------------------------- */
static int32_t USART_Send(const void *data, uint32_t num,
                          USART_RESOURCES *usart)
{
    if ((data == NULL) || (num == 0U))
    {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U)
    {
        /* USART is not configured (mode not selected) */
        return ARM_DRIVER_ERROR;
    }

    if (usart->info->status.tx_busy != 0U)
    {
        /* Send is not completed yet */
        return ARM_DRIVER_ERROR_BUSY;
    }

    /* Set Send active flag */
    usart->info->status.tx_busy = 1U;

    /* Save transmit buffer info */
    usart->xfer->tx_buf = (uint8_t *)data;
    usart->xfer->tx_num = num;
    usart->xfer->tx_cnt = 0U;

    /* Enable UART TX interrupt */
    NVIC_ClearPendingIRQ(usart->uart_tx_irqn);
    NVIC_EnableIRQ(usart->uart_tx_irqn);

    /* Transmit first byte (remaining will be transmitted
     * in interrupt handler) */
    usart->reg->TX_DATA = usart->xfer->tx_buf[usart->xfer->tx_cnt];

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_Receive(void *data, uint32_t num,
 *                                    USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Non-blocking receive function. If a callback was previously
 *                 registered using the Initialize function, the caller is
 *                 notified when 'num' bytes are received.
 * Inputs        : data                  - pointer to data buffer
 *                 num                   - number of bytes to receive
 *                 usart                 - USART instance
 * Outputs       : ARM_DRIVER_OK         - if it can successfully start the
 *                                         receive operation
 *                 ARM_DRIVER_ERROR_BUSY - if it's already busy handling
 *                                         other receive operation
 * Assumptions   : The data buffer argument points to a memory area with at
 *                 least 'num' bytes of available space
 * ------------------------------------------------------------------------- */
static int32_t USART_Receive(void *data, uint32_t num, USART_RESOURCES *usart)
{
    if ((data == NULL) || (num == 0U))
    {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U)
    {
        /* USART is not configured (mode not selected) */
        return ARM_DRIVER_ERROR;
    }

    if (usart->info->status.rx_busy == 1U)
    {
        /* Receiver is busy */
        return ARM_DRIVER_ERROR_BUSY;
    }

    /* Set RX busy flag */
    usart->info->status.rx_busy = 1U;

    /* Save number of data to be received */
    usart->xfer->rx_num = num;

    /* Save receive buffer info */
    usart->xfer->rx_buf = (uint8_t *)data;
    usart->xfer->rx_cnt = 0U;

    /* Enable UART RX interrupt */
    NVIC_ClearPendingIRQ(usart->uart_rx_irqn);
    NVIC_EnableIRQ(usart->uart_rx_irqn);

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_GetTxCount(USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Get USARTX TX count. This count is reset for every send
 *                 operation.
 * Inputs        : usart    - USART instance
 * Outputs       : Number of bytes transmitted by USARTX
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static uint32_t USART_GetTxCount(USART_RESOURCES *usart)
{
    return usart->xfer->tx_cnt;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_GetRxCount(USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Get USARTX RX count. This count is reset for every receive
 *                 operation.
 * Inputs        : usart    - USART instance
 * Outputs       : Number of bytes received by USARTX
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static uint32_t USART_GetRxCount(USART_RESOURCES *usart)
{
    return usart->xfer->rx_cnt;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_GetStatus(USART_RESOURCES *usart)
 * ----------------------------------------------------------------------------
 * Description   : Get USARTX status
 * Inputs        : usart    - USART instance
 * Outputs       : Return USARTX status as an ARM_USART_STATUS structure
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_USART_STATUS USART_GetStatus(USART_RESOURCES *usart)
{
    ARM_USART_STATUS status;

    status.tx_busy          = usart->info->status.tx_busy;
    status.rx_busy          = usart->info->status.rx_busy;
    status.rx_overflow      = 0U;
    status.tx_underflow     = 0U;
    status.rx_break         = 0U;
    status.rx_framing_error = 0U;
    status.rx_parity_error  = 0U;

    return status;
}

/* ----------------------------------------------------------------------------
 * Function      : void USART_GetModemStatus(void)
 * ----------------------------------------------------------------------------
 * Description   : Not supported
 * Inputs        : None
 * Outputs       : Return status as 0
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_USART_MODEM_STATUS USART_GetModemStatus(USART_RESOURCES *usart)
{
    ARM_USART_MODEM_STATUS modem_status;

    modem_status.cts = 0U;
    modem_status.dsr = 0U;
    modem_status.ri  = 0U;
    modem_status.dcd = 0U;

    return modem_status;
}

#if RTE_USART0

/* USART0 Driver Wrapper functions. See functions above for usage */
static ARM_USART_CAPABILITIES USART0_GetCapabilities(void)
{
    return USART_GetCapabilities(&USART0_Resources);
}

static int32_t USART0_Initialize(ARM_USART_SignalEvent_t cb_event)
{
    return USART_Initialize(cb_event, &USART0_Resources);
}

static int32_t USART0_Uninitialize(void)
{
    return USART_Uninitialize(&USART0_Resources);
}

static int32_t USART0_PowerControl(ARM_POWER_STATE state)
{
    return USART_PowerControl(state, &USART0_Resources);
}

static int32_t USART0_Send(const void *data, uint32_t num)
{
    return USART_Send(data, num, &USART0_Resources);
}

static int32_t USART0_Receive(void *data, uint32_t num)
{
    return USART_Receive(data, num, &USART0_Resources);
}

/* ----------------------------------------------------------------------------
 * Function      : void USART0_Transfer(const void *data_out, void *data_in,
 *                                      uint32_t num)
 * ----------------------------------------------------------------------------
 * Description   : Not supported
 * Inputs        : None
 * Outputs       : ARM_DRIVER_ERROR_UNSUPPORTED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t USART0_Transfer(const void *data_out, void *data_in, uint32_t num)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static uint32_t USART0_GetTxCount(void)
{
    return USART_GetTxCount(&USART0_Resources);
}

static uint32_t USART0_GetRxCount(void)
{
    return USART_GetRxCount(&USART0_Resources);
}

static int32_t USART0_Control(uint32_t control, uint32_t arg)
{
    return USART_Control(control, arg, &USART0_Resources);
}

static ARM_USART_STATUS USART0_GetStatus(void)
{
    return USART_GetStatus(&USART0_Resources);
}

/* ----------------------------------------------------------------------------
 * Function      : void USART0_SetModemControl(ARM_USART_MODEM_CONTROL control)
 * ----------------------------------------------------------------------------
 * Description   : Not supported
 * Inputs        : None
 * Outputs       : ARM_DRIVER_ERROR_UNSUPPORTED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t USART0_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_USART_MODEM_STATUS USART0_GetModemStatus(void)
{
    return USART_GetModemStatus(&USART0_Resources);
}

void UART_TX_IRQHandler(void)
{
    USART_TX_IRQHandler(&USART0_Resources);
}

void UART_RX_IRQHandler(void)
{
    USART_RX_IRQHandler(&USART0_Resources);
}

/* USART0 Driver Control Block */
ARM_DRIVER_USART Driver_USART0 =
{
    USARTx_GetVersion,
    USART0_GetCapabilities,
    USART0_Initialize,
    USART0_Uninitialize,
    USART0_PowerControl,
    USART0_Send,
    USART0_Receive,
    USART0_Transfer,
    USART0_GetTxCount,
    USART0_GetRxCount,
    USART0_Control,
    USART0_GetStatus,
    USART0_SetModemControl,
    USART0_GetModemStatus
};
#endif    /* if RTE_USART0 */

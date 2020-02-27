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
 * I2C_RSLxx.c
 * - I2C CMSIS-Driver implementation for RSLxx family of devices
 * ----------------------------------------------------------------------------
 * $Revision: 1.9 $
 * $Date: 2018/03/14 17:38:14 $
 * ------------------------------------------------------------------------- */
#include <I2C_RSLxx.h>

#define ARM_I2C_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion =
{
    ARM_I2C_API_VERSION,
    ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
/* 10-bit addresses not supported */
static const ARM_I2C_CAPABILITIES DriverCapabilities = { 0U };

#if RTE_I2C0

void I2C_IRQHandler(void);

/* I2C0 Run-Time Information */
static I2C_INFO I2C0_Info = { 0U };
static I2C_TRANSFER_INFO I2C0_TransferInfo = { 0U };
static I2C_PINS I2C0_Pins = {
        PIN_I2C_SCK,
        PIN_I2C_SDA,
        (DIO_LPF_ENABLE | DIO_6X_DRIVE | DIO_NO_PULL)
};

/* I2C0 Resources */
static const I2C_RESOURCES I2C0_Resources =
{
    (I2C_Type *)I2C,
    &I2C0_Pins,
    I2C_IRQn,
    &I2C0_Info,
    &I2C0_TransferInfo
};
#endif    /* RTE_I2C0 */

/* ----------------------------------------------------------------------------
 * Function      : void I2Cx_MasterIRQHandler (const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Called by hardware ISR when a byte is transferred.
 * Inputs        : i2c    - Pointer to I2C resources
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static void I2Cx_MasterIRQHandler (const I2C_RESOURCES *i2c)
{
    uint32_t event = 0U;
    uint32_t i2c_status = Sys_I2C_Get_Status();

    if (i2c->info->status.busy)
    {
        if (i2c_status & I2C_BUS_ERROR)
        {
            i2c->info->status.bus_error = true;
            event |= ARM_I2C_EVENT_BUS_ERROR | ARM_I2C_EVENT_ARBITRATION_LOST;

            /* Reset the peripheral to a known state */
            Sys_I2C_Reset();
        }
        else if (i2c_status & I2C_STOP_DETECTED)
        {
            /* Stop detected, finish transfer */
            if (i2c->xfer->cnt == i2c->xfer->num)
            {
                event |= ARM_I2C_EVENT_TRANSFER_DONE;
            }
            else
            {
                event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;

                if (i2c->xfer->cnt == 0)
                {
                    event |= ARM_I2C_EVENT_ADDRESS_NACK;
                }
            }
            i2c->info->status.busy = 0U;
        }
        else if (i2c_status & I2C_IS_READ)
        {
            /* READ mode, If buffer full put a new data on RX buffer. When receive
             * the number of byte expected, send NACK and Stop. */
            if (i2c_status & I2C_BUFFER_FULL)
            {
                if (i2c->xfer->cnt < (i2c->xfer->num - 1))
                {
                    Sys_I2C_ACK();
                }
                else
                {
                    Sys_I2C_NACKAndStop();
                }
                i2c->xfer->data[i2c->xfer->cnt++] = I2C->DATA;
            }

            /* If Data Event is set send an ACK to start the read */
            else if (i2c_status & I2C_DATA_EVENT)
            {
                Sys_I2C_ACK();
            }
        }
        else
        {
            /* If WRITE mode, send the next byte.
             * If all bytes from the buffer are sent, set LAST_DATA bit */
            if ((i2c_status & (1 << I2C_STATUS_ACK_STATUS_Pos)) == I2C_HAS_ACK)
            {
                if (i2c->xfer->cnt < i2c->xfer->num)
                {
                    I2C->DATA = i2c->xfer->data[i2c->xfer->cnt++];
                }

                if (i2c->xfer->cnt == i2c->xfer->num)
                {
                    if (i2c->xfer->pending == true)
                    {
                        Sys_I2C_ACK();

                        event |= ARM_I2C_EVENT_TRANSFER_DONE;
                        i2c->info->status.busy = 0U;
                    }
                    else
                    {
                        Sys_I2C_LastData();
                    }
                }
            }
        }
    }

    /* If an event happened and the application registered a callback */
    if (event && i2c->info->cb_event)
    {
        i2c->info->cb_event(event);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void I2Cx_SlaveIRQHandler (const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Called by hardware ISR when a byte is transferred.
 * Inputs        : i2c    - Pointer to I2C resources
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static void I2Cx_SlaveIRQHandler (const I2C_RESOURCES *i2c)
{
    uint32_t event = 0U;
    uint32_t i2c_status = Sys_I2C_Get_Status();

    if (i2c->info->status.busy)
    {
        if (i2c_status & I2C_BUS_ERROR)
        {
            i2c->info->status.bus_error = true;
            event |= ARM_I2C_EVENT_BUS_ERROR;

            /* Reset the peripheral to a known state */
            Sys_I2C_Reset();
        }
        else if (i2c_status & I2C_DATA_IS_ADDR)
        {
            /* If direction matches with the operation the application is waiting for */
            if ((i2c_status & (1 << I2C_STATUS_READ_WRITE_Pos)) ^
                (i2c->info->status.direction << I2C_STATUS_READ_WRITE_Pos))
            {
                /* Slave I2C selected. Send an ACK and reset the counter */
                Sys_I2C_ACK();

                /* If general call, notify application */
                if (i2c_status & I2C_ADDR_GEN_CALL)
                {
                    event |= ARM_I2C_EVENT_GENERAL_CALL;
                    i2c->info->status.general_call = true;
                }

                i2c->xfer->cnt = 0;

                event |= (i2c_status & I2C_IS_READ) ? ARM_I2C_EVENT_SLAVE_TRANSMIT : ARM_I2C_EVENT_SLAVE_RECEIVE;
            }
            else
            {
                Sys_I2C_NACK();
            }
        }
        else if (i2c_status & I2C_DATA_EVENT)
        {
            /* Data event:
             *  - When in write mode send an ACK and read data written by Master.
             *  - When in read mode and NACK status not set send a new byte. */

            if (i2c->xfer->cnt < i2c->xfer->num)
            {
                if ((i2c_status & (1 << I2C_STATUS_READ_WRITE_Pos)) == I2C_IS_WRITE)
                {
                    /* Send ACK */
                    Sys_I2C_ACK();

                    /* Fill buffer with received data */
                    i2c->xfer->data[i2c->xfer->cnt++] = I2C->DATA;
                }
                else if ((i2c_status & (1 << I2C_STATUS_ACK_STATUS_Pos)) == I2C_HAS_ACK)
                {
                    /* Send next byte */
                    I2C->DATA = i2c->xfer->data[i2c->xfer->cnt++];
                }
            }
            else
            {
                /* NACK and notify transfer done */
                Sys_I2C_NACK();
                event |= ARM_I2C_EVENT_TRANSFER_DONE;
                i2c->info->status.busy = 0U;
            }
        }
        else if (i2c_status & I2C_STOP_DETECTED)
        {
            /* Stop detected, finish transfer */
            if (i2c->xfer->cnt == i2c->xfer->num)
            {
                event |= ARM_I2C_EVENT_TRANSFER_DONE;
            }
            else
            {
                event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE | ARM_I2C_EVENT_BUS_ERROR;
            }

            i2c->info->status.busy = 0U;
        }
    }

    /* If an event happened and the application registered a callback */
    if (event && i2c->info->cb_event)
    {
        i2c->info->cb_event(event);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void I2Cx_GetVersion (void)
 * ----------------------------------------------------------------------------
 * Description   : Driver version
 * Inputs        : None
 * Outputs       : ARM_DRIVER_VERSION
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_DRIVER_VERSION I2Cx_GetVersion (void)
{
    return DriverVersion;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2Cx_GetCapabilities (void)
 * ----------------------------------------------------------------------------
 * Description   : Get I2Cx driver capabilities
 * Inputs        : None
 * Outputs       : ARM_I2C_CAPABILITIES
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_I2C_CAPABILITIES I2Cx_GetCapabilities (void)
{
    return DriverCapabilities;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_Initialize (ARM_I2C_SignalEvent_t cb_event,
 *                                     const I2C_RESOURCES* i2c)
 * ----------------------------------------------------------------------------
 * Description   : Initialize I2C flags, GPIO pins and reference to
 *                 callback function.
 * Inputs        : cb_event - pointer to callback function (optional)
 *                 i2c      - Pointer to I2C resources
 * Outputs       : ARM_DRIVER_OK
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_Initialize (ARM_I2C_SignalEvent_t cb_event,
                               const I2C_RESOURCES *i2c)
{
    if (i2c->info->state & I2C_INITIALIZED)
    {
        /* Driver is already initialized */
        return ARM_DRIVER_OK;
    }

    /* Configure the DIOs for I2C operation */
    Sys_I2C_DIOConfig(i2c->pins->pin_cfg, i2c->pins->scl, i2c->pins->sda);

    /* Reset Run-Time information structure */
    memset(i2c->info, 0, sizeof(I2C_INFO));

    /* Initialize I2C Run-Time Resources */
    i2c->info->cb_event = cb_event;
    i2c->info->state    = I2C_INITIALIZED;

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_Uninitialize (const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Uninitialize I2C flags, GPIO pins and removes reference to
 *                 callback function.
 * Inputs        : i2c - Pointer to I2C resources
 * Outputs       : ARM_DRIVER_OK
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_Uninitialize (const I2C_RESOURCES *i2c)
{
    /* Reset pin configuration */
    DIO->CFG[i2c->pins->scl] = DIO_MODE_DISABLE;
    DIO->CFG[i2c->pins->sda] = DIO_MODE_DISABLE;

    /* Reset DIO_I2C_SRC configuration */
    DIO->I2C_SRC = (SDA_SRC_CONST_HIGH | SCL_SRC_CONST_HIGH);

    /* Clear I2C state */
    i2c->info->state = 0U;

    /* Remove reference to callback function */
    i2c->info->cb_event = NULL;

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_PowerControl (ARM_POWER_STATE state,
 *                                       const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Operates the power modes of the I2C interface
 * Inputs        : state - ARM_POWER_FULL or ARM_POWER_OFF
 *                 i2c - Pointer to I2C resources
 * Outputs       : ARM_DRIVER_OK - if the operation is successful
 *                 ARM_DRIVER_ERROR_UNSUPPORTED - if argument is ARM_POWER_LOW
 *                                                or an invalid value
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_PowerControl (ARM_POWER_STATE state,
                                 const I2C_RESOURCES *i2c)
{
    switch (state)
    {
        case ARM_POWER_OFF:
        {
            /* Disable I2C interrupts */
            NVIC_DisableIRQ(i2c->irqn);

            /* Clear pending I2C interrupts in NVIC */
            NVIC_ClearPendingIRQ(i2c->irqn);

            /* Disable I2C CLK */
            i2c->reg->CTRL0 = I2C_SAMPLE_CLK_DISABLE;

            /* Clear status flags */
            i2c->info->status = (const ARM_I2C_STATUS) {
                0
            };

            i2c->info->state &= ~I2C_POWERED;
        }
        break;

        case ARM_POWER_FULL:
        {
            if ((i2c->info->state & I2C_INITIALIZED) == 0U)
            {
                return ARM_DRIVER_ERROR;
            }
            if ((i2c->info->state & I2C_POWERED) != 0U)
            {
                return ARM_DRIVER_OK;
            }

            /* Enable I2C interrupt */
            NVIC_ClearPendingIRQ(i2c->irqn);
            NVIC_EnableIRQ(i2c->irqn);

            /* Enable I2C clock and configure it to be controlled by CM3 */
            i2c->reg->CTRL0 = (I2C_CONTROLLER_CM3 | I2C_SAMPLE_CLK_ENABLE
                               | I2C_AUTO_ACK_DISABLE | I2C_STOP_INT_ENABLE);

            /* Reset the peripheral to a known state */
            Sys_I2C_Reset();

            /* Set powered flag */
            i2c->info->state |= I2C_POWERED;
        }
        break;

        case ARM_POWER_LOW:
        default:
            return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t I2C_MasterTransmit (uint32_t addr,
 *                                             const uint8_t *data,
 *                                             uint32_t num, bool xfer_pending,
 *                                             const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Start transmitting data as I2C Master.
 * Inputs        : addr         - Slave address (7-bit)
 *                 data         - Pointer to data buffer to send to I2C Slave
 *                 num          - Number of data bytes to send
 *                 xfer_pending - Transfer operation is pending (Stop condition
 *                                will not be generated)
 *                 i2c          - Pointer to I2C resources
 * Outputs       : Execution status
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_MasterTransmit (uint32_t addr, const uint8_t *data,
                                   uint32_t num, bool xfer_pending,
                                   const I2C_RESOURCES *i2c)
{
    if ((data == NULL) || (num == 0U) ||
        ((addr & ~ARM_I2C_ADDRESS_GC) > 0x7FU))
    {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if ((i2c->info->state & I2C_CONFIGURED) == 0U)
    {
        /* Driver not yet configured */
        return ARM_DRIVER_ERROR;
    }

    if (i2c->info->status.busy)
    {
        /* Transfer operation in progress */
        return ARM_DRIVER_ERROR_BUSY;
    }

    /* Configure status */
    i2c->info->status = (const ARM_I2C_STATUS) {
        0
    };
    i2c->info->status.busy             = true;
    i2c->info->status.mode             = I2C_STATUS_MODE_MASTER;
    i2c->info->status.direction        = I2C_STATUS_DIRECTION_TX;

    /* Set transfer info */
    i2c->xfer->num     = num;
    i2c->xfer->cnt     = 0U;
    i2c->xfer->data    = (uint8_t *)data;
    i2c->xfer->addr    = (uint8_t)(addr);
    i2c->xfer->pending = xfer_pending;

    /* Disable slave mode and set clock prescaler */
    i2c->reg->CTRL0 = (((i2c->reg->CTRL0 & ~I2C_CTRL0_SPEED_Mask)   |
                        (i2c->info->prescale << I2C_CTRL0_SPEED_Pos)) &
                       ~I2C_SLAVE_ENABLE);

    /* Generate start condition for specified slave */
    Sys_I2C_StartWrite(i2c->xfer->addr);

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t I2C_MasterReceive (uint32_t addr,
 *                                            uint8_t *data,
 *                                            uint32_t num, bool xfer_pending,
 *                                            const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Start receiving data as I2C Master.
 * Inputs        : addr         - Slave address (7-bit)
 *                 data         - Pointer to data buffer to receive from I2C
 *                                Slave
 *                 num          - Number of data bytes to receive
 *                 xfer_pending - Transfer operation is pending (Stop condition
 *                                will not be generated)
 *                 i2c          - Pointer to I2C resources
 * Outputs       : Execution status
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_MasterReceive (uint32_t addr, uint8_t *data,
                                  uint32_t num, bool xfer_pending,
                                  const I2C_RESOURCES *i2c)
{
    if ((data == NULL) || (num == 0U) ||
        ((addr & ~ARM_I2C_ADDRESS_GC) > 0x7FU))
    {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if ((i2c->info->state & I2C_CONFIGURED) == 0U)
    {
        /* Driver not yet configured */
        return ARM_DRIVER_ERROR;
    }

    if (i2c->info->status.busy)
    {
        /* Transfer operation in progress */
        return ARM_DRIVER_ERROR_BUSY;
    }

    /* Reset status */
    i2c->info->status = (const ARM_I2C_STATUS) {
        0
    };
    i2c->info->status.busy             = true;
    i2c->info->status.mode             = I2C_STATUS_MODE_MASTER;
    i2c->info->status.direction        = I2C_STATUS_DIRECTION_RX;

    /* Set transfer info */
    i2c->xfer->num     = num;
    i2c->xfer->cnt     = 0U;
    i2c->xfer->data    = (uint8_t *)data;
    i2c->xfer->addr    = (uint8_t)(addr);
    i2c->xfer->pending = xfer_pending;

    /* Disable slave mode and set clock prescaler */
    i2c->reg->CTRL0 = (((i2c->reg->CTRL0 & ~I2C_CTRL0_SPEED_Mask)   |
                        (i2c->info->prescale << I2C_CTRL0_SPEED_Pos)) &
                       ~I2C_SLAVE_ENABLE);

    /* Generate start condition for specified slave */
    Sys_I2C_StartRead(i2c->xfer->addr);

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t I2C_SlaveTransmit (const uint8_t *data,
 *                                            uint32_t num,
 *                                            const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Start transmitting data as I2C Slave.
 * Inputs        : data - Pointer to buffer for data to transmit to I2C Master
 *                 num  - Number of data bytes to transmit
 *                 i2c  - Pointer to I2C resources
 * Outputs       : Execution status
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_SlaveTransmit (const uint8_t *data, uint32_t num,
                                  const I2C_RESOURCES *i2c)
{
    if ((data == NULL) || (num == 0U))
    {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if ((i2c->info->state & I2C_POWERED) == 0U)
    {
        /* Driver not yet configured */
        return ARM_DRIVER_ERROR;
    }

    if (i2c->info->status.busy)
    {
        /* Transfer operation in progress */
        return ARM_DRIVER_ERROR_BUSY;
    }

    /* Reset status */
    i2c->info->status = (const ARM_I2C_STATUS) {
        0
    };
    i2c->info->status.busy             = true;
    i2c->info->status.mode             = I2C_STATUS_MODE_SLAVE;
    i2c->info->status.direction        = I2C_STATUS_DIRECTION_TX;

    /* Set transfer info */
    i2c->xfer->num  = num;
    i2c->xfer->cnt  = 0U;
    i2c->xfer->data = (uint8_t *)data;

    /* Enable slave mode and set slave speed */
    i2c->reg->CTRL0 = ((i2c->reg->CTRL0 & ~I2C_CTRL0_SPEED_Mask) |
                       (I2C_SLAVE_SPEED_1 << I2C_CTRL0_SPEED_Pos) |
                       I2C_SLAVE_ENABLE);

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_SlaveReceive (uint8_t *data, uint32_t num,
 *                                        const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Start receiving data as I2C Slave.
 * Inputs        : data - Pointer to buffer for data to receive from I2C Master
 *                 num  - Number of data bytes to receive
 *                 i2c  - Pointer to I2C resources
 * Outputs       : Execution status
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_SlaveReceive (uint8_t *data, uint32_t num,
                                 const I2C_RESOURCES *i2c)
{
    if ((data == NULL) || (num == 0U))
    {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if ((i2c->info->state & I2C_POWERED) == 0U)
    {
        /* Driver not yet configured */
        return ARM_DRIVER_ERROR;
    }

    if (i2c->info->status.busy)
    {
        /* Transfer operation in progress */
        return ARM_DRIVER_ERROR_BUSY;
    }

    /* Reset status */
    i2c->info->status = (const ARM_I2C_STATUS) {
        0
    };
    i2c->info->status.busy             = true;
    i2c->info->status.mode             = I2C_STATUS_MODE_SLAVE;
    i2c->info->status.direction        = I2C_STATUS_DIRECTION_RX;

    /* Set transfer info */
    i2c->xfer->num  = num;
    i2c->xfer->cnt  = 0U;
    i2c->xfer->data = data;

    /* Enable slave mode and set slave speed */
    i2c->reg->CTRL0 = ((i2c->reg->CTRL0 & ~I2C_CTRL0_SPEED_Mask) |
                       (I2C_SLAVE_SPEED_1 << I2C_CTRL0_SPEED_Pos) |
                       I2C_SLAVE_ENABLE);

    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_GetDataCount (const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Get transferred data count.
 * Inputs        : i2c    - Pointer to I2C resources
 * Outputs       : number of data bytes transferred; -1 when Slave is not
 *                 addressed by Master
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_GetDataCount (const I2C_RESOURCES *i2c)
{
    return ((int32_t)i2c->xfer->cnt);
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_Control (uint32_t control, uint32_t arg,
 *                                   const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Control I2C Interface
 * Inputs        : control - control operation
 *                 arg     - Argument of operation (optional)
 *                 i2c     - Pointer to I2C resources
 * Outputs       : Execution status
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int32_t I2C_Control (uint32_t control, uint32_t arg, const I2C_RESOURCES *i2c)
{
    uint32_t bus_speed, delay_cycles;

    if ((i2c->info->state & I2C_POWERED) == 0U)
    {
        /* I2C not powered */
        return ARM_DRIVER_ERROR;
    }

    switch (control)
    {
        case ARM_I2C_OWN_ADDRESS:
        {
            if (i2c->info->status.busy)
            {
                /* Transfer operation in progress */
                return ARM_DRIVER_ERROR_BUSY;
            }

            /* Wait until bus is free */
            while (!(Sys_I2C_Get_Status() & I2C_BUS_FREE))
            {
                Sys_Watchdog_Refresh();
            }

            /* Set address */
            i2c->reg->CTRL0 = ((i2c->reg->CTRL0 & ~I2C_CTRL0_SLAVE_ADDRESS_Mask) |
                               ((arg << I2C_CTRL0_SLAVE_ADDRESS_Pos) &
                                I2C_CTRL0_SLAVE_ADDRESS_Mask));
        }
        break;

        case ARM_I2C_BUS_SPEED:
        {
            if (i2c->info->status.busy)
            {
                /* Transfer operation in progress */
                return ARM_DRIVER_ERROR_BUSY;
            }

            /* Set Bus Speed */
            switch (arg)
            {
                case ARM_I2C_BUS_SPEED_STANDARD:
                {
                    /* Standard Speed (100kHz) */
                    bus_speed = 100000;
                }
                break;

                case ARM_I2C_BUS_SPEED_FAST:
                {
                    /* Fast Speed     (400kHz) */
                    bus_speed = 400000;
                }
                break;

                case ARM_I2C_BUS_SPEED_FAST_PLUS:
                {
                    /* Fast+ Speed    (  1MHz) */
                    bus_speed = 1000000;
                }
                break;

                case ARM_I2C_BUS_SPEED_HIGH:

                /* High Speed     (3.4MHz) not supported */
                default:
                    return ARM_DRIVER_ERROR_UNSUPPORTED;
            }

            /* If system clock is not fast enough or
             * There isn't any prescale value that would divide system
             * clock exactly for the request bus speed, return error. */
            if ((SystemCoreClock < 3.25 * bus_speed) /*||
                ((SystemCoreClock % (3 * bus_speed)) != 0)*/)
            {
                return ARM_DRIVER_ERROR_UNSUPPORTED;
            }

            /* Calculate I2C prescaler */
            i2c->info->prescale = SystemCoreClock / (3 * bus_speed) - 1;

            /* Wait until bus is free */
            while (!(Sys_I2C_Get_Status() & I2C_BUS_FREE))
            {
                Sys_Watchdog_Refresh();
            }

            /* Set I2C prescaler */
            i2c->reg->CTRL0 = ((i2c->reg->CTRL0 & ~I2C_CTRL0_SPEED_Mask) |
                               (i2c->info->prescale << I2C_CTRL0_SPEED_Pos));

            /* Speed configured, I2C Master active */
            i2c->info->state |= I2C_CONFIGURED;
        }
        break;

        case ARM_I2C_BUS_CLEAR:
        {
            /* Disable I2C clock and interrupts temporarily */
            i2c->reg->CTRL0 &= ~I2C_SAMPLE_CLK_ENABLE;
            NVIC_DisableIRQ(i2c->irqn);

            /* Reset DIO_I2C_SRC configuration */
            DIO->I2C_SRC = (SDA_SRC_CONST_HIGH | SCL_SRC_CONST_HIGH);

            /* Configure pins as GPIO output */
            Sys_DIO_Config(i2c->pins->scl, DIO_MODE_GPIO_OUT_1);
            Sys_DIO_Config(i2c->pins->sda, DIO_MODE_GPIO_OUT_1);

            /* Delay cycles for the standard 100KHz speed */
            delay_cycles = SystemCoreClock / 100000;

            /* Issue 9 clock pulses on SCL line */
            for (uint8_t i = 0U; i < 9U; i++)
            {
                /* Clock high */
                Sys_GPIO_Set_High(i2c->pins->scl);
                Sys_Delay_ProgramROM(delay_cycles);

                /* Clock low */
                Sys_GPIO_Set_Low(i2c->pins->scl);
                Sys_Delay_ProgramROM(delay_cycles);
            }

            /* Manually generates STOP condition - SDA goes high while SCL is
             * high to end transaction. */
            Sys_GPIO_Set_Low(i2c->pins->sda);
            Sys_Delay_ProgramROM(delay_cycles);
            Sys_GPIO_Set_High(i2c->pins->scl);
            Sys_Delay_ProgramROM(delay_cycles);
            Sys_GPIO_Set_High(i2c->pins->sda);

            /* Reconfigure SCL and SDA Pins as I2C peripheral pins */
            Sys_I2C_DIOConfig(i2c->pins->pin_cfg, i2c->pins->scl,
                              i2c->pins->sda);

            /* Enable I2C clock and interrupt */
            i2c->reg->CTRL0 |= I2C_SAMPLE_CLK_ENABLE;
            NVIC_ClearPendingIRQ(i2c->irqn);
            NVIC_EnableIRQ(i2c->irqn);

            /* Send event */
            if (i2c->info->cb_event)
            {
                i2c->info->cb_event(ARM_I2C_EVENT_BUS_CLEAR);
            }
        }
        break;

        case ARM_I2C_ABORT_TRANSFER:
        {
            if (i2c->info->status.busy)
            {
                /* Disable IRQ temporarily */
                NVIC_DisableIRQ(i2c->irqn);

                /* If master, send stop */
                if (i2c->info->status.mode == I2C_STATUS_MODE_MASTER)
                {
                    i2c->reg->CTRL1 = I2C_STOP;

                    /* Wait for stop to be issued */
                    while (!(Sys_I2C_Get_Status() & I2C_STOP_DETECTED))
                    {
                        Sys_Watchdog_Refresh();
                    }
                }

                /* If slave */
                else
                {
                    /* Disable slave mode */
                    i2c->reg->CTRL0 &= ~I2C_SLAVE_ENABLE;

                    /* Reset the peripheral to a known state */
                    Sys_I2C_Reset();
                }

                /* Clear transfer info */
                i2c->xfer->num  = 0U;
                i2c->xfer->cnt  = 0U;
                i2c->xfer->data = NULL;
                i2c->xfer->addr = 0U;

                /* Clear statuses */
                i2c->info->status.busy             = 0U;
                i2c->info->status.general_call     = 0U;
                i2c->info->status.arbitration_lost = 0U;
                i2c->info->status.bus_error        = 0U;

                NVIC_ClearPendingIRQ(i2c->irqn);
                NVIC_EnableIRQ(i2c->irqn);
            }
        }
        break;

        default:
            return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_GetStatus(const I2C_RESOURCES *i2c)
 * ----------------------------------------------------------------------------
 * Description   : Get I2Cx status
 * Inputs        : i2c    - I2C resources
 * Outputs       : Return I2Cx status as an ARM_I2C_STATUS structure
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static ARM_I2C_STATUS I2C_GetStatus(const I2C_RESOURCES *i2c)
{
    return (i2c->info->status);
}

/* I2C0 Driver Wrapper functions. See functions above for usage */
#if RTE_I2C0
static int32_t I2C0_Initialize (ARM_I2C_SignalEvent_t cb_event)
{
    return I2C_Initialize(cb_event, &I2C0_Resources);
}

static int32_t I2C0_Uninitialize (void)
{
    return I2C_Uninitialize(&I2C0_Resources);
}

static int32_t I2C0_PowerControl (ARM_POWER_STATE state)
{
    return I2C_PowerControl(state, &I2C0_Resources);
}

static int32_t I2C0_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending)
{
    return I2C_MasterTransmit(addr, data, num, xfer_pending, &I2C0_Resources);
}

static int32_t I2C0_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending)
{
    return I2C_MasterReceive(addr, data, num, xfer_pending, &I2C0_Resources);
}

static int32_t I2C0_SlaveTransmit (const uint8_t *data, uint32_t num)
{
    return I2C_SlaveTransmit(data, num, &I2C0_Resources);
}

static int32_t I2C0_SlaveReceive (uint8_t *data, uint32_t num)
{
    return I2C_SlaveReceive(data, num, &I2C0_Resources);
}

static int32_t I2C0_GetDataCount (void)
{
    return I2C_GetDataCount(&I2C0_Resources);
}

static int32_t I2C0_Control (uint32_t control, uint32_t arg)
{
    return I2C_Control(control, arg, &I2C0_Resources);
}

static ARM_I2C_STATUS I2C0_GetStatus (void)
{
    return I2C_GetStatus(&I2C0_Resources);
}

void I2C_IRQHandler(void)
{
    if (I2C0_Resources.info->status.mode == I2C_STATUS_MODE_MASTER)
    {
        I2Cx_MasterIRQHandler(&I2C0_Resources);
    }
    else
    {
        I2Cx_SlaveIRQHandler(&I2C0_Resources);
    }
}

/* I2C0 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C0 =
{
    I2Cx_GetVersion,
    I2Cx_GetCapabilities,
    I2C0_Initialize,
    I2C0_Uninitialize,
    I2C0_PowerControl,
    I2C0_MasterTransmit,
    I2C0_MasterReceive,
    I2C0_SlaveTransmit,
    I2C0_SlaveReceive,
    I2C0_GetDataCount,
    I2C0_Control,
    I2C0_GetStatus
};
#endif    /* if RTE_I2C0 */

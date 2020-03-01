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
 * I2C_RSLxx.h
 * - I2C CMSIS-Driver header file for RSLxx family of devices
 * ----------------------------------------------------------------------------
 * $Revision: 1.5 $
 * $Date: 2018/03/09 19:11:39 $
 * ------------------------------------------------------------------------- */

#ifndef I2C_RSLXX_H
#define I2C_RSLXX_H

#include <Driver_I2C.h>
#include <HAL.h>

#define RTE_I2C0  1

#if (!RTE_I2C0)
  #error "I2C0 not configured in RTE_Device.h!"
#endif    /* if (!RTE_I2C0) */

/* Driver status flag definition */
#define I2C_INITIALIZED           ((uint8_t)(1U))
#define I2C_POWERED               ((uint8_t)(1U << 1))
#define I2C_CONFIGURED            ((uint8_t)(1U << 2))

/* Constants used to improve code readability */
#define I2C_STATUS_MODE_MASTER           1U
#define I2C_STATUS_MODE_SLAVE            0U
#define I2C_STATUS_DIRECTION_TX          0U
#define I2C_STATUS_DIRECTION_RX          1U

/* SPI Pins Configuration */
typedef const struct _I2C_PINS
{
    uint8_t scl;       /* SCL IO Pin number */
    uint8_t sda;       /* SDA IO Pin number */
    uint32_t pin_cfg;    /* LPF, Drive and Pull configuration */
} I2C_PINS;

/* I2C Transfer Information (Run-Time) */
typedef struct _I2C_TRANSFER_INFO
{
    uint32_t num;        /* Total number of data to transfer */
    uint32_t cnt;        /* Number of data transferred */
    uint8_t              *data;       /* Pointer to data buffer */
    uint8_t addr;       /* Device address */
    bool pending;    /* If transfer is pending */
} I2C_TRANSFER_INFO;

/* I2C Information (Run-time) */
typedef struct _I2C_INFO
{
    ARM_I2C_SignalEvent_t cb_event;    /* Event Callback */
    ARM_I2C_STATUS status;     /* Status flags */
    uint8_t state;      /* Current I2C state */
    uint32_t prescale;    /* I2C clock prescale speed */
} I2C_INFO;

/* I2C Resources definition */
typedef struct
{
    I2C_Type               *reg;      /* I2C peripheral pointer */
    I2C_PINS               *pins;     /* I2C Input/Output pins */
    IRQn_Type irqn;                   /* I2C IRQ Number */
    I2C_INFO               *info;     /* Run-Time Information */
    I2C_TRANSFER_INFO      *xfer;     /* I2C transfer information */
} I2C_RESOURCES;

#endif    /* I2C_RSLXX_H */

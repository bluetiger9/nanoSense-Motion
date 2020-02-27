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
 * SPI_RSLxx.h
 * - CMSIS-Driver header file for RSLXX SPI
 * ----------------------------------------------------------------------------
 * $Revision: 1.6 $
 * $Date: 2018/03/14 17:09:19 $
 * ------------------------------------------------------------------------- */

#ifndef SPI_RSLXX_H
#define SPI_RSLXX_H

#include <Driver_SPI.h>
#include <HAL.h>

#define RTE_SPI0  1

#if ((!RTE_SPI0) && (!RTE_SPI1))
  #error "SPI not configured in RTE_Device.h!"
#endif    /* if ((!RTE_SPI0) && (!RTE_SPI1)) */

/* Constants */
#define SPI_MAX_PRESCALE          9U

/* Driver status flag definition */
#define SPI_INITIALIZED           ((uint8_t)(1U))
#define SPI_POWERED               ((uint8_t)(1U << 1))
#define SPI_CONFIGURED            ((uint8_t)(1U << 2))
#define SPI_DATA_LOST             ((uint8_t)(1U << 3))

/* SPI Pins Configuration */
typedef const struct _SPI_PINS
{
    uint8_t mosi;       /* Master Out, Slave In */
    uint8_t miso;       /* Master In, Slave Out */
    uint8_t ssel;       /* Slave select */
    uint8_t sclk;       /* Serial clock */
} SPI_PINS;

/* SPI Status */
typedef struct _SPI_STATUS
{
    uint8_t busy;     /* Transmitter/Receiver busy flag */
    uint8_t data_lost;    /* Data lost: Receive overflow/Transmit underflow
                           * (cleared on start of transfer operation) */
    uint8_t mode_fault;    /* Not supported */
} SPI_STATUS;

/* SPI Information (Run-time) */
typedef struct _SPI_INFO
{
    ARM_SPI_SignalEvent_t cb_event;    /* Event Callback */
    SPI_STATUS status;     /* Status flags */
    uint8_t state;      /* Current SPI state */
    uint32_t mode;       /* Current SPI mode */
} SPI_INFO;

/* SPI Transfer Information (Run-Time) */
typedef struct _SPI_TRANSFER_INFO
{
    uint32_t num;        /* Total number of data to transfer */
    uint8_t              *rx_buf;     /* Pointer to in data buffer */
    uint8_t              *tx_buf;     /* Pointer to out data buffer */
    uint32_t rx_cnt;     /* Number of data received */
    uint32_t tx_cnt;     /* Number of data sent */
    uint32_t def_val;    /* Default transfer value */
} SPI_TRANSFER_INFO;

/* SPI Resources definition */
typedef struct
{
    SPI1_Type              *reg;      /* SPI peripheral pointer */
    SPI_PINS               *pins;     /* SPI Input/Output pins */
    IRQn_Type rx_irqn;                /* SPI RX IRQ Number */
    IRQn_Type tx_irqn;                /* SPI TX IRQ Number */
    SPI_INFO               *info;     /* Run-Time Information */
    SPI_TRANSFER_INFO      *xfer;     /* SPI transfer information */
} SPI_RESOURCES;

#endif    /* SPI_RSLXX_H */

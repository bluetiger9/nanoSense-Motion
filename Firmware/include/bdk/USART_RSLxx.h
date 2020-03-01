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
 * USART_RSLXX.h
 * - CMSIS-Driver header file for RSLXX UART
 * ----------------------------------------------------------------------------
 * $Revision: 1.1 $
 * $Date: 2018/03/15 21:34:38 $
 * ------------------------------------------------------------------------- */

#ifndef USART_RSLXX_H
#define USART_RSLXX_H

#include "Driver_USART.h"
#include <HAL.h>

#define RTE_USART0  1


#if !RTE_USART0
  #error "USART not configured in RTE_Device.h!"
#endif    /* if !RTE_USART0 */

/* USART flags */
#define USART_FLAG_INITIALIZED      ((uint8_t)(1U))
#define USART_FLAG_POWERED          ((uint8_t)(1U << 1))
#define USART_FLAG_CONFIGURED       ((uint8_t)(1U << 2))

/* USART Input/Output Configuration */
typedef const struct _USART_IO
{
    uint8_t tx;           /* TX  Pin identifier */
    uint8_t rx;           /* RX  Pin identifier */
} USART_PINS;

/* USART Transfer Information (Run-Time) */
typedef struct _USART_TRANSFER_INFO
{
    uint32_t rx_num;     /* Total number of receive data */
    uint32_t tx_num;     /* Total number of transmit data */
    uint8_t  *rx_buf;     /* Pointer to in data buffer */
    uint8_t  *tx_buf;     /* Pointer to out data buffer */
    uint32_t rx_cnt;     /* Number of data received */
    uint32_t tx_cnt;     /* Number of data sent */
} USART_TRANSFER_INFO;

/* USART Status */
typedef struct _USART_STATUS
{
    uint8_t tx_busy;      /* Transmitter busy flag */
    uint8_t rx_busy;      /* Receiver busy flag */
} USART_STATUS;

/* USART Information (Run-time) */
typedef struct _USART_INFO
{
    ARM_USART_SignalEvent_t cb_event;    /* Event Callback */
    USART_STATUS status;    /* Status flags */
    uint8_t flags;    /* Current USART flags */
    uint32_t mode;     /* Current USART mode */
    uint32_t baudrate;    /* Baudrate */
} USART_INFO;

/* USART Resources definition */
typedef const struct
{
    ARM_USART_CAPABILITIES capabilities;    /* Capabilities */
    UART_Type              *reg;         /* UART peripheral pointer */
    USART_PINS pins;         /* USART Input/Output pins */
    IRQn_Type uart_rx_irqn;    /* USART RX IRQ Number */
    IRQn_Type uart_tx_irqn;    /* USART TX IRQ Number */
    USART_INFO             *info;        /* Run-Time Information */
    USART_TRANSFER_INFO    *xfer;        /* USART transfer information */
} USART_RESOURCES;

#endif    /* USART_RSLXX_H */

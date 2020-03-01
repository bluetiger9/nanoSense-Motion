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
//! \file BLE_ICS.h
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup BLE_GRP
//! \{
//! \addtogroup ICS IoT IDK Custom Service Profile
//!
//! \brief BLE profile for bidirectional communication between BDK and client
//! device.
//!
//! Two characteristics are provided to act as RX line for receiving requests
//! from client device and TX line for providing responses.<br>
//! Size of message that can be send and received over these characteristics is
//! limited to 20 bytes.<br>
//! Transmitted data are application specific and can be in binary form or as
//! readable AT commands.
//!
//! \warning Custom Service Profile uses message handlers registered under
//! application task (TASK_APP) to communicate with GATTM and GATTC tasks which
//! manage attribute database and active connections.
//! Therefore there cannot be second user defined profile handled by
//! application task at the same time as they would collide.<br>
//! This concerns only profiles that are not directly supported by RSL10 BLE
//! stack which have assigned their own tasks by event kernel.<br>
//! Following message handlers will be added to application task:
//!     * GATTM_ADD_SVC_RSP
//!     * GATTC_READ_REQ_IND
//!     * GATTC_WRITE_REQ_IND
//!     * GATTC_ATT_INFO_REQ_IND
//!     * GATTC_CMP_EVT
//!
//! \b Example: \n
//! Minimal code example which uses Custom Service.
//! It echoes all data written to RX characteristic back to client by sending
//! notifications over TX characteristic with the same  data.
//! \include ics_example.c
//! \{
//-----------------------------------------------------------------------------

#ifndef BLE_ICS_H
#define BLE_ICS_H

#include "BLE_PeripheralServer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief IoT IDK Service UUID */
#define ICS_SERVICE_UUID                { 0x24, 0xdc, 0x0e, 0x6e, 0x01, 0x40, \
                                          0xca, 0x9e, 0xe5, 0xa9, 0xa3, 0x00, \
                                          0xb5, 0xf3, 0x93, 0xe0 }

/** \brief IoT IDK Service TX Characteristic UUID */
#define ICS_TX_CHARACTERISTIC_UUID      { 0x24, 0xdc, 0x0e, 0x6e, 0x02, 0x40, \
                                          0xca, 0x9e, 0xe5, 0xa9, 0xa3, 0x00, \
                                          0xb6, 0xf3, 0x93, 0xe0 }

/** \brief IoT IDK Service RX Characteristic UUID */
#define ICS_RX_CHARACTERISTIC_UUID      { 0x24, 0xdc, 0x0e, 0x6e, 0x03, 0x40, \
                                          0xca, 0x9e, 0xe5, 0xa9, 0xa3, 0x00, \
                                          0xb7, 0xf3, 0x93, 0xe0 }

/** \brief Human readable TX characteristic description.
 *
 * Can be read from <i>Characteristic User Description</i> of TX
 * characteristic.
 */
#define ICS_TX_CHARACTERISTIC_NAME      "TX_VALUE - Response from BDK"

/** \brief Human readable RX characteristic description.
 *
 * Can be read from <i>Characteristic User Description</i> of RX
 * characteristic.
 */
#define ICS_RX_CHARACTERISTIC_NAME      "RX_VALUE - Command for BDK"

#define ICS_TX_CHARACTERISTIC_NAME_LEN  (sizeof(ICS_TX_CHARACTERISTIC_NAME) - 1)

#define ICS_RX_CHARACTERISTIC_NAME_LEN  (sizeof(ICS_RX_CHARACTERISTIC_NAME) - 1)

/** \brief Maximum amount of data that can be either received from RX
 * characteristic or send over TX characteristic.
 *
 * This is limited to 20 characters to accommodate for maximal allowed
 * notification length.
 */
#define ICS_CHARACTERISTIC_VALUE_LENGTH (20)

/** \brief Attribute database indexes of ICS characteristics. */
enum BLE_ICS_AttributeIndex
{
    /* TX Characteristic */
    ICS_IDX_TX_VALUE_CHAR,
    ICS_IDX_TX_VALUE_VAL,
    ICS_IDX_TX_VALUE_CCC,
    ICS_IDX_TX_VALUE_USR_DSCP,

    /* RX Characteristic */
    ICS_IDX_RX_VALUE_CHAR,
    ICS_IDX_RX_VALUE_VAL,
    ICS_IDX_RX_VALUE_CCC,
    ICS_IDX_RX_VALUE_USR_DSCP,

    /* Max number of characteristics */
    ICS_IDX_NB,
};

/** \brief Initialization state of ICS library. */
enum BLE_ICS_State
{
    BLE_ICS_OFF = 0, /**< Initial state after device power up. */
    BLE_ICS_CREATE_DB, /**< Waiting for attribute database creation confirmation. */
    BLE_ICS_READY, /**< Waiting for client device to connect.  */
    BLE_ICS_CONNECTED /**< Client device connected. Notifications can be send. */
};

/** \brief Data structure passed to application specific Write Indication
 * callback handler.
 */
struct BLE_ICS_RxIndData
{
    /** \brief Stores data received in last write to RX characteristic. */
    uint8_t data[ICS_CHARACTERISTIC_VALUE_LENGTH];

    /** \brief Number of valid data bytes in \ref data array. */
    uint8_t data_len;
};

/** \brief Callback type for handling of RX Write indication events. */
typedef void (*BLE_ICS_RxIndHandler)(struct BLE_ICS_RxIndData *ind);

/** \brief Stores internal state ICS Profile. */
struct BLE_ICS_Resources
{
    /** \brief Stores current initialization and connection state. */
    uint8_t state;

    /** \brief Index of first ICS characteristic in attribute database. */
    uint16_t start_hdl;

    /** \brief Application specific handler for RX characteristic write
     * indication events.
     */
    BLE_ICS_RxIndHandler rx_write_handler;

    uint8_t tx_value[ICS_CHARACTERISTIC_VALUE_LENGTH];
    uint8_t tx_value_length;
    uint16_t tx_cccd_value;

    uint8_t rx_value[ICS_CHARACTERISTIC_VALUE_LENGTH];
    uint8_t rx_value_length;
    uint16_t rx_cccd_value;
};

/** \brief Adds IoT IDK Custom Service into BDK BLE stack.
 *
 * Calling this function will add Custom Service to BLE attribute database.
 *
 * This function does the following initialization steps:
 *     1. Initialize BDK BLE stack by calling BDK_BLE_Initalize.
 *     2. Add message handler callbacks to application task.<br>
 *        These handlers are needed to communicate with GATTM and GATTC tasks.
 *     3. Adds service initialization & enable callbacks that will be executed
 *        during attribute database generation.
 *
 * \pre This functions must be called before Application task is started and
 * before Event Kernel messaging is started, e.g. before entering main loop
 * and calling \ref BDK_Schedule .
 *
 * \param rx_ind_handler
 */
extern void BLE_ICS_Initialize(BLE_ICS_RxIndHandler rx_ind_handler);

/** \brief Send out a notification over TX characteristic.
 *
 * TX characteristic will be updated with new data and connected device will
 * receive notification of this change.
 *
 * \param data
 * Data that will be sent in notification packet and stored in TX
 * characteristic.
 *
 * \param data_len
 * Length of given data.
 *
 * \returns Operation status code.
 * | Code | Description                                              |
 * | ---- | -------------------------------------------------------- |
 * | 0    | On success.                                              |
 * | 1    | If there is no BLE client device connected.              |
 * | 2    | If length of data is bigger than allowed maximum length. |
 *
 */
extern uint32_t BLE_ICS_Notify(uint8_t *data, uint8_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* BLE_ICS_H */

//! \}
//! \}
//! \}

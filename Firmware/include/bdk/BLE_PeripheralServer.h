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
//! \file BLE_PeripheralServer.h
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup BLE_GRP Bluetooth Low Energy
//!
//! \brief Library for handling of BLE functionality and libraries of supported
//! BLE profiles.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BLE_PERIPHERAL_SERVER_H_
#define BLE_PERIPHERAL_SERVER_H_

#include <rsl10.h>
#include <rsl10_ke.h>
#include <rsl10_ble.h>
#include <rsl10_profiles.h>
#include <rsl10_protocol.h>
#include <rsl10_hw_cid101.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Maximum number of BDK supported BLE Profiles that can be added to
 * database.
 */
#define BDK_BLE_SVC_MAX                (5)

/** \brief Maximum number of connected devices.
 *
 * BDK BLE stack does not support multiple connections at the same time.
 */
#define BDK_BLE_MASTER_MAX             (1)

/** \brief Maximum length of device <i>Complete Local Name</i>.
 *
 * \see BDK_BLE_SetLocalName
 */
#define BDK_BLE_LOCAL_NAME_MAX_LENGTH  (26)

/** \brief Default local name that will be used.
 *
 * Device name can be changed by calling BDK_BLE_SetLocalName.
 */
#define BDK_BLE_DEFAULT_LOCAL_NAME     "BDK"

/** \brief Maximum length of application specific data that can be attached to
 * <i>Manufacturer Specific Data</i>.
 */
#define BDK_BLE_MANUF_DATA_MAX_LENGTH  (27)

/** \brief Start of <i>Manufacturer Specific Data</i> containing ON
 * Semiconductor <i>Company Identifier Code</i> <t>0x0362</t>.
 */
#define BDK_BLE_MANUFACTURER_ID        "\xff\x62\x03"
#define BDK_BLE_MANUFACTURER_ID_LENGTH 3

/** Length of Bluetooth address (in octets) */
#define BDK_BLE_BADDR_LENGTH           BD_ADDR_LEN

/** Factory loaded static public address from ROM memory will be used. */
#define BDK_BLE_BADDR_TYPE_PUBLIC      (0)

/** Static private address defined by BDK_BLE_PRIVATE_ADDR will be used for
 * this device.
 */
#define BDK_BLE_BADDR_TYPE_PRIVATE     (1)

/** Selects which Bluetooth address will be used by the application. */
#define BDK_BLE_BADDR_TYPE             BDK_BLE_BADDR_TYPE_PUBLIC

/** \brief Static private Bluetooth device address.
 *
 * This address will be used when BDK_BADDR_TYPE is set to
 * BDK_BADDR_TYPE_PRIVATE.
 */
#define BDK_BLE_PRIVATE_BADDR          { 0x94, 0x11, 0x22, 0xff, 0xff, 0xD5 }

/** RF Oscillator accuracy in ppm */
#define BDK_BLE_RADIO_CLOCK_ACCURACY   (20)

/* Slave preferred connection parameters */

// BDK_BLE_PREF_SLV_MIN_CON_INTERVAL = int_min [ms] / 1.25 [ms]
// 100 ms -> 80
#define BDK_BLE_PREF_SLV_MIN_CON_INTERVAL (80)

// BDK_BLE_PREF_SLV_MAX_CON_INTERVAL = int_max [ms] / 1.25 [ms]
// 500 ms -> 400
#define BDK_BLE_PREF_SLV_MAX_CON_INTERVAL (400)

#define BDK_BLE_PREF_SLV_LATENCY          (0)
#define BDK_BLE_PREF_SLV_SUP_TIMEOUT      (200)

/* GAPM configuration definitions */
#define BDK_BLE_RENEW_DUR              (15000)
#define BDK_BLE_MTU_MAX                (0x200)
#define BDK_BLE_MPS_MAX                (0x200)
#define BDK_BLE_ATT_CFG                (0x80)
#define BDK_BLE_TX_OCT_MAX             (0x1b)
#define BDK_BLE_TX_TIME_MAX            (14 * 8 + BDK_BLE_TX_OCT_MAX * 8)

/** \brief Default advertisement interval - 40ms (64*0.625ms) */
#define BDK_BLE_ADV_INT_DEFAULT        (64)

#define INVALID_DEV_IDX                (-1)

typedef void (*BDK_BLE_SVC_AddFunc)(void);
typedef void (*BDK_BLE_SVC_EnableFunc)(uint8_t);

/** \brief Initializes BLE Stack of RSL10.
 *
 */
extern void BDK_BLE_Initialize(void);

/** \brief Sets Complete Local Name of this device.
 *
 * This name will be broadcasted in advertisement packets.
 *
 * \param name
 * Null terminated string containing complete local name of this device.
 * Maximum length of this string excluding Null character is
 * \ref BDK_BLE_LOCAL_NAME_MAX_LENGTH .
 */
extern void BDK_BLE_SetLocalName(const char* name);

/** \brief Sets additional data that will be appended to Manufacturer Specific
 * Data.
 *
 * This function allows to add custom data to the Manufacturer Specific Data
 * broadcasted in Scan Response packets.
 * Custom data are appended after predefined 2 byte Manufacturer ID.
 *
 * \param data
 * Pointer to data that will be appended to manufacturer specific record.
 *
 * \param len
 * Length of data.
 * Maximum allowed length is \ref BDK_BLE_MANUF_DATA_MAX_LENGTH .
 */
extern void BDK_BLE_SetManufSpecificData(const uint8_t* data, uint32_t len);

/** \brief Set custom advertising interval.
 *
 * \param interval_min
 * Minimum interval N for advertising.<br>
 * Value Time = N * 0.625 ms
 *
 * \param interval_max
 * Maximum interval N for advertising.<br>
 * Value Time = N * 0.625 ms
 *
 * \see BDK_BLE_ADV_INT_DEFAULT for default advertisement interval value.
 */
extern void BDK_BLE_SetAdvertisementInterval(uint16_t interval_min, uint16_t interval_max);

extern signed int BDK_BLE_GetConIdx(void);

extern bool BDK_BLE_IsConnected(void);

extern void BDK_BLE_AddService(void (*svc_add_func)(void), void (*svc_enable_func)(uint8_t));

/** internal */
extern void BDK_BLE_ProfileAddedInd(void);

extern void BDK_BLE_AdvertisingStart(void);

extern void BDK_BLE_AdvertisingStop(void);


#ifdef __cplusplus
}
#endif

#endif /* BLE_PERIPHERAL_SERVER_H_ */

//! \}
//! \}

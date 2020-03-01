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
//! \file BLE_BASS.h
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup BLE_GRP
//! \{
//! \addtogroup BASS Battery Service Profile
//!
//! \brief BLE profile for reporting of BDK battery level.
//!
//! Reported battery level is in range from 0% to 100%.
//! Battery voltages of 3V and above are reported as 100%.
//! Battery voltages of 2V and below are reported as 0%.
//!
//!
//! \b Example: \n
//! This is an example of how to integrate Battery Service into an BDK
//! application.
//! Battery level will be automatically updated every 32 seconds and send to
//! any connected devices.
//! \include bass_example.c
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BLE_BASS_H
#define BLE_BASS_H

#include "BLE_PeripheralServer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ADC values for different VBAT voltage levels */
#define VBAT_1p1V_MEASURED             0x1200
#define VBAT_1p4V_MEASURED             0x16CC
#define VBAT_2p0V_MEASURED             0x1FFF
#define VBAT_3p0V_MEASURED             0x2FFF

/** \brief Convert voltage level to ADC output value. */
#define CALC_VBAT_MEASURED(V_in)       ((uint16_t) (16383.0f * (((float)V_in) / 4.0f)))

/** \brief This callback is used by BASS service to notify application of
 * battery level change.
 *
 * \param bat_voltage
 * Measured battery voltage level as received from ADC.
 * Ranges from 0x0000 (0V) up to  (4V).
 *
 * \param bat_perccent
 * Battery level percentage calculated based on battery empty/full voltages.
 * Ranges from 0% up to 100%.
 */
typedef void (*BLE_BASS_BattLevelInd)(uint16_t bat_voltage, uint16_t bat_percent);

/** \brief Adds Battery Service Profile to current BLE configuration.
 *
 * Connected device will be able to retrieve current battery level.
 *
 * Device will report battery level of 0% before first measurement cycle is
 * completed.
 *
 * \param sample_rate
 * How often the battery voltage retrieved from the ADC.
 * Value is in milliseconds and will be rounded to 10 ms.
 *
 * \param avg_count
 * How many ADC samples will be averaged to produce final battery level.
 */
extern void BLE_BASS_Initialize(uint16_t sample_rate, uint8_t avg_count);

/** \brief Sets custom battery voltage limits for calculating battery level
 * percentage.
 *
 * Default voltage range is 2V - 3V (0% - 100%).
 *
 * Parameter values are the voltage levels as measured by ADC.
 * ADC provides and VBAT/2 divider which allows it to measure full range of
 * operating voltages.
 * Parameter values can be calculated as follows:
 *
 *     ADC_meas = ADC_max * (V_level / V_max)
 *
 * Where:
 *   * ADC_max - Maximum ADC output value. Constant at 0x3FFF (16383).
 *   * V_max - Input voltage to ADC that would result in ADC measuring ADC_max
 *     output value. With the VBAT/2 divider this is 4V.
 *   * V_level - Voltage level for which ADC_meas is being calculated.
 *   * ADC_meas - ADC output value that will be measured when input voltage
 *     equals V_level.
 *
 * Example:<br>
 * Calculating ADC output value for input voltage V_level = 3V.
 *
 *     ADC_meas = 16383 * (3V / 4V)
 *     ADC_meas = 12287
 *
 * CALC_VBAT_MEASURED macro can be used to calculate battery level.
 *
 * \param batt_empty
 * Battery voltage level representing fully discharged battery
 * (0% battery level).
 *
 * \param batt_full
 * Battery voltage level representing fully charged battery
 * (100% battery level).
 */
extern void BLE_BASS_SetVoltageRange(uint16_t batt_empty, uint16_t batt_full);

/** \brief Sets application callback to notify application of battery level
 * change.
 *
 * Callback will be also called after first measurement cycle when battery
 * level changes from 0% to actual battery level.
 *
 * \param cb Application provided battery level indication callback.
 */
extern void BLE_BASS_SetBattLevelInd(BLE_BASS_BattLevelInd cb);

#ifdef __cplusplus
}
#endif

#endif /* BLE_BASS_H */

//! \}
//! \}
//! \}

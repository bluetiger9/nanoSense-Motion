/* ----------------------------------------------------------------------------
 * Copyright (c) 2017 Semiconductor Components Industries, LLC (d/b/a
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
 * calibration.h
 * - Calibration header
 * ----------------------------------------------------------------------------
 * $Revision: 1.30 $
 * $Date: 2018/09/27 17:26:25 $
 * ------------------------------------------------------------------------- */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ----------------------------------------------------------------------------
 * Include files
 * ------------------------------------------------------------------------- */

#if (CALIB_RECORD == USER_CALIB)
#include <rsl10_calibrate.h>
#endif    /* CALIB_RECORD */

/* ----------------------------------------------------------------------------
 * Defines
 * ------------------------------------------------------------------------- */

/* RF output transmission power level (in unit of dBm)
 * Options: [0 | 3 | 6 ] for 0, 3, 6 dBm respectively */
#ifndef RF_TX_POWER_LEVEL_DBM
#define RF_TX_POWER_LEVEL_DBM           0
#endif

/* This sample application considers three possible cases regarding the
 * calibrations of supply voltages:
 *  - (1) Trim values for desired voltages were loaded in the calibration
 * records during manufacturing (NVR4). This application simply reads them
 * from NVR4 and load them into corresponding trim registers to calibrate
 * the board. In this case, define CALIB_RECORD as MANU_CALIB.
 *  - (2) Not case (1) and supplemental_calibrate sample application was
 * used to calculate and store those trim values in the calibration information
 * region of NVR3. During the system boot process, the user-defined initialization
 * function reads and loads the supplemental calibrated values from NVR3 into
 * corresponding trim registers to calibrate the board. In this case,
 * define CALIB_RECORD as SUPPLEMENTAL_CALIB.
 *  - (3) Neither case (1) nor (2). This application needs to calculate
 * trim values for desired voltages and load them into corresponding trim
 * registers to calibrate the board. In this case, define CALIB_RECORD as
 * USER_CALIB.
 */
#define MANU_CALIB                      1
#define SUPPLEMENTAL_CALIB              2
#define USER_CALIB                      3

/* Calibration records to be loaded for voltage calibration
 * Options: - MANU_CALIB
 *          - SUPPLEMENTAL_CALIB
 *          - USER_CALIB */
#ifndef CALIB_RECORD
#define CALIB_RECORD                    MANU_CALIB
#endif

/* Hold the application to verify if calibrated voltages agree with
 * target voltages */

/* #define VOLTAGES_CALIB_VERIFY */

/* PA_PWR trim value */
#define PA_PWR_TRIM_VAL                 12

/* To check if there are any errors when calibrating supply voltages */
#define VOLTAGES_CALIB_NO_ERROR         (uint8_t)(0x0)
#define VCC_CALIB_ERROR                 (uint8_t)(0x1 << 0)
#define VDDRF_CALIB_ERROR               (uint8_t)(0x1 << 1)
#define VDDPA_CALIB_ERROR               (uint8_t)(0x1 << 2)
#define VDDC_CALIB_ERROR                (uint8_t)(0x1 << 3)
#define VDDM_CALIB_ERROR                (uint8_t)(0x1 << 4)

/* If trim values for desired voltages were loaded in
 * the calibration records during manufacturing (NVR4) */
#if (CALIB_RECORD == MANU_CALIB)

/* Calibrated voltage targets [10 * mV] */

#define VDDC_TARGET                     (uint8_t)(92)
#define VDDM_TARGET                     (uint8_t)(105)

/* RF output transmission power level = 0 dBm */
#if (RF_TX_POWER_LEVEL_DBM == 0)
#define VCC_TARGET                      (uint8_t)(110)

/* RF output transmission power level = 3 dBm */
#elif (RF_TX_POWER_LEVEL_DBM == 3)
#define VCC_TARGET                      (uint8_t)(120)

/* RF output transmission power level = 6 dBm */
#elif (RF_TX_POWER_LEVEL_DBM == 6)
#define VCC_TARGET                      (uint8_t)(120)

#endif    /* RF_TX_POWER_LEVEL_DBM */

/* Else if supplemental_calibrate already calculated and saved
 * trim values in NVR3 */
#elif (CALIB_RECORD == SUPPLEMENTAL_CALIB)

/* ----------------------------------------------------------------------------
 * Structure that holds calibration values to be read from NVR3
 * ------------------------------------------------------------------------- */
typedef struct
{
    uint16_t VDDC_CAL_TRIM_VALUE;
    uint16_t VDDC_CAL_TARGET;

    uint16_t VDDM_CAL_TRIM_VALUE;
    uint16_t VDDM_CAL_TARGET;

    uint16_t VCC_CAL_TRIM_VALUE;
    uint16_t VCC_CAL_TARGET;

    uint16_t START_OSC_CAL_TRIM_VALUE;
    uint16_t START_OSC_CAL_TARGET;

    int16_t TX_POWER_VALUE;
    uint32_t reserved2;                  /* Required for alignment to an even
                                          * number of words */
} CalSetting_Type;

/* Read the supplemental calibration values from NVR3 */
#define CALSETTING_BASE                 (MANU_INFO_INIT - sizeof(CalSetting_Type))
#define CALSETTING                      ((CalSetting_Type *)CALSETTING_BASE)

/* If supplemental_calibrate has not calculated and saved
 * supply voltage trim values in NVR3 */
#elif (CALIB_RECORD == USER_CALIB)

/* Calibrated voltage targets [10 * mV] */

#define ADC_VDDC_TARGET                 (uint8_t)(92)
#define ADC_VDDM_TARGET                 (uint8_t)(105)

#define VCC_TRIM_1P12V_BYTE             ((uint8_t)(0xCU << \
                                                   ACS_VCC_CTRL_VTRIM_BYTE_Pos))

/* RF output transmission power level = 0 dBm */
#if (RF_TX_POWER_LEVEL_DBM == 0)
#define ADC_VDDRF_TARGET                (uint8_t)(105)
#define ADC_DCDC_TARGET                 (uint8_t)(110)
#define VCC_TRIM_BYTE_FOR_VDDRF_CALIB   VCC_TRIM_1P12V_BYTE

/* RF output transmission power level = 3 dBm */
#elif (RF_TX_POWER_LEVEL_DBM == 3)
#define ADC_VDDRF_TARGET                (uint8_t)(126)
#define ADC_DCDC_TARGET                 (uint8_t)(131)
#define VCC_TRIM_BYTE_FOR_VDDRF_CALIB   VCC_TRIM_1P31V_BYTE

/* RF output transmission power level = 6 dBm */
#elif (RF_TX_POWER_LEVEL_DBM == 6)
#define ADC_VDDRF_TARGET                (uint8_t)(110)
#define ADC_DCDC_TARGET                 (uint8_t)(120)
#define ADC_VDDPA_TARGET                (uint8_t)(160)
#define VCC_TRIM_BYTE_FOR_VDDRF_CALIB   VCC_TRIM_1P20V_BYTE
#define POWER_AMPLIFIER_ON

#endif    /* RF_TX_POWER_LEVEL_DBM */

/* ----------------------------------------------------------------------------
 * Structure that holds calibration values to be written in NVR3
 * ------------------------------------------------------------------------- */
typedef struct
{
    uint16_t VBG_CAL_TRIM_VALUE;
    uint16_t VBG_CAL_TARGET;
    uint16_t VDDRF_CAL_TRIM_VALUE;
    uint16_t VDDRF_CAL_TARGET;
    uint16_t VDDPA_CAL_TRIM_VALUE;
    uint16_t VDDPA_CAL_TARGET;
    uint16_t VDDC_CAL_TRIM_VALUE;
    uint16_t VDDC_CAL_TARGET;
    uint16_t VDDM_CAL_TRIM_VALUE;
    uint16_t VDDM_CAL_TARGET;
    uint16_t DCDC_CAL_TRIM_VALUE;
    uint16_t DCDC_CAL_TARGET;
    uint16_t RCOSC_CAL_TRIM_VALUE;
    uint16_t RCOSC_CAL_TARGET;
    uint16_t START_OSC_CAL_TRIM_VALUE;
    uint16_t START_OSC_CAL_TARGET;
} CalSetting;

#endif    /* CALIB_RECORD */

/* ----------------------------------------------------------------------------
 * Function prototype definitions
 * ------------------------------------------------------------------------- */

#if (CALIB_RECORD == MANU_CALIB)
uint8_t Load_Trim_Values_And_Calibrate_MANU_CALIB(void);

#elif (CALIB_RECORD == USER_CALIB)
uint8_t Calculate_Trim_Values_And_Calibrate(void);

#endif    /* CALIB_RECORD */

#if ((CALIB_RECORD == SUPPLEMENTAL_CALIB) || (CALIB_RECORD == MANU_CALIB))
int16_t Load_Tx_Power_Value(void);

#endif    /* if ((CALIB_RECORD == SUPPLEMENTAL_CALIB) || (CALIB_RECORD == MANU_CALIB)) */

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* CALIBRATION_H_ */

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
//! \file BME680_ENV.h
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup BME680_ENV_GRP BME680 Environmental Sensor
//!
//! \brief Environmental sensor measuring Temperature, Humidity and Pressure.
//!
//! For usage example see the bme680_env_example available in BDK CMSIS-Pack
//! sample projects.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BME680_ENV_H_
#define BME680_ENV_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Data structure for passing measured environmental data from driver to
 * application.
 */
struct BME680_ENV_Data
{
    /** \brief Relative humidity in % x1000
     *
     * To get relative humidity in percent this value has to be divided by 1000.
     */
    uint32_t humidity;

    /** \brief Atmospheric pressure in Pascal. */
    uint32_t pressure;

    /** \brief Ambient temperature in degree Celsius x100.
     *
     * To get ambient temperature in degrees Celsius this value has to be
     * divided by 100.
     */
    int16_t temperature;
};

/** \brief New environmental data indication callback prototype.
 *
 * \param output
 * Contains newly measured environmental data.
 * \warning
 * This pointer is valid only within the scope of this function.
 * If data are needed outside of this function they need to be copied.
 *
 * \see BME680_ENV_Interface::StartPeriodic
 */
typedef void (*BME680_ENV_Callback)(struct BME680_ENV_Data *output);

/** \brief Initializes the BME680 environmental sensor.
 *
 * This functions checks if BME680 is present on the I2C bus and then
 * configures it with preset configuration.
 * Preset configuration can be modified by changing the project specific
 * RTE_BDK_BME680_ENV.h configuration file.
 *
 * Device will be put into sleep mode until periodic measurements are
 * started.
 *
 * \returns
 * 0 - On success.<br>
 * BME680_E_* - Error code on failure.
 */
extern int32_t BME680_ENV_Initialize(void);

/** \brief Returns the time required to complete measurement with current configuration.
 *
 * This can be used to check that desired measurement period is larger than
 * measurement duration.
 *
 * \returns
 * Time required to complete measurement in milliseconds.
 */
extern uint32_t BME680_ENV_GetProfileDuration(void);

/** \brief Starts periodic sensor measurements with given time period.
 *
 * \param cb
 * Callback that will be called with newest measured data after each
 * measurement cycle is completed.
 *
 * \param period_ms
 * Measurement will be started every \p period_ms milliseconds.
 * Provided callback \p cb will be called as soon as measurement is
 * finished.
 *
 * \returns
 * HAL_ERROR_PARAMETER - If period is smaller than measurement profile
 * duration or no callback is provided.<br>
 * 0 - On success.
 */
extern int32_t BME680_ENV_StartPeriodic(BME680_ENV_Callback cb, uint32_t period_ms);

/** \brief Stops periodic measurements and puts BME680 into sleep mode.
 *
 * \returns
 * 0 - On when BME680 was successfully put into sleep mode.<br>
 * BME680_E_* - Error code on failure.
 */
extern int32_t BME680_ENV_StopPeriodic(void);

#ifdef __cplusplus
}
#endif

#endif /* BME680_ENV_H_ */

//! \}
//! \}
//! \}

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
//! \file RTE_BDK_BME680_ENV.h
//!
//! This file contains the Run Time Configuration (RTE) options for BME680_ENV
//! CMSIS Component.
//!
//! These options can be edited by opening this file in CMSIS Configuration
//! Wizard Editor that is part of Eclipse installation.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup BME680_ENV_GRP
//! \{
//! \name Run Time Environment Configuration
//!
//! These parameters are part of the \ref RTE_BME680_ENV.h RTE configuration
//! file and can be used to adjust library behavior.
//! This file is copied into the Eclipse project when the BME680_ENV component
//! is selected and can be edited by using the <i>CMIS Configuration Wizard</i>
//! editor.
//!
//! \image html rte_bme680_env.jpg
//!
//-----------------------------------------------------------------------------

#ifndef RTE_BME680_ENV_H_
#define RTE_BME680_ENV_H_

#include <stdint.h>
#include <stdbool.h>

// <<< Use Configuration Wizard in Context Menu >>>

/** \def RTE_BME680_ENV_TEMP_ENABLE
 * \brief Enables or disables temperature measurement.
 *
 * Disabling is discouraged as other measurements depend on temperature
 * measurement for calibration.
 *
 * If disabled a constant value of INT16_MAX will be reported in
 * \ref BME680_ENV_Data::temperature.
 */
// <e> Temperature measurement
// <i> Enables temperature measurement.
// <i> Disabling is discouraged as other measurements depend on temperature measurement for calibration.
// <i> If disabled a constant value of INT16_MAX will be reported.
#ifndef RTE_BME680_ENV_TEMP_ENABLE
#define RTE_BME680_ENV_TEMP_ENABLE 1
#endif

#if defined RTE_BME680_ENV_TEMP_ENABLE == 1

/** \def RTE_BME680_ENV_OS_TEMP
 * \brief Temperature measurement oversampling setting.
 *
 * Oversampling improves temperature measurement resolution by reducing noise.
 * Higher oversampling values will result in longer measurement duration.
 *
 * \see
 * Section <b>3.3.1 Temperature Measurement</b> of BME680 datasheet for more
 * details.
 */
// <o> Oversampling
// <i> Oversampling improves temperature measurement resolution by reducing noise.
// <i> Higher oversampling values will result in longer measurement duration.
// <i> See section '3.3.1 Temperature Measurement' of BME680 datasheet for more details.
//    <1=> 1x
//    <2=> 2x
//    <3=> 4x
//    <4=> 8x
//    <5=> 16x
#define RTE_BME680_ENV_OS_TEMP         2

#else
#define RTE_BME680_ENV_OS_TEMP         0
#endif /* defined RTE_BME680_ENV_TEMP_ENABLE == 1 */

// </e>

/** \def RTE_BME680_ENV_PRES_ENABLE
 * \brief Enables or disables pressure measurement.
 *
 * If disabled a constant value of UINT32_MAX will be reported in
 * \ref BME680_ENV_Data::pressure.
 */
// <e> Pressure measurement
// <i> Enables pressure measurement.
// <i> If disabled a constant value of UINT32_MAX will be reported.
#ifndef RTE_BME680_ENV_PRES_ENABLE
#define RTE_BME680_ENV_PRES_ENABLE 1
#endif

#if defined RTE_BME680_ENV_PRES_ENABLE == 1

/** \def RTE_BME680_ENV_OS_PRES
 * \brief Pressure measurement oversampling setting.
 *
 * Oversampling improves pressure measurement resolution by reducing noise.
 * Higher oversampling values will result in longer measurement duration.
 *
 * \see
 * Section <b>3.3.2 Pressure Measurement</b> of BME680 datasheet for more
 * details.
 */
// <o> Oversampling
// <i> Oversampling improves pressure measurement resolution by reducing noise.
// <i> Higher oversampling values will result in longer measurement duration.
// <i> See section '3.3.2 Pressure Measurement' of BME680 datasheet for more details.
//    <1=> 1x
//    <2=> 2x
//    <3=> 4x
//    <4=> 8x
//    <5=> 16x
#define RTE_BME680_ENV_OS_PRES         2

#else
#define RTE_BME680_ENV_OS_PRES         0
#endif /* defined RTE_BME680_ENV_PRES_ENABLE == 1 */

// </e>

/** \def RTE_BME680_ENV_HUM_ENABLE
 * \brief Enables or disables humidity measurement.
 *
 * If disabled a constant value of UINT32_MAX will be reported in
 * \ref BME680_ENV_Data::humidity.
 */
// <e> Humidity measurement
// <i> Enables humidity measurement
// <i> If disabled a constant value of UINT32_MAX will be reported.
#ifndef RTE_BME680_ENV_HUM_ENABLE
#define RTE_BME680_ENV_HUM_ENABLE 1
#endif

#if defined RTE_BME680_ENV_HUM_ENABLE == 1

/** \def RTE_BME680_ENV_OS_HUM
 * \brief Humidity measurement oversampling setting.
 *
 * Oversampling improves humidity measurement by reducing noise.
 * Higher oversampling values will result in longer measurement duration.
 *
 * \see
 * Section <b>3.3.3 Humidity Measurement</b> of BME680 datasheet for more
 * details.
 */
// <o> Oversampling
// <i> Higher oversampling setting reduces noise.
// <i> Higher oversampling values will result in longer measurement duration.
// <i> See section '3.3.3 Humidity Measurement' of BME680 datasheet for more details.
//    <1=> 1x
//    <2=> 2x
//    <3=> 4x
//    <4=> 8x
//    <5=> 16x
#ifndef RTE_BME680_ENV_OS_HUM
#define RTE_BME680_ENV_OS_HUM          2
#endif

#else
#define RTE_BME680_ENV_OS_HUM          0
#endif /* defined RTE_BME680_ENV_HUM_ENABLE == 1 */

// </e>

/** \def RTE_BME680_ENV_FILTER_SIZE
 * \brief IIR filter coefficient value.
 *
 * Reduces the bandwidth of temperature and pressure output signals and
 * increases the resolution of output data to 20 bit.
 *
 * \see
 * Section <b>3.3.4 IIR filter</b> of BME680 datasheet for more details.
 */
// <o> IIR Filter Size
// <i> Reduces the bandwidth of temperature and pressure output signals and increases the resolution of output data to 20 bit.
// <i> See section '3.3.4 IIR filter' of BME680 datasheet for more details.
//    <0=> 0
//    <1=> 1
//    <2=> 3
//    <3=> 7
//    <4=> 15
//    <5=> 31
//    <6=> 63
//    <7=> 127
#ifndef RTE_BME680_ENV_FILTER_SIZE
#define RTE_BME680_ENV_FILTER_SIZE     1
#endif

// <<< end of configuration section >>>

#endif /* RTE_BME680_ENV_H_ */

//! \}
//! \}
//! \}
//! \}

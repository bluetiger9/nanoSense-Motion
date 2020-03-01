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
//! \file NOA1305_ALS.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup NOA1305_ALS_GRP NOA1305 Ambient Light Sensor
//! \{
//!
//! \brief Library for measuring of Ambient Light Intensity using NOA1305 ALS
//! sensor.
//!
//! This library defines common API for NOA1305 sensors across all compatible
//! evaluation boards.
//!
//-----------------------------------------------------------------------------
#ifndef NOA1305_ALS_H_
#define NOA1305_ALS_H_

#include <stdbool.h>
#include <stdint.h>

// Platform independent library for reading / writing of NOA1305 I2C registers.
#include <noa1305.h>

#ifdef __cplusplus
extern "C"
{
#endif

//! \name ALS-GEVB IDK Shield definitions
//!
//! These definitions are related to the ALS-GEVB shield that can be used to
//! extend functionality of IDK / BDK compatible boards with PMOD connector.
//!
//! This shield utilizes PCA9655E I2C IO Expander to relay
//! \{
//!

#define NOA1305_ALS_IOEXP_ADDR         (0x11)

#define NOA1305_ALS_IOEXP_INT_PORT     (PCA9655E_PORT_INPUT0)

#define NOA1305_ALS_IOEXP_INT_PIN      (PCA9655E_PIN0)

//! \}

/** \brief Callback function prototype used to report light level to
 * application.
 *
 * \param lux
 * Latest measured ambient light value measured by sensor.
 *
 * \param is_interrupt
 * false - If this is periodical measurement.<br>
 * true - If callback was called as result of light value crossing the threshold
 * value.
 */
typedef void(*NOA1305_ALS_ReadCallback)(uint32_t lux, bool is_interrupt);

/** \brief Initializes NOA1305 and puts it into power down mode.
 *
 * \param I_k
 * Integration constant used for conversion of sensor tick count to LUX.
 * Datasheet specifies value of 7.7 (I_k = 7700) as integration constant for
 * white LED light source.<br>
 * Value has to be multiplied by scale factor of 1000
 * (NOA1305_INTEG_CONSTANT_DIV) to allow for integer based computation.
 *
 * \param T_i
 * Integration time setting to be used by sensor.
 * Use one of <b>NOA1305_INTEG_TIME_[ 800MS | 400MS | 200MS | 100MS | 50MS |
 * 25MS | 12p5MS | 6p25MS]</b>
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \b NOA1305_E_* - Error code on failure.
 *
 */
extern int32_t NOA1305_ALS_InitializeParam(uint32_t I_k, uint8_t T_i);

/** \brief Initializes NOA1305 and puts it into power down mode.
 *
 * This function uses predefined sensor parameters.
 *
 * \see NOA1305_ALS_InitializeParam
 */
static inline int32_t NOA1305_ALS_Initialize(void)
{
	return NOA1305_ALS_InitializeParam(7700, NOA1305_INTEG_TIME_200MS);
}

/** \brief Powers on the sensor for enough time to get single measurement and
 * calls given callback function.
 *
 * The callback is called after 4 integration cycles have passed to allow for
 * sensor output to stabilize.
 *
 * Sensor will be put back to power down state after measurement is complete.
 *
 * \param cb
 * Optional callback to be called when measurement is completed.
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \b NOA1305_E_* - Error code on failure.
 */
extern int32_t NOA1305_ALS_StartOneShot(NOA1305_ALS_ReadCallback cb);

/** \brief Powers on the sensor to continuously update output value every
 * integration cycle.
 *
 * Sensor will remain powered on until \ref NOA1305_ALS_Stop is called.
 *
 * \param period_ms
 * Period with which to call provided callback in milliseconds.
 *
 * Use zero to disable periodic callback calling.
 * If periodic calling is disabled, the callback will be called only in case of
 * interrupt threshold event.
 *
 * \param
 * Optional callback to be called periodically when \p period_ms is non zero
 * or when threshold interrupt is generated.
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \b NOA1305_E_* - Error code on failure.
 */
extern int32_t NOA1305_ALS_StartContinuous(uint32_t period_ms, NOA1305_ALS_ReadCallback cb);

/** \brief Powers off the sensor and disables periodic and interrupt callbacks.
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \b NOA1305_E_* - Error code on failure.
 */
extern int32_t NOA1305_ALS_Stop(void);

/** \brief Reads latest measured lux value from sensor.
 *
 * \param lux
 * Pointer to variable where to store read lux value.
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \b NOA1305_E_* - Error code on failure.
 */
extern int32_t NOA1305_ALS_ReadLux(uint32_t *lux);

/** \brief Enables interrupt generation when sensor output value passes above or
 * below defined threshold.
 *
 * Sensor needs to be in continuous measurement mode with an callback function
 * attached to be able to notify the application.
 *
 * Interrupt event caused by interrupt enable operation will be ignored.
 *
 * \param lux_threshold
 * Lux level to use as threshold for interrupt generation.
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \ref NOA1305_E_UNSUPPORTED - If interrupt support is disabled in RTE configuration.<br>
 * \b NOA1305_E_* - Error code on failure.
 */
extern int32_t NOA1305_ALS_EnableInterrupt(uint32_t lux_threshold);

/** \brief Disables interrupt generation if it was previously enabled.
 *
 *
 * Interrupt event caused by interrupt disable operation will be ignored.
 *
 * \returns
 * \ref NOA1305_OK - On success.<br>
 * \ref NOA1305_E_UNSUPPORTED - If interrupt support is disabled in RTE configuration.<br>
 * \b NOA1305_E_* - Error code on failure.
 */
extern int32_t NOA1305_ALS_DisableInterrupt(void);


#ifdef __cplusplus
}
#endif

#endif /* NOA1305_ALS_H_ */

//! \}
//! \}
//! \}

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
//! \file BHI160_NDOF.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup BHI160_NDOF_GRP BHI160
//!
//! \brief Ultra low-power sensor hub incl. integrated IMU
//!
//! This library implements platform specific configuration of BHI160 sensor hub
//! and BMM150 magnetometer for HB-GEVB evaluation board.
//! This includes sensor initialization, loading of correct RAM patch and
//! automatic scheduling of FIFO read operations when IRQ is received from
//! BHI160.
//!
//! Only basic functionality of Nine Degrees of Freedom (NDOF) related sensors
//! is provided on this level.
//! To gain more control over the sensor hub or enabling of additional virtual
//! sensors it is recommended to use underlying BHy library APIs and use this
//! layer only for initialization.
//!
//!
//! \section bhi160_coord Coordinate System (HB-GEVB)
//!
//! The coordinate system used by this library for HB-GEVB board is <b>ENU
//! coordinate system</b> (East, North, Up):
//!
//! - \b x points east and is tangential to the ground.
//! - \b y points north and is tangential to the ground.
//! - \b z points towards sky and is perpendicular to the ground.
//!
//! \image html enu_coordinates.png "Figure 1: Coordinate system relative to ENU convention"
//!
//! This library provides axis translation matrixes defined in \ref
//! BHI160_NDOF_BHI160_AXIS_MAPPING and \ref BHI160_NDOF_BMM150_AXIS_MAPPING
//! macros which take into account relative positions of BHI160 (accelerometer,
//! gyroscope) and BMM150 (magnetometer) chips which are used to compute board
//! orientation.
//! The resulting board coordinate system can be seen in <b>Figure 2</b>.
//!
//! \image html hb_enu_coordinates.png "Figure 2: ENU coordinate system used for HB-GEVB" width=480px
//!
//! \see <b>Sensor Types</b> section of Android Sensors documentation
//! [[link](https://source.android.com/devices/sensors/sensor-types)]
//!
//! \see <b>Axes remapping of BHA250(B) / BHI160(B)</b> application note.
//! [[link](https://www.bosch-sensortec.com/bst/support_tools/downloads/overview_downloads)]
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BHI160_NDOF_H_
#define BHI160_NDOF_H_

#include <stdbool.h>
#include <stdint.h>

#include <bhy.h>
#include <bhy_uc_driver.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Translation matrix used to rotate axes of accelerometer and
 * gyroscope within BHI160.
 *
 * Can contain only values of -1, 0 or 1.
 */
#define BHI160_NDOF_BHI160_AXIS_MAPPING \
        { 0, -1,  0, \
          1,  0,  0, \
          0,  0,  1 }

/** \brief Translation matrix used to rotate axes of BMM150 magnetometer
 * to adjust for different package orientation on the PCB.
 *
 * Can contain only values of -1, 0 or 1.
 */
#define BHI160_NDOF_BMM150_AXIS_MAPPING \
        { -1,  0,  0, \
           0,  1,  0, \
           0,  0, -1 }

/** \brief These macros can be used with BHI160_NDOF_SetPowerMode to modify the
 * behavior of BHI160.
 */
enum BHI160_NDOF_PowerMode
{
    /** Both BHI160 and Application Processor (AP) are operating normally. */
    BHI160_NDOF_PM_NORMAL,

    /** Tells BHI160 that Application Processor is suspended and only sensors
     * configured with wake-up setting can trigger an host interrupt request.
     */
    BHI160_NDOF_PM_AP_SUSPEND,

    /** BHI160 algorithm is paused and all sensors are deactivated to save
     * power.
     */
    BHI160_NDOF_PM_STANDBY
};

/** \brief List of NDOF related sensor outputs of BHI160 sensor hub.
 *
 * Additional virtual sensors can be enabled by using their respective sensor
 * IDs (VS_TYPE_*) or using BHy library directly.
 */
enum BHI160_NDOF_Sensor
{
    BHI160_NDOF_S_ORIENTATION          = VS_TYPE_ORIENTATION,
    BHI160_NDOF_S_GRAVITY              = VS_TYPE_GRAVITY,
    BHI160_NDOF_S_LINEAR_ACCELERATION  = VS_TYPE_LINEAR_ACCELERATION,
    BHI160_NDOF_S_RATE_OF_ROTATION     = VS_TYPE_GYROSCOPE,
    BHI160_NDOF_S_MAGNETIC_FIELD       = VS_TYPE_GEOMAGNETIC_FIELD
};

/** \brief Callback function called by BHy library when new data are available
 * for given virtual sensor.
 *
 * \param data
 * Pointer to generic structure capable of holding data of all virtual sensors
 * supported by BHI160.
 * The \p sensor parameter has to be examined to determine the data format
 * stored within this structure.
 *
 * \param sensor
 * Sensor ID that can be used to determine how to process \p data.
 * Possible values are one of the VS_TYPE_* macros defined by BHy library.
 *
 * \see Section <b>12. Sensor Data Types and Output Format</b> of <i>BHI160
 * Datasheet</i> for expected data types of different virtual sensors.
 */
typedef void (*BHI160_NDOF_SensorCallback)(bhy_data_generic_t *data,
        bhy_virtual_sensor_t sensor);

/** \brief Initializes the sensor hub.
 *
 * The following initialization steps are performed:
 * 1. Software reset of BHI160.
 * 2. RAM patch for BMM150 magnetometer support is uploaded into BHI160.
 * 3. Sensor axes are remapped to correspond to HB-GEVB sensor placement.
 * 4. Timestamp callback is installed to update local timestamp on each change.
 * 5. Sensor Dynamic range callback is installed to update locally stored sensor
 *    ranges.
 * 6. Interrupt is enabled for BHI160 IRQ signal.
 *    FIFO processing routine will be scheduled on every rising edge of BHI160
 *    IRQ signal.
 *
 * \note
 * The speed of I2C transactions is not managed in this library.
 * Bus speed is set globally using HAL_I2C_SetBusSpeed by application.
 * Slow I2C speeds may result in limited throughput of sensor data and longer
 * initialization time since RAM patch needs to be loaded into BHI160.
 *
 * \returns
 * \b BHY_SUCCESS - When sensor was successfully initialized.<br>
 * BHY library error code on failure.
 */
extern int32_t BHI160_NDOF_Initialize(void);

/** \brief Restores DIO pad configuration after waking up from deep sleep mode.
 *
 * Needs to be called before PAD retention is disabled.
 */
extern void BHI160_NDOF_PadRestore(void);

/** \brief
 *
 * This function is intended to be used from within virtual sensor callbacks.
 * When called from sensor callback it will report timestamp of the measurement
 * that is being processed.
 *
 * \returns
 * 32-bit BHI160 timestamp.
 */
extern uint32_t BHI160_NDOF_GetTimestamp(void);

/** \brief Returns latest reported timestamp converted to seconds.
 *
 * \returns
 * BHI160 timestamp converted to seconds.
 *
 * \see BHI160_NDOF_GetTimestamp
 */
static inline float BHI160_NDOF_GetTimestampSeconds(void)
{
    return BHI160_NDOF_GetTimestamp() / 32000.0f;
}

/** \brief Can be used to temporarily disable BHI160 outputs to save power.
 *
 * \param power_mode
 * Can be one of BHI160_NDOF_PM_[ NORMAL | AP_SUSPEND | STANDBY ].
 *
 * \returns
 * \b BHY_SUCCESS - On success.<br>
 * BHY library error code on failure.
 */
extern int32_t BHI160_NDOF_SetPowerMode(enum BHI160_NDOF_PowerMode power_mode);

/** \brief Enables the output of desired virtual sensor.
 *
 * For more detailed configuration it is possible to use the
 * underlying \b bhy_install_sensor_callback and \b bhy_enable_virtual_sensor
 * API functions directly.
 *
 * \param sensor
 * Selects which virtual sensor will be enabled.
 * Can be one of BHI160_NDOF_S_* or VS_TYPE_* macros.
 *
 * \param cb
 * Application specific callback function that will be called every time when
 * new data are read from FIFO.
 *
 * \param sample_rate
 * The desired rate at which new data will be inserted into FIFO in samples per
 * second.
 *
 * \returns
 * \b BHY_SUCCESS - On success.<br>
 * BHY library error code on failure.
 */
extern int32_t BHI160_NDOF_EnableSensor(enum BHI160_NDOF_Sensor sensor,
        BHI160_NDOF_SensorCallback cb, uint16_t sample_rate);

/** \brief Returns current dynamic range for physical accelerometer sensor.
 *
 * It is needed to convert data output reported by BHI160 into SI units.
 * The value of dynamic range is automatically updated when BHI160 reports range
 * change.
 *
 * Example conversion from reported data into acceleration in \b [g] :
 * \code
 * void accel_callback(bhy_data_generic_t *data, bhy_virtual_sensor_t sensor)
 * {
 *     uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();
 *
 *     float x = data->data_vector.x / 32768.0f * dyn_range;
 *     float y = data->data_vector.y / 32768.0f * dyn_range;
 *     float z = data->data_vector.z / 32768.0f * dyn_range;
 *
 *     // Processing
 * }
 * \endcode
 *
 * \returns
 * Accelerometer dynamic range in \b [g].
 * Typically 1g, 2g or 4g.
 */
extern uint16_t BHI160_NDOF_GetAccelDynamicRange(void);

/** \brief Returns current dynamic range for physical gyroscope sensor.
 *
 * It is needed to convert data output reported by BHI160 into SI units.
 * The value of dynamic range is automatically updated when BHI160 reports range
 * change.
 *
 * Example conversion from reported data into rate of rotation in degrees per
 * second:
 * \code
 * void gyro_callback(bhy_data_generic_t *data, bhy_virtual_sensor_t sensor)
 * {
 *     uint16_t dyn_range = BHI160_NDOF_GetGyroDynamicRange();
 *
 *     float x = data->data_vector.x / 32768.0f * dyn_range;
 *     float y = data->data_vector.y / 32768.0f * dyn_range;
 *     float z = data->data_vector.z / 32768.0f * dyn_range;
 *
 *     // Processing
 * }
 * \endcode
 *
 * \returns
 * Gyroscope dynamic range in <b>degree per second</b>.
 * Typically 2000 degrees per second.
 */
extern uint16_t BHI160_NDOF_GetGyroDynamicRange(void);

/** \brief Returns current dynamic range for physical magnetometer sensor.
 *
 * It is needed to convert data output reported by BHI160 into SI units.
 * The value of dynamic range is automatically updated when BHI160 reports range
 * change.
 *
 * Example conversion from reported data into magnetic field intensity in \b uT:
 * \code
 * void mag_callback(bhy_data_generic_t *data, bhy_virtual_sensor_t sensor)
 * {
 *     uint16_t dyn_range = BHI160_NDOF_GetMagDynamicRange();
 *
 *     float x = data->data_vector.x / 32768.0f * dyn_range;
 *     float y = data->data_vector.y / 32768.0f * dyn_range;
 *     float z = data->data_vector.z / 32768.0f * dyn_range;
 *
 *     // Processing
 * }
 * \endcode
 *
 * \returns
 * Magentometer dynamic range in \b [uT].
 * Typically 1000 uT or 2000 uT.
 */
extern uint16_t BHI160_NDOF_GetMagDynamicRange(void);


#ifdef __cplusplus
}
#endif

#endif /* BHI160_NDOF_H_ */

//! \}
//! \}
//! \}

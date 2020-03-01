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
//! \file BSEC_ENV.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup BSEC_ENV_GRP BSEC library + BME680 Environmental Sensor
//!
//! \brief Environmental sensor measuring Indoor Air Quality, Temperature,
//! Humidity and Pressure.
//!
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BSEC_ENV_H_
#define BSEC_ENV_H_

#include <stdbool.h>
#include <stdint.h>

#include <bme680.h>
#include <bsec_interface.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
    int64_t timestamp;

    float iaq;
    float static_iaq;
    float co2_equivalent;
    float breath_voc_equivalent;
    float temperature;
    float humidity;
    float raw_pressure;

    uint8_t iaq_accuracy : 4;
    uint8_t static_iaq_accuracy : 4;
    uint8_t co2_accuracy : 4;
    uint8_t breath_voc_accuracy : 4;
    uint8_t temperature_accuracy : 4;
    uint8_t humidity_accuracy : 4;
} bsec_env_output_struct;


/** \brief System specific implementation of sleep function.
 *
 * \param t_ms
 * Time in milliseconds.
 */
typedef void (*bsec_env_sleep_fct)(uint32_t t_ms);

/** \brief Capture system time in microseconds.
 *
 * \returns Current system timestamp in microseconds.
 */
typedef int64_t (*bsec_env_get_timestamp_us_fct)();

/** \brief Load library configuration from non-volatile memory.
 *
 * \param config_buffer
 * Buffer to hold the loaded configuration string.
 *
 * \param buf_len
 * Size of the allocated config_buffer.
 *
 * \returns
 * Number of bytes copied into \p config_buffer.
 */
typedef uint32_t (*bsec_env_config_load_fct)(uint8_t *config_bufer, uint32_t bufer_len);

/** \brief Load previous library state from non-volatile memory.
 *
 * \param state_buffer
 * Buffer to hold the loaded state string.
 *
 * \param buffer_len
 * Size of the allocated state_buffer.
 *
 * \returns
 * Number of bytes copied into state_buffer.
 */
typedef uint32_t (*bsec_env_state_load_fct)(uint8_t *state_buffer, uint32_t buffer_len);

/** \brief Save library state to non-volatile memory.
 *
 * \param state_buffer
 * Buffer holding the state to be stored.
 *
 * \param buffer_len
 * Length of the state string to be stored.
 */
typedef void (*bsec_env_state_save_fct)(uint8_t *state_buffer, uint32_t size);

/** \brief Handling of the ready outputs.
 *
 * This callback is called whenever there are new virtual sensor data available.
 *
 * \param output
 * Pointer to structure filled with the latest data.
 * Value f this parameter is valid only withing the context of this function.
 * Sensor outputs have to be copied into application memory if they need to be
 * processed outside of this function.
 */
typedef void (*bsec_output_fct)(bsec_env_output_struct *output);

/** \brief Joint return value containing status of bme680 driver library and
 * BSEC library.
 */
typedef struct
{
    int8_t bme680_status;

    /** \see \ref bsec_library_return_t */
    bsec_library_return_t bsec_status;
} bsec_env_return_val;

/** \brief Structure for containing of all application specific functions and
 * parameters used by BSEC initialization routine.
 */
typedef struct
{
    /** \brief Device-specific temperature offset (due to self-heating). */
    float temperature_offset;

    /** \brief Pointer to the system-specific sleep function.
     *
     * It is used to wait for BME680 after software restart of the chip.
     */
    bsec_env_sleep_fct sleep;

    /** \brief Pointer to the system-specific configuration load function.
     *
     * Set to NULL pointer to use default configuration.
     */
    bsec_env_config_load_fct config_load;

    /** \brief Pointer to the system-specific state load function.
     *
     * Set to NULL pointer if saved status is not available / used.
     */
    bsec_env_state_load_fct state_load;

    /** \brief */
    bsec_sensor_configuration_t requested_virtual_sensors[BSEC_NUMBER_OUTPUTS];

    /** \brief */
    uint8_t n_requested_virtual_sensors;
} bsec_env_init_struct;

/** \brief Structure containing all application specific functions and
 * parameters used by BSEC during data retrieval & processing.
 */
typedef struct
{
    /** \brief Pointer to the system-specific sleep function. */
    bsec_env_sleep_fct sleep;

    /** \brief Pointer to the system-specific timestamp derivation function. */
    bsec_env_get_timestamp_us_fct get_timestamp_us;

    /** \brief Pointer to the system specific output processing function. */
    bsec_output_fct output_ready;

    /** \brief Pointer to the system-specific state save function.
     *
     * Set to NULL pointer if state saving should be disabled.
     */
    bsec_env_state_save_fct save_state;

    /** \brief Interval at which BSEC state should be saved (in samples). */
    uint32_t save_interval;
} bsec_env_process_struct;

typedef enum {
    BSEC_ENV_PROCESS_STATE_IDLE = 0,
    BSEC_ENV_PROCESS_STATE_MEASURING,
} bsec_env_process_state;

/** \brief Initialize the BME680 sensor and the BSEC library.
 *
 * This function does the following:
 *
 * 1. Initializes BME680 sensor using low level driver library using I2C
 *    interface.
 * 2. Initializes BSEC library.
 * 3. Attempts to load custom BSEC configuration if \ref
 *    bsec_env_init_struct.config_load function was provided.
 *    Default BSEC configuration will be used otherwise.
 * 4. Attempts to load previously saved BSEC state from non-volatile memory if
 *    \ref bsec_env_init_struct.state_load function was provided.
 *    BSEC will start from clean state and start calibrating sensors from
 *    scratch if no state load function is provided.
 * 5. Virtual sensor subscriptions will be updated according to \ref
 *    bsec_env_init_struct.requested_virtual_sensors .
 *    New sensor data will be calculated only for subscribed sensors.
 *
 * \param init_struct
 * Pointer to structure containing initialization parameters and functions
 * needed for initialization.
 *
 * \returns
 * Status codes of bme680 driver library and BSEC status code.
 */
extern bsec_env_return_val BSEC_ENV_Initialize(
        bsec_env_init_struct *init_struct);

/** \brief Execute one cycle of BSEC processing routine.
 *
 * This function determines when and what data are to be measured by the BME680
 * sensor followed by processing stage.
 * After the function completes its current processing routine it will return a
 * time delay in milliseconds after which it should be called again.
 *
 * Calling this function will result on one of the following two routines to be
 * executed:
 *
 * * <b>Measurement start routine</b>
 *
 *   It is used to start new measurement.
 *
 *   1. Determine BME680 configuration parameters for next measurement.
 *   2. Load parameters into BME680 and start one shot measurement.
 *   3. Calculate time required by sensor to complete measurement and return
 *      it as delay in ms.
 *
 * * <b>Measurement processing routine</b>
 *
 *   It is used to process measurement results and output new sensor data.
 *
 *   1. Determine if measurement has completed and BME680 is idle.
 *   2. Read sensor data from BME680.
 *   3. Pass sensor data to BSEC for processing.
 *   4. Parse BSEC virtual sensor output and call application provided output
 *      function ( \ref bsec_env_process_struct.output_ready ) when new data
 *      are available.
 *   5. Save current BSEC library state to non-volatile memory if \ref
 *      bsec_env_process_struct.save_state function was provided and \ref
 *      bsec_env_process_struct.save_interval measurements completed since last
 *      save operation.
 *   6. Calculate when new measurement should be started and return time offset
 *      in ms after which this function should be called again.
 *
 * \param process_struct
 * Pointer to structure containing runtime parameters and functions
 * needed for processing of data.
 *
 * \returns
 * Time in milliseconds after which this function should be called again.
 */
extern int64_t BSEC_ENV_Process(bsec_env_process_struct *process_struct);


#ifdef __cplusplus
}
#endif

#endif /* BSEC_ENV_H_ */

//! \}
//! \}
//! \}

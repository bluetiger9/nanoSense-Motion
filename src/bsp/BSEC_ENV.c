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
//! \file BSEC_ENV.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup BSEC_ENV_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <BDK.h>
#include <BSEC_ENV.h>

#include <bsec_interface.h>
#include <bme680.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

/* Global sensor APIs data structure */
static struct bme680_dev bme680_g;

/* Global temperature offset to be subtracted */
static float bme680_temperature_offset_g = 0.0f;



//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

static int8_t BSEC_ENV_I2C_Read(uint8_t dev_id, uint8_t reg_addr,
        uint8_t *reg_data, uint16_t len)
{
    int32_t status;

    status = HAL_I2C_Write(dev_id, &reg_addr, 1, false);
    if (status != HAL_OK)
    {
        return status;
    }

    return HAL_I2C_Read(dev_id, reg_data, len, false);
}

static int8_t BSEC_ENV_I2C_Write(uint8_t dev_id, uint8_t reg_addr,
        uint8_t *reg_data, uint16_t len)
{
    int32_t status;

    uint8_t *write_data = malloc(len + 1);
    if (write_data == NULL)
    {
        return -1;
    }
    write_data[0] = reg_addr;
    memcpy(write_data + 1, reg_data, len);

    status = HAL_I2C_Write(dev_id, write_data, len + 1, false);

    free(write_data);
    return status;
}

static uint32_t BSEC_ENV_TriggerMeasurement(bsec_bme_settings_t *sensor_settings)
{
    uint16_t meas_period;
    uint8_t set_required_settings;
    int8_t bme680_status = BME680_OK;

    /* Check if a forced-mode measurement should be triggered now */
    if (sensor_settings->trigger_measurement)
    {
        /* Set sensor configuration */

        bme680_g.tph_sett.os_hum  = sensor_settings->humidity_oversampling;
        bme680_g.tph_sett.os_pres = sensor_settings->pressure_oversampling;
        bme680_g.tph_sett.os_temp = sensor_settings->temperature_oversampling;
        bme680_g.gas_sett.run_gas = sensor_settings->run_gas;
        bme680_g.gas_sett.heatr_temp = sensor_settings->heater_temperature; /* degree Celsius */
        bme680_g.gas_sett.heatr_dur  = sensor_settings->heating_duration; /* milliseconds */

        /* Select the power mode */
        /* Must be set before writing the sensor configuration */
        bme680_g.power_mode = BME680_FORCED_MODE;
        /* Set the required sensor settings needed */
        set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_GAS_SENSOR_SEL;

        /* Set the desired sensor configuration */
        bme680_status = bme680_set_sensor_settings(set_required_settings, &bme680_g);
        ASSERT_DEBUG(bme680_status == BME680_OK);

        /* Set power mode as forced mode and trigger forced mode measurement */
        bme680_status = bme680_set_sensor_mode(&bme680_g);
        ASSERT_DEBUG(bme680_status == BME680_OK);

        /* Get the total measurement duration so as to sleep or wait till the measurement is complete */
        bme680_get_profile_dur(&meas_period, &bme680_g);

        return meas_period;
    }

    return 0;
}

static void BSEC_ENV_ReadData(int64_t time_stamp_trigger, bsec_input_t *inputs,
        uint8_t *num_bsec_inputs, int32_t bsec_process_data)
{
    static struct bme680_field_data data;
    int8_t bme680_status = BME680_OK;

    /* We only have to read data if the previous call the bsec_sensor_control() actually asked for it */
    if (bsec_process_data)
    {
        bme680_status = bme680_get_sensor_data(&data, &bme680_g);
        ASSERT_DEBUG(bme680_status == BME680_OK);

        if (data.status & BME680_NEW_DATA_MSK)
        {
            /* Pressure to be processed by BSEC */
            if (bsec_process_data & BSEC_PROCESS_PRESSURE)
            {
                /* Place presssure sample into input struct */
                inputs[*num_bsec_inputs].sensor_id = BSEC_INPUT_PRESSURE;
                inputs[*num_bsec_inputs].signal = data.pressure;
                inputs[*num_bsec_inputs].time_stamp = time_stamp_trigger;
                (*num_bsec_inputs)++;
            }
            /* Temperature to be processed by BSEC */
            if (bsec_process_data & BSEC_PROCESS_TEMPERATURE)
            {
                /* Place temperature sample into input struct */
                inputs[*num_bsec_inputs].sensor_id = BSEC_INPUT_TEMPERATURE;
#ifdef BME680_FLOAT_POINT_COMPENSATION
                inputs[*num_bsec_inputs].signal = data.temperature;
#else
                inputs[*num_bsec_inputs].signal = data.temperature / 100.0f;
#endif
                inputs[*num_bsec_inputs].time_stamp = time_stamp_trigger;
                (*num_bsec_inputs)++;

                /* Also add optional heatsource input which will be subtracted from the temperature reading to
                 * compensate for device-specific self-heating (supported in BSEC IAQ solution)*/
                inputs[*num_bsec_inputs].sensor_id = BSEC_INPUT_HEATSOURCE;
                inputs[*num_bsec_inputs].signal = bme680_temperature_offset_g;
                inputs[*num_bsec_inputs].time_stamp = time_stamp_trigger;
                (*num_bsec_inputs)++;
            }
            /* Humidity to be processed by BSEC */
            if (bsec_process_data & BSEC_PROCESS_HUMIDITY)
            {
                /* Place humidity sample into input struct */
                inputs[*num_bsec_inputs].sensor_id = BSEC_INPUT_HUMIDITY;
#ifdef BME680_FLOAT_POINT_COMPENSATION
                inputs[*num_bsec_inputs].signal = data.humidity;
#else
                inputs[*num_bsec_inputs].signal = data.humidity / 1000.0f;
#endif
                inputs[*num_bsec_inputs].time_stamp = time_stamp_trigger;
                (*num_bsec_inputs)++;
            }
            /* Gas to be processed by BSEC */
            if (bsec_process_data & BSEC_PROCESS_GAS)
            {
                /* Check whether gas_valid flag is set */
                if (data.status & BME680_GASM_VALID_MSK)
                {
                    /* Place sample into input struct */
                    inputs[*num_bsec_inputs].sensor_id = BSEC_INPUT_GASRESISTOR;
                    inputs[*num_bsec_inputs].signal = data.gas_resistance;
                    inputs[*num_bsec_inputs].time_stamp = time_stamp_trigger;
                    (*num_bsec_inputs)++;
                }
            }
        }
    }
}

/*!
 * @brief       This function is written to process the sensor data for the requested virtual sensors
 *
 * @param[in]   bsec_inputs         input structure containing the information on sensors to be passed to do_steps
 * @param[in]   num_bsec_inputs     number of inputs to be passed to do_steps
 * @param[in]   output_ready        pointer to the function processing obtained BSEC outputs
 *
 * @return      none
 */
static void BSEC_ENV_ProcessData(bsec_input_t *bsec_inputs, uint8_t num_bsec_inputs, bsec_env_process_struct *process_struct)
{
    /* Output buffer set to the maximum virtual sensor outputs supported */
    bsec_output_t bsec_outputs[BSEC_NUMBER_OUTPUTS];
    uint8_t num_bsec_outputs = 0;
    uint8_t index = 0;

    bsec_library_return_t bsec_status = BSEC_OK;

    bsec_env_output_struct output = { 0 };

    /* Check if something should be processed by BSEC */
    if (num_bsec_inputs > 0)
    {
        /* Set number of outputs to the size of the allocated buffer */
        /* BSEC_NUMBER_OUTPUTS to be defined */
        num_bsec_outputs = BSEC_NUMBER_OUTPUTS;

        /* Perform processing of the data by BSEC
           Note:
           * The number of outputs you get depends on what you asked for during bsec_update_subscription(). This is
             handled under bme680_bsec_update_subscription() function in this example file.
           * The number of actual outputs that are returned is written to num_bsec_outputs. */
        bsec_status = bsec_do_steps(bsec_inputs, num_bsec_inputs, bsec_outputs,
                &num_bsec_outputs);
        ASSERT_DEBUG(bsec_status == BME680_OK);

        /* Iterate through the outputs and extract the relevant ones. */
        for (index = 0; index < num_bsec_outputs; index++)
        {
            switch (bsec_outputs[index].sensor_id)
            {
                case BSEC_OUTPUT_IAQ:
                    output.iaq = bsec_outputs[index].signal;
                    output.iaq_accuracy = bsec_outputs[index].accuracy;
                    break;
                case BSEC_OUTPUT_STATIC_IAQ:
                    output.static_iaq = bsec_outputs[index].signal;
                    output.static_iaq_accuracy = bsec_outputs[index].accuracy;
                    break;
                case BSEC_OUTPUT_CO2_EQUIVALENT:
                    output.co2_equivalent = bsec_outputs[index].signal;
                    output.co2_accuracy = bsec_outputs[index].accuracy;
                    break;
                case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
                    output.breath_voc_equivalent = bsec_outputs[index].signal;
                    output.breath_voc_accuracy = bsec_outputs[index].accuracy;
                    break;
                case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
                    output.temperature = bsec_outputs[index].signal;
                    output.temperature_accuracy = bsec_outputs[index].accuracy;
                    break;
                case BSEC_OUTPUT_RAW_PRESSURE:
                    output.raw_pressure = bsec_outputs[index].signal;
                    break;
                case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
                    output.humidity = bsec_outputs[index].signal;
                    output.humidity_accuracy = bsec_outputs[index].accuracy;
                    break;
                default:
                    continue;
            }

            /* Assume that all the returned timestamps are the same */
            output.timestamp = bsec_outputs[index].time_stamp;
        }

        if (process_struct->output_ready != NULL)
        {
            process_struct->output_ready(&output);
        }
    }
}

bsec_env_return_val BSEC_ENV_Initialize(bsec_env_init_struct *init_struct)
{
    bsec_env_return_val retval = { BME680_OK, BSEC_OK};

    if (init_struct == NULL)
    {
        retval.bme680_status = BME680_E_NULL_PTR;
        return retval;
    }

    /* Initialize BME680 */
    bme680_g.dev_id = BME680_I2C_ADDR_PRIMARY;
    bme680_g.intf = BME680_I2C_INTF;
    bme680_g.write = BSEC_ENV_I2C_Write;
    bme680_g.read = BSEC_ENV_I2C_Read;
    bme680_g.delay_ms = init_struct->sleep;

    retval.bme680_status = bme680_init(&bme680_g);
    if (retval.bme680_status != BME680_OK)
    {
        return retval;
    }

    /* Initialize BSEC library */
    retval.bsec_status = bsec_init();
    if (retval.bsec_status != BSEC_OK)
    {
        return retval;
    }

    /* Load library config, if available */
    if (init_struct->config_load != NULL)
    {
        uint32_t bsec_config_len = 0;
        uint8_t bsec_config[BSEC_MAX_PROPERTY_BLOB_SIZE];
        uint8_t work_buffer[BSEC_MAX_PROPERTY_BLOB_SIZE];

        bsec_config_len = init_struct->config_load(bsec_config,
                sizeof(bsec_config));
        if (bsec_config_len != 0)
        {
            retval.bsec_status = bsec_set_configuration(bsec_config,
                    bsec_config_len, work_buffer, sizeof(work_buffer));
            if (retval.bsec_status != BSEC_OK)
            {
                return retval;
            }
        }
    }

    /* Load previous library state form non-volatile memory, if available */
    if (init_struct->state_load != NULL)
    {
        uint32_t bsec_state_len = 0;
        uint8_t bsec_state[BSEC_MAX_PROPERTY_BLOB_SIZE];
        uint8_t work_buffer[BSEC_MAX_PROPERTY_BLOB_SIZE];

        bsec_state_len = init_struct->state_load(bsec_state,
                sizeof(bsec_state));
        if (bsec_state_len != 0)
        {
            retval.bsec_status = bsec_set_state(bsec_state, bsec_state_len,
                    work_buffer, sizeof(work_buffer));
            if (retval.bsec_status != BSEC_OK)
            {
                return retval;
            }
        }
    }

    bme680_temperature_offset_g = init_struct->temperature_offset;

    if (init_struct->requested_virtual_sensors != NULL && init_struct->n_requested_virtual_sensors != 0)
    {
        bsec_sensor_configuration_t required_sensor_settings[BSEC_MAX_PHYSICAL_SENSOR];
        uint8_t n_required_sensor_settings = BSEC_MAX_PHYSICAL_SENSOR;

        retval.bsec_status = bsec_update_subscription(
                init_struct->requested_virtual_sensors,
                init_struct->n_requested_virtual_sensors,
                required_sensor_settings, &n_required_sensor_settings);
    }
    else
    {
        retval.bsec_status = BSEC_I_SU_SUBSCRIBEDOUTPUTGATES;
    }

    return retval;
}

int64_t BSEC_ENV_Process(bsec_env_process_struct *process_struct)
{
    static bsec_env_process_state state = BSEC_ENV_PROCESS_STATE_IDLE;
    static int64_t timestamp = 0;
    static bsec_bme_settings_t sensor_settings;
    static uint32_t save_counter = 0;

    bsec_library_return_t ret;

    if (state == BSEC_ENV_PROCESS_STATE_IDLE)
    {
        timestamp = process_struct->get_timestamp_us() * 1000;

        /* Retrieve sensor settings for next measurement. */
        ret = bsec_sensor_control(timestamp, &sensor_settings);
//        ASSERT_DEBUG(ret == BSEC_OK);

        if (sensor_settings.trigger_measurement)
        {
            /* Start next measurement and return time delay until new data are
             * ready.
             */
            uint32_t meas_dur_ms = 0;
            meas_dur_ms = BSEC_ENV_TriggerMeasurement(&sensor_settings);

            state = BSEC_ENV_PROCESS_STATE_MEASURING;
            return meas_dur_ms;
        }
        else
        {
            return (sensor_settings.next_call - process_struct->get_timestamp_us() * 1000) / 1000000;
        }
    }
    else
    {
        /* Call the API to get current operation mode of the sensor */
        ret = bme680_get_sensor_mode(&bme680_g);
        ASSERT_DEBUG(ret == BME680_OK);

        /* When the measurement is completed and data is ready for reading, the sensor must be in BME680_SLEEP_MODE.
         * Read operation mode to check whether measurement is completely done and wait until the sensor is no more
         * in BME680_FORCED_MODE. */
        if (bme680_g.power_mode == BME680_FORCED_MODE)
        {
            /* Sleep for 5 more ms. */
            return 5;
        }
        else
        {
            bsec_input_t bsec_inputs[BSEC_MAX_PHYSICAL_SENSOR];
            uint8_t num_bsec_inputs = 0;

            /* Read raw sensor data from BME680 and fill it into structure that
             * can be consumed by BSEC. */
            BSEC_ENV_ReadData(timestamp, bsec_inputs, &num_bsec_inputs,
                    sensor_settings.process_data);

            /* Time to invoke BSEC to perform the actual processing */
            BSEC_ENV_ProcessData(bsec_inputs, num_bsec_inputs, process_struct);

            /* Save BSEC state to non-volatile memory every save_interval samples. */
            save_counter += 1;
            if (process_struct->save_state != NULL
                    && save_counter >= process_struct->save_interval)
            {
                uint32_t bsec_state_len = 0;
                uint8_t bsec_state[BSEC_MAX_PROPERTY_BLOB_SIZE];
                uint8_t work_buffer[BSEC_MAX_PROPERTY_BLOB_SIZE];

                ret = bsec_get_state(0, bsec_state, sizeof(bsec_state), work_buffer, sizeof(work_buffer), &bsec_state_len);
                if (ret == BSEC_OK)
                {
                    process_struct->save_state(bsec_state, bsec_state_len);
                    save_counter = 0;
                }
            }

            /* Calculate and return time remaining until next measurement. */
            state = BSEC_ENV_PROCESS_STATE_IDLE;
            return (sensor_settings.next_call
                    - process_struct->get_timestamp_us() * 1000) / 1000000;
        }
    }
}

//! \}
//! \}
//! \}

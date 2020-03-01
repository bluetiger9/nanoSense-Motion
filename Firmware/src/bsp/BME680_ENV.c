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
//! \file BME680_ENV.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup BME680_ENV_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include <RTE_HB_BME680_ENV.h>
#include "BDK.h"

#include "BME680_ENV.h"
#include "SoftwareTimer.h"
#include "bme680.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

/** \brief Contains all variables required for BME680_ENV operation. */
struct BME680_ENV_Resources
{
    /** \brief BME680 driver structure. */
    struct bme680_dev gas;

    /** \brief Timer that periodically starts measurement. */
    struct SwTimer periodic_timer;

    /** \brief Timer that reads measured data after measurement is complete. */
    struct SwTimer meas_timeout;

    /** \brief Total time it will take to complete measurement with current
     * configuration. */
    uint16_t profile_duration;

    /** \brief Application provided callback that will be called when new data
     * are available.
     */
    BME680_ENV_Callback callback;
};

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

/** \brief I2C Read wrapper function for bme680 library. */
static int8_t ENV_I2C_Read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
        uint16_t len);

/** \brief I2C Write wrapper function for bme680 library. */
static int8_t ENV_I2C_Write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
        uint16_t len);

/** \brief Timer callback for measurement start event. */
static void ENV_EventMeasure(void *arg);

/** \brief Measurement start routine that will be scheduled when measurement
 * start event occurs.
 */
static void ENV_TaskMeasure(void *arg);

/** \brief Timer callback for measurement completed event. */
static void ENV_EventRead(void *arg);

/** \brief Measurement read routine that will be scheduled when measurement
 * completed event occurs.
 */
static void ENV_TaskRead(void *arg);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static struct BME680_ENV_Resources env_res = { 0 };

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

static int8_t ENV_I2C_Read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data,
        uint16_t len)
{
    int32_t status;

    status = HAL_I2C_Write(dev_id, &reg_addr, 1, false);
    if (status != HAL_OK)
    {
        return status;
    }

    return HAL_I2C_Read(dev_id, reg_data, len, false);
}

static int8_t ENV_I2C_Write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
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

static void ENV_EventMeasure(void *arg)
{
    SwTimer_Advance(&env_res.periodic_timer);
    BDK_TaskSchedule(&ENV_TaskMeasure, NULL);
}

static void ENV_TaskMeasure(void *arg)
{
    if (env_res.gas.power_mode == BME680_FORCED_MODE)
    {
        bme680_set_sensor_mode(&env_res.gas);

        SwTimer_Initialize(&env_res.meas_timeout);
        SwTimer_AttachInt(&env_res.meas_timeout, &ENV_EventRead, NULL);
        SwTimer_ExpireInMs(&env_res.meas_timeout, env_res.profile_duration);
    }
}

static void ENV_EventRead(void *arg)
{
    SwTimer_Stop(&env_res.meas_timeout);
    BDK_TaskSchedule(&ENV_TaskRead, NULL);
}

static void ENV_TaskRead(void *arg)
{
    struct bme680_field_data field_data;

    SwTimer_Remove(&env_res.meas_timeout);

    bme680_get_sensor_data(&field_data, &env_res.gas);

    if (env_res.callback != NULL)
    {
        struct BME680_ENV_Data output_data;

#if defined RTE_BME680_ENV_TEMP_ENABLE == 1
        output_data.temperature = field_data.temperature;
#else
        output_data.temperature = INT16_MAX;
#endif /* defined RTE_BME680_ENV_TEMP_ENABLE == 1 */

#if defined RTE_BME680_ENV_HUM_ENABLE == 1
        output_data.humidity = field_data.humidity;
#else
        output_data.humidity = UINT32_MAX;
#endif /* defined RTE_BME680_ENV_HUM_ENABLE == 1 */

#if defined RTE_BME680_ENV_PRES_ENABLE == 1
        output_data.pressure = field_data.pressure;
#else
        output_data.pressure = UINT32_MAX;
#endif /* defined RTE_BME680_ENV_PRES_ENABLE == 1 */

        env_res.callback(&output_data);
    }
}

int32_t BME680_ENV_Initialize(void)
{
    int8_t status;
    uint8_t sel_setting;

    env_res.gas.dev_id = BME680_I2C_ADDR_PRIMARY;
    env_res.gas.intf = BME680_I2C_INTF;
    env_res.gas.read = &ENV_I2C_Read;
    env_res.gas.write = &ENV_I2C_Write;
    env_res.gas.delay_ms = &HAL_Delay;
    env_res.gas.amb_temp = 25;

    status = bme680_init(&env_res.gas);
    if (status != BME680_OK)
    {
        return status;
    }

    env_res.gas.tph_sett.os_hum = RTE_BME680_ENV_OS_HUM;
    env_res.gas.tph_sett.os_pres = RTE_BME680_ENV_OS_PRES;
    env_res.gas.tph_sett.os_temp = RTE_BME680_ENV_OS_TEMP;
    env_res.gas.tph_sett.filter = RTE_BME680_ENV_FILTER_SIZE;

    env_res.gas.gas_sett.run_gas = BME680_DISABLE_GAS_MEAS;
    env_res.gas.gas_sett.heatr_temp = 320;
    env_res.gas.gas_sett.heatr_dur = 150;

    env_res.gas.power_mode = BME680_SLEEP_MODE;

    sel_setting = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL
            | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

    status = bme680_set_sensor_settings(sel_setting, &env_res.gas);
    if (status != BME680_OK)
    {
        return status;
    }

    status = bme680_set_sensor_mode(&env_res.gas);
    if (status != BME680_OK)
    {
        return status;
    }

    bme680_get_profile_dur(&env_res.profile_duration, &env_res.gas);

    return 0;
}

uint32_t BME680_ENV_GetProfileDuration(void)
{
    return env_res.profile_duration;
}

int32_t BME680_ENV_StartPeriodic(BME680_ENV_Callback cb, uint32_t period_ms)
{
    /* Check if measurement period is longer than timer required to complete
     * measurement.
     */
    if (cb == NULL || period_ms < env_res.profile_duration)
    {
        return HAL_ERROR_PARAMETER;
    }

    env_res.gas.power_mode = BME680_FORCED_MODE;
    env_res.callback = cb;

    SwTimer_Initialize(&env_res.periodic_timer);
    SwTimer_AttachInt(&env_res.periodic_timer, &ENV_EventMeasure, NULL);
    SwTimer_ExpireInMs(&env_res.periodic_timer, period_ms);

    return HAL_OK;
}

int32_t BME680_ENV_StopPeriodic(void)
{
    SwTimer_Remove(&env_res.periodic_timer);
    SwTimer_Remove(&env_res.meas_timeout);

    env_res.gas.power_mode = BME680_SLEEP_MODE;
    return bme680_set_sensor_mode(&env_res.gas);
}

//! \}
//! \}
//! \}

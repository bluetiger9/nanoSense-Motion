// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BDK.h>
#include <BSEC_ENV.h>
#include <ics/CS.h>
#include <CSN_LP_ENV.h>
#include <I2CEeprom.h>
#include <api/led_api.h>
#include <stimer.h>
#include <app_trace.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

#define CSN_ENV_NODE_NAME              "EV"

#define CSN_ENV_AVAIL_BIT              ((uint32_t)0x00000040)

#define CSN_ENV_TEMP_OFFSET            (0)

#define CSN_ENV_PROP_CNT               (8)


// Shortcut macros for logging of ALS node messages.
#define CSN_ENV_Error(...) CS_LogError("ENV", __VA_ARGS__)
#define CSN_ENV_Warn(...) CS_LogWarning("ENV", __VA_ARGS__)
#define CSN_ENV_Info(...) CS_LogInfo("ENV", __VA_ARGS__)
#define CSN_ENV_Verbose(...) CS_LogVerbose("ENV", __VA_ARGS__)

/* Initializer value for BSEC init structure used to initialize the BSEC library.
 */
#define CSN_BSEC_INIT_STRUCT_VALUE { \
    .temperature_offset = CSN_BSEC_TEMPERATURE_OFFSET, \
    .sleep = CSN_BsecSleep, \
    .config_load = NULL, \
    .state_load =  NULL, /* CSN_BsecStateLoadFromEEPROM, */ \
    .requested_virtual_sensors = CSN_BSEC_REQUESTED_SENSORS, \
    .n_requested_virtual_sensors = CSN_BSEC_REQUESTED_SENSOR_COUNT, \
}

/* Initializer value for BSEC process structure that defines runtime behavior.
 *
 */
#define CSN_BSEC_PROCESS_STRUCT_VALUE { \
    .sleep = CSN_BsecSleep, \
    .get_timestamp_us = CSN_BsecGetTimestampUs, \
    .output_ready = CSN_BsecOutputReady, \
    .save_state = NULL, /* CSN_BsecStateSaveToEEPROM, */ \
    .save_interval = CSN_BSEC_SAVE_INTERVAL, \
}

#define CSN_BSEC_EEPROM_I2C_ADDR       (0x50)
#define CSN_BSEC_EEPROM_PAGE_SIZE      (4)
#define CSN_BSEC_EEPROM_ADDR           (8192-256)
#define CSN_BSEC_EEPROM_MAGIC          {'C', 'S', 'N', ' ', 'B', 'S', 'E', 'C', 0}
#define CSN_BSEC_EEPROM_MAGIC_SIZE     (9)

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static int CSN_LP_ENV_PowerModeHandler(enum CS_PowerMode mode);
static void CSN_LP_ENV_PollHandler(void);

/** \brief Handler for CS requests provided in node structure. */
static int CSN_ENV_RequestHandler(const struct CS_Request_Struct* request,
                                  char* response);

// Temperature sensor Properties
static int CSN_ENV_T_PropHandler(char* response);
static int CSN_ENV_TF_PropHandler(char* response);

// Pressure sensor Properties
static int CSN_ENV_P_PropHandler(char* response);
static int CSN_ENV_PP_PropHandler(char* response);

// Humidity sensor Properties
static int CSN_ENV_H_PropHandler(char* response);

// IAQ sensor Properties
static int CSN_ENV_A_PropHandler(char* response);

// Composite requests
static int CSN_ENV_D_PropHandler(char* response);
static int CSN_ENV_DI_PropHandler(char* response);

static void CSN_BsecSleep(uint32_t t_ms);
static int64_t CSN_BsecGetTimestampUs();
static void CSN_BsecOutputReady(bsec_env_output_struct *output);
//static uint32_t CSN_BsecStateLoadFromEEPROM(uint8_t *state_buffer,
//        uint32_t buffer_len);
//static void CSN_BsecStateSaveToEEPROM(uint8_t *state_buffer, uint32_t size);


//-----------------------------------------------------------------------------
// INTERNAL VARIABLES
//-----------------------------------------------------------------------------

static bsec_env_output_struct env_data = { 0 };

static struct stimer env_timer;

static struct stimer env_timebase_timer;

static I2CEeprom m24rf64;

static bsec_env_process_struct env_process_params = CSN_BSEC_PROCESS_STRUCT_VALUE;

/** \brief CS node structure passed to CS. */
static struct CS_Node_Struct env_node = {
        CSN_ENV_NODE_NAME,
        CSN_ENV_AVAIL_BIT,
        &CSN_ENV_RequestHandler,
        &CSN_LP_ENV_PowerModeHandler,
        &CSN_LP_ENV_PollHandler
};

struct CSN_ENV_Property_Struct
{
    const char* name;
    const char* prop_def;
    int (*callback)(char* response);
};

static struct CSN_ENV_Property_Struct env_prop[CSN_ENV_PROP_CNT] = {
        { "T",  "p/R/f/T",  &CSN_ENV_T_PropHandler},
        { "TF", "p/R/f/TF", &CSN_ENV_TF_PropHandler},
        { "P",  "p/R/f/P",  &CSN_ENV_P_PropHandler},
        { "PP", "p/R/f/PP", &CSN_ENV_PP_PropHandler},
        { "H",  "p/R/f/H",  &CSN_ENV_H_PropHandler},
        { "A",  "p/R/i/A",  &CSN_ENV_A_PropHandler},
        { "D",  "p/R/c/D",  &CSN_ENV_D_PropHandler},
        { "DI", "p/R/c/DI", &CSN_ENV_DI_PropHandler}
};


//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

/** \brief Checks if MULTISENSOR-GEVB (NOA1306) is connected to baseboard.
 *
 *  This check needs to be executed before any other CSN_ALS2 functions are
 *  called or they will fail.
 *
 *  \returns 0 If MULTISENSOR-GEVB is not present.
 *  \returns 1 If MULTISENSOR-GEVB is connected to Baseboard and is responding.
 */
struct CS_Node_Struct* CSN_LP_ENV_Create(struct stimer_ctx* ctx)
{
    struct CS_Node_Struct* retval_node = NULL;
    int32_t retval;
    bsec_env_return_val retval_bsec;
    bsec_env_init_struct init_params = CSN_BSEC_INIT_STRUCT_VALUE;

    /* Check if timer context was provided. */
    if (ctx != NULL)
    {
        /* Initialize EEPROM memory structure. */
        retval = I2CEeprom_Initialize(CSN_BSEC_EEPROM_I2C_ADDR, 4, &m24rf64);
        if (retval == I2C_EEPROM_OK)
        {
            /* Initialize BME680 sensor and BSEC library. */
            retval_bsec = BSEC_ENV_Initialize(&init_params);
            if (retval_bsec.bme680_status == BME680_OK
                    && retval_bsec.bsec_status == BSEC_OK)
            {
                /* Initialize & schedule event timer to take control over when
                 * BSEC processing should be called from main loop.
                 */
                stimer_init(&env_timer, ctx);
                stimer_expire_from_now_ns(&env_timer, 1);

                /* Always running timer that keeps track of how much time has
                 * elapsed since node initialization.
                 * This timer uses RTC due to usage of deep slep mode of RSL10.
                 * Used as a source of timestamps for BSEC library.
                 */
                stimer_init(&env_timebase_timer, ctx);
                stimer_start(&env_timebase_timer);

                retval_node = &env_node;
            }
            else
            {
                CSN_ENV_Error("BSEC initialization failed.");
            }
        }
        else
        {
            CSN_ENV_Error("EEPROM initialization failed.");
        }
    }

    return retval_node;
}

static int CSN_LP_ENV_PowerModeHandler(enum CS_PowerMode mode)
{
    // nothing to do
    // BME680 works only in one-shot mode and automatically enters sleep mode
    // when measurement is completed.

    return CS_OK;
}

static void CSN_LP_ENV_PollHandler(void)
{
    static struct stimer_duration last;
    struct stimer_duration now;

    if (env_timer.is_running && stimer_is_expired(&env_timer))
    {
        int32_t next_call_ms = 0;

        next_call_ms = BSEC_ENV_Process(&env_process_params);

        stimer_expire_from_now_ms(&env_timer, next_call_ms);

        stimer_get_elapsed_time(&env_timebase_timer, &now);
        TRACE_PRINTF("Next BSEC process in: %lu ms\r\n", next_call_ms);

        memcpy(&last, &now, sizeof(struct stimer_duration));
    }
}

void CSN_BsecSleep(uint32_t t_ms)
{
    HAL_Delay(t_ms);
}

void CSN_BsecOutputReady(bsec_env_output_struct *output)
{
    memcpy(&env_data, output, sizeof(bsec_env_output_struct));
}

int64_t CSN_BsecGetTimestampUs()
{
    struct stimer_duration elapsed;
    int64_t timestamp_us;

    stimer_get_elapsed_time(&env_timebase_timer, &elapsed);

    timestamp_us = ((int64_t) elapsed.seconds * 1000000U)
            + ((int64_t) elapsed.nanoseconds / 1000U);

    return timestamp_us;
}

//static uint32_t CSN_BsecStateLoadFromEEPROM(uint8_t *state_buffer,
//        uint32_t buffer_len)
//{
//    int32_t retval;
//    uint8_t magic[CSN_BSEC_EEPROM_MAGIC_SIZE] = CSN_BSEC_EEPROM_MAGIC;
//    uint8_t magic_read[CSN_BSEC_EEPROM_MAGIC_SIZE] = CSN_BSEC_EEPROM_MAGIC;
//
//    retval = I2CEeprom_Read(CSN_BSEC_EEPROM_ADDR, magic_read,
//            CSN_BSEC_EEPROM_MAGIC_SIZE, &m24rf64);
//    if (retval == I2C_EEPROM_OK)
//    {
//        if (memcmp(magic, magic_read, CSN_BSEC_EEPROM_MAGIC_SIZE - 1) == 0)
//        {
//            uint32_t saved_size = magic_read[CSN_BSEC_EEPROM_MAGIC_SIZE - 1];
//
//            if (saved_size != 0 && saved_size <= buffer_len)
//            {
//                retval = I2CEeprom_Read(
//                        CSN_BSEC_EEPROM_ADDR + CSN_BSEC_EEPROM_MAGIC_SIZE,
//                        state_buffer, saved_size, &m24rf64);
//                if (retval == I2C_EEPROM_OK)
//                {
//                    CSN_ENV_Verbose("Loaded BSEC state from EEPROM memory.");
//                    return saved_size;
//                }
//            }
//        }
//    }
//
//    CSN_ENV_Warn("Failed to load BSEC state from EEPROM memory.");
//    return 0;
//}
//
//static void CSN_BsecStateSaveToEEPROM(uint8_t *state_buffer, uint32_t size)
//{
//    int32_t retval;
//    uint8_t magic[CSN_BSEC_EEPROM_MAGIC_SIZE] = CSN_BSEC_EEPROM_MAGIC;
//    magic[CSN_BSEC_EEPROM_MAGIC_SIZE - 1] = size;
//
//    retval = I2CEeprom_Write(CSN_BSEC_EEPROM_ADDR, magic,
//            CSN_BSEC_EEPROM_MAGIC_SIZE, &m24rf64);
//    if (retval == I2C_EEPROM_OK)
//    {
//        retval = I2CEeprom_Write(
//                CSN_BSEC_EEPROM_ADDR + CSN_BSEC_EEPROM_MAGIC_SIZE, state_buffer,
//                size, &m24rf64);
//    }
//
//    if (retval == I2C_EEPROM_OK)
//    {
//        CSN_ENV_Verbose("Saved BSEC state to EEPROM.");
//    }
//    else
//    {
//        CSN_ENV_Error("Error while saving BSEC state to EEPROM. (errcode=%d)",
//                retval);
//    }
//}

static int CSN_ENV_RequestHandler(const struct CS_Request_Struct* request, char* response)
{
    // Check request type
    if (request->property_value != NULL)
    {
        CSN_ENV_Error("ALS properties support only read requests.");
        strcpy(response, "e/ACCESS");
        return CS_OK;
    }

    // AO Data property requests
    for (int i = 0; i < CSN_ENV_PROP_CNT; ++i)
    {
        if (strcmp(request->property, env_prop[i].name) == 0)
        {
            if (env_prop[i].callback(response) != CS_OK)
            {
                strcpy(response, "e/NODE_ERR");
            }
            return CS_OK;
        }
    }

    // PROP property request
    if (strcmp(request->property, "PROP") == 0)
    {
        sprintf(response, "i/%d", CSN_ENV_PROP_CNT);
        return CS_OK;
    }

    // NODEx property request
    if (strlen(request->property) > 4 &&
        memcmp(request->property, "PROP", 4) == 0)
    {
        // check if there are only digits after first 4 characters
        char* c = (char*)&request->property[4];
        int valid_number = 1;
        while (*c != '\0')
        {
            if (isdigit(*c) == 0)
            {
                valid_number = 0;
                break;
            }
            ++c;
        }

        if (valid_number == 1)
        {
            int prop_index = atoi(&request->property[4]);
            if (prop_index >= 0 && prop_index < CSN_ENV_PROP_CNT)
            {
                sprintf(response, "n/%s", env_prop[prop_index].prop_def);
                return CS_OK;
            }
            else
            {
                CSN_ENV_Error("Out of bound NODEx request.");
                // Invalid property error
            }
        }
        else
        {
            // Invalid property error
        }
    }

    CSN_ENV_Error("ENV property '%s' does not exist.", request->property);
    strcpy(response, "e/UNK_PROP");
    return CS_OK;
}


static int CSN_ENV_T_PropHandler(char* response)
{
    float t = env_data.temperature;
    sprintf(response, "f/%.2f", t);
    return CS_OK;
}

static int CSN_ENV_TF_PropHandler(char* response)
{
    sprintf(response, "f/%.2f", (env_data.temperature) * 1.8f + 32);
    return CS_OK;
}

static int CSN_ENV_P_PropHandler(char* response)
{
    sprintf(response, "f/%.2f", env_data.raw_pressure / 1000.0f);
    return CS_OK;
}

static int CSN_ENV_PP_PropHandler(char* response)
{
    sprintf(response, "f/%.2f", env_data.raw_pressure * 0.000145f);
    return CS_OK;
}

static int CSN_ENV_H_PropHandler(char* response)
{
    sprintf(response, "f/%.2f", env_data.humidity);
    return CS_OK;
}

static int CSN_ENV_A_PropHandler(char* response)
{
    sprintf(response, "i/%.0f", env_data.iaq);
    return CS_OK;
}

static int CSN_ENV_D_PropHandler(char* response)
{
    snprintf(response, 19, "%.0f,%.0f,%.0f,%.0f",
            (env_data.temperature * 10.0f), env_data.humidity,
            env_data.iaq, env_data.raw_pressure / 100.0f);
    return CS_OK;
}

static int CSN_ENV_DI_PropHandler(char* response)
{
    int temp = (((env_data.temperature) * 1.8f + 32.0f) * 10.0f);
    int pressure = env_data.raw_pressure * 0.000145f * 10.0f;

    snprintf(response, 19, "%d,%d,%d,%d", temp,
            (int) (env_data.humidity / 1000), 0, pressure);
    return CS_OK;
}

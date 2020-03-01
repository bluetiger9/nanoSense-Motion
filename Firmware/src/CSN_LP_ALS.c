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


#include <stdio.h>
#include <string.h>

#include <CSN_LP_ALS.h>
#include <HAL.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

#define CSN_ALS_NODE_NAME "AL"

#define CSN_ALS_AVAIL_BIT ((uint32_t)0x00000002)

// Shortcut macros for logging of ALS node messages.
#define CSN_ALS_Error(...) CS_LogError("ALS", __VA_ARGS__)
#define CSN_ALS_Warn(...) CS_LogWarning("ALS", __VA_ARGS__)
#define CSN_ALS_Info(...) CS_LogInfo("ALS", __VA_ARGS__)
#define CSN_ALS_Verbose(...) CS_LogVerbose("ALS", __VA_ARGS__)

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

/** \brief Handler for CS requests provided in node structure. */
static int CSN_ALS_RequestHandler(const struct CS_Request_Struct* request,
                                  char* response);

static int CSN_ALS_PowerModeHandler(enum CS_PowerMode mode);

static void CSN_ALS_PollHandler(void);

//-----------------------------------------------------------------------------
// INTERNAL VARIABLES
//-----------------------------------------------------------------------------

/** \brief CS node structure passed to CS. */
static struct CS_Node_Struct als_node = {
        CSN_ALS_NODE_NAME,
		CSN_ALS_AVAIL_BIT,
		&CSN_ALS_RequestHandler,
		&CSN_ALS_PowerModeHandler,
		&CSN_ALS_PollHandler
};

static struct noa1305_t noa1305 = { 0 };

/* Stores current power state of the NOA1305 sensor. */
static bool noa1305_is_awake = false;

/* Timer used to delay ambient light request responses until ALS sensor output
 * stabilizes after power up.
 */
static struct stimer noa1305_timer;

static char als_response_token = 0;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

static int32_t NOA1305_ALS_BusRead(uint8_t dev_id, uint8_t addr, uint8_t *value)
{
    if (HAL_I2C_Write(dev_id, &addr, 1, false) != HAL_OK)
    {
        return NOA1305_COMM_ERROR;
    }

    if (HAL_I2C_Read(dev_id, value, 1, false) != HAL_OK)
    {
        return NOA1305_COMM_ERROR;
    }

    return NOA1305_COMM_OK;
}

static int32_t NOA1305_ALS_BusWrite(uint8_t dev_id, uint8_t addr, uint8_t value)
{
    uint8_t data[2] = { addr, value };

    if (HAL_I2C_Write(dev_id, data, 2, false) != HAL_OK)
    {
        return NOA1305_COMM_ERROR;
    }

    return NOA1305_COMM_OK;
}

struct CS_Node_Struct* CSN_LP_ALS_Create(struct stimer_ctx *ctx)
{
    struct CS_Node_Struct *retval_node = NULL;
    int32_t retval = NOA1305_OK;

    /* Initialize and power down the NOA1305 sensor. */
    noa1305.id = NOA1305_I2C_ADDR;
    noa1305.integration_constatnt = 7700;
    noa1305.read_func = &NOA1305_ALS_BusRead;
    noa1305.write_func = &NOA1305_ALS_BusWrite;
    noa1305.delay_func = &HAL_Delay;

    retval = noa1305_init(&noa1305);
    if (retval == NOA1305_OK)
    {
        retval = noa1305_set_int_select(NOA1305_INT_INACTIVE, &noa1305);
        if (retval == NOA1305_OK)
        {
            retval = noa1305_set_integration_time(CSN_LP_ALS_INTEG_TIME_SETTING,
                    &noa1305);
            if (retval == NOA1305_OK)
            {
                retval_node = &als_node;
            }
        }
    }

    noa1305_is_awake = false;

    /* Initialize internal timer. */
    stimer_init(&noa1305_timer, ctx);

    return retval_node;
}

static int CSN_ALS_RequestHandler(const struct CS_Request_Struct* request, char* response)
{
    // Check request type
    if (request->property_value != NULL)
    {
        CSN_ALS_Error("ALS properties support only read requests.");
        sprintf(response, "e/ACCESS");
        return CS_OK;
    }

    // LUX property request
    if (strcmp(request->property, "L") == 0)
    {
        /* Wake-up the NOA1305 sensor. */
        CSN_ALS_PowerModeHandler(CS_POWER_MODE_NORMAL);

        // Set timer to read light data after required number of measurement
        // cycles elapsed.
        stimer_expire_from_now_us(&noa1305_timer,
                CSN_LP_ALS_MEASURE_CYCLES * CSN_LP_ALS_INTEG_TIME_US);

        // Save token of this request that will be used in the response
        als_response_token = request->token[0];

        // Tell ICS that it should not send any response to peer device.
        return CS_NO_RESPONSE;
    }

    // PROP property request
    if (strcmp(request->property, "PROP") == 0)
    {
        sprintf(response, "i/1");
        return CS_OK;
    }

    // PROP0 property request
    if (strcmp(request->property, "PROP0") == 0)
    {
        sprintf(response, "p/R/f/L");
        return CS_OK;
    }

    CSN_ALS_Error("ALS property '%s' does not exist.", request->property);
    sprintf(response, "e/UNK_PROP");
    return CS_OK;
}

static int CSN_ALS_PowerModeHandler(enum CS_PowerMode mode)
{
    int32_t retval = NOA1305_OK;

    switch (mode)
    {
    case CS_POWER_MODE_NORMAL:
        retval = noa1305_set_power_mode(NOA1305_POWER_ON, &noa1305);
        noa1305_is_awake = true;
        CSN_ALS_Verbose("NOA1305 powered on.");
        break;

    case CS_POWER_MODE_SLEEP:
        retval = noa1305_set_power_mode(NOA1305_POWER_DOWN, &noa1305);
        noa1305_is_awake = false;
        stimer_stop(&noa1305_timer);
        CSN_ALS_Verbose("NOA1305 powered off.");
        break;
    }

    return (retval == NOA1305_OK) ? CS_OK : CS_ERROR;
}

static void CSN_ALS_PollHandler(void)
{
    int32_t retval;
    uint32_t lux;
    char response[21];

    /* Check if ongoing measurement result is ready. */
    if (noa1305_timer.is_running && stimer_is_expired(&noa1305_timer))
    {
        // Retrieve sensor data and convert the result into Lux
        retval = noa1305_convert_als_data_lux(&lux, &noa1305);
        if (retval == NOA1305_OK)
        {
            // Compose response packet and send notification to peer device.
            snprintf(response, 21, "%c/f/%lu.00", als_response_token, lux);

            CS_InjectResponse(response);
        }
        else
        {
            CSN_ALS_Error("Failed to read ALS data.");
        }

        /* Put the sensor to power down mode until next request is received. */
        CSN_ALS_PowerModeHandler(CS_POWER_MODE_SLEEP);
    }
}

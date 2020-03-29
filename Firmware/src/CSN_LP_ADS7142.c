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
#include <ics/CS.h>
#include <ads7142.h>
#include <CSN_LP_ADS7142.h>
#include <I2CEeprom.h>
#include <api/led_api.h>
#include <stimer.h>
#include <app_trace.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

//#define CSN_ADS7142_NODE_NAME              "AD"
#define CSN_ADS7142_NODE_NAME              "EV"
//#define CSN_ADS7142_AVAIL_BIT              ((uint32_t)0x00000080)
#define CSN_ADS7142_AVAIL_BIT              ((uint32_t)0x00000040)

#define CSN_ADS7142_PROP_CNT               (2)

// Shortcut macros for logging of ALS node messages.
#define CSN_ADS7142_Error(...) CS_LogError("ADS", __VA_ARGS__)
#define CSN_ADS7142_Warn(...) CS_LogWarning("ADS", __VA_ARGS__)
#define CSN_ADS7142_Info(...) CS_LogInfo("ADS", __VA_ARGS__)
#define CSN_ADS7142_Verbose(...) CS_LogVerbose("ADS", __VA_ARGS__)

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static int CSN_LP_ADS7142_PowerModeHandler(enum CS_PowerMode mode);
static void CSN_LP_ADS7142_PollHandler(void);

/** \brief Handler for CS requests provided in node structure. */
static int CSN_ADS7142_RequestHandler(const struct CS_Request_Struct* request,
                                  char* response);

// CH 0 & 1
static int CSN_ADS7142_X_PropHandler(char* response);
static int CSN_ADS7142_Y_PropHandler(char* response);

//-----------------------------------------------------------------------------
// INTERNAL VARIABLES
//-----------------------------------------------------------------------------

static struct stimer env_timer;

static struct stimer env_timebase_timer;

static int32_t channels[2] = { -1 };

static bool data_requested = false;

/** \brief CS node structure passed to CS. */
static struct CS_Node_Struct env_node = {
        CSN_ADS7142_NODE_NAME,
        CSN_ADS7142_AVAIL_BIT,
        &CSN_ADS7142_RequestHandler,
        &CSN_LP_ADS7142_PowerModeHandler,
        &CSN_LP_ADS7142_PollHandler
};

struct CSN_ADS7142_Property_Struct
{
    const char* name;
    const char* prop_def;
    int (*callback)(char* response);
};

static struct CSN_ADS7142_Property_Struct env_prop[CSN_ADS7142_PROP_CNT] = {
        { "T",  "p/R/f/T",  &CSN_ADS7142_X_PropHandler},
        { "TF", "p/R/f/TF", &CSN_ADS7142_Y_PropHandler}
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
struct CS_Node_Struct* CSN_LP_ADS7142_Create(struct stimer_ctx* ctx)
{
    struct CS_Node_Struct* retval_node = NULL;

	/* Check if timer context was provided. */
	if (ctx != NULL) {

		/* Initialize ADS7142 sensor */
		ret_t ads7142_ret = ads7142_init();
		if (ads7142_ret == ADS7142_OK) {
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

		} else {
			CSN_ADS7142_Error("BSEC initialization failed.");
		}
	}

    return retval_node;
}

static int CSN_LP_ADS7142_PowerModeHandler(enum CS_PowerMode mode)
{
    ledNotif2(3, 150);
    if (mode == CS_POWER_MODE_SLEEP) {
    	stimer_stop(&env_timer);
    	data_requested = false;

    	// sleep mode:
        uint32_t channels[2];
		ads7142_read(channels);

        uint32_t channels_low[2];
        uint32_t channels_high[2];

        channels_low[0] = channels[0] * 0.95;
        channels_high[0] = channels[0] * 1.05;

        channels_low[1] = channels[1] * 0.95;
        channels_high[1] = channels[1] * 1.05;

        ads7142_autonomous_mode_configure();

        ads7142_set_alert_thresholds(0, channels_low[0], channels_high[0]);
        ads7142_set_alert_thresholds(1, channels_low[1], channels_high[1]);

        ads7142_enable_alerts(true, true);

        ads7142_autonomous_mode_start();
    }
    return CS_OK;
}

static void CSN_LP_ADS7142_PollHandler(void)
{
    static struct stimer_duration last;
    struct stimer_duration now;

    if (env_timer.is_running && stimer_is_expired(&env_timer))
    {
    	if (!data_requested) {
    		CSN_LP_ADS7142_PowerModeHandler(CS_POWER_MODE_SLEEP);

    	} else {
			int32_t next_call_ms = 500;
			ledNotif2(4, 50);

			ads7142_read(channels);
			//next_call_ms = BSEC_ADS7142_Process(&env_process_params);

			stimer_expire_from_now_ms(&env_timer, next_call_ms);

			stimer_get_elapsed_time(&env_timebase_timer, &now);
			TRACE_PRINTF("Next BSEC process in: %lu ms\r\n", next_call_ms);

			memcpy(&last, &now, sizeof(struct stimer_duration));
    	}
    }
}

static int CSN_ADS7142_RequestHandler(const struct CS_Request_Struct* request, char* response)
{
    // Check request type
    if (request->property_value != NULL)
    {
        CSN_ADS7142_Error("ALS properties support only read requests.");
        strcpy(response, "e/ACCESS");
        return CS_OK;
    }

    // AO Data property requests
    data_requested = true;
    for (int i = 0; i < CSN_ADS7142_PROP_CNT; ++i)
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
        sprintf(response, "i/%d", CSN_ADS7142_PROP_CNT);
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
            if (prop_index >= 0 && prop_index < CSN_ADS7142_PROP_CNT)
            {
                sprintf(response, "n/%s", env_prop[prop_index].prop_def);
                return CS_OK;
            }
            else
            {
                CSN_ADS7142_Error("Out of bound NODEx request.");
                // Invalid property error
            }
        }
        else
        {
            // Invalid property error
        }
    }

    CSN_ADS7142_Error("ENV property '%s' does not exist.", request->property);
    strcpy(response, "e/UNK_PROP");
    return CS_OK;
}


static int CSN_ADS7142_X_PropHandler(char* response)
{
    sprintf(response, "f/%.2f", (float) channels[0]);
    return CS_OK;
}

static int CSN_ADS7142_Y_PropHandler(char* response)
{
    sprintf(response, "f/%.2f", (float) channels[1]);
    return CS_OK;
}

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

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BDK.h>
#include <CSN_LP_AO.h>
#include <BHI160_NDOF.h>


//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

#define CSN_AO_NODE_NAME               "AO"

#define CSN_AO_AVAIL_BIT               0x00000010

#define CSN_AO_PROP_CNT                21

// Shortcut macros for logging of AO node messages.
#define CSN_AO_Error(...) CS_LogError("AO", __VA_ARGS__)
#define CSN_AO_Warn(...) CS_LogWarning("AO", __VA_ARGS__)
#define CSN_AO_Info(...) CS_LogInfo("AO", __VA_ARGS__)
#define CSN_AO_Verbose(...) CS_LogVerbose("AO", __VA_ARGS__)

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static void CSN_AO_SensorCallback(bhy_data_generic_t *data,
        bhy_virtual_sensor_t sensor);

/** \brief Handler for CS requests provided in node structure. */
static int CSN_LP_AO_RequestHandler(const struct CS_Request_Struct* request,
                                  char* response);

static int CSN_LP_AO_PowerModeHandler(enum CS_PowerMode mode);
static void CSN_LP_AO_PollHandler(void);
static void CSN_LP_AO_EnableVirtualSensor(enum BHI160_NDOF_Sensor sensor);

// Sensor Calibration Status
static int CSN_AO_C_PropHandler(char* response);

// Absolute Orientation Properties
static int CSN_AO_H_PropHandler(char* response);
static int CSN_AO_P_PropHandler(char* response);
static int CSN_AO_R_PropHandler(char* response);

// Accelerometer Gravity Vector Properties
static int CSN_AO_GX_PropHandler(char* response);
static int CSN_AO_GY_PropHandler(char* response);
static int CSN_AO_GZ_PropHandler(char* response);

// Accelerometer Linear Acceleration Vector Properties
static int CSN_AO_AX_PropHandler(char* response);
static int CSN_AO_AY_PropHandler(char* response);
static int CSN_AO_AZ_PropHandler(char* response);

// Magnetometer Magnetic Flux
static int CSN_AO_MX_PropHandler(char* response);
static int CSN_AO_MY_PropHandler(char* response);
static int CSN_AO_MZ_PropHandler(char* response);

// Gyroscope Angular Rotation
static int CSN_AO_ARX_PropHandler(char* response);
static int CSN_AO_ARY_PropHandler(char* response);
static int CSN_AO_ARZ_PropHandler(char* response);

// Composite Properties
static int CSN_AO_O_PropHandler(char* response);
static int CSN_AO_G_PropHandler(char* response);
static int CSN_AO_A_PropHandler(char* response);
static int CSN_AO_M_PropHandler(char* response);
static int CSN_AO_AR_PropHandler(char* response);

//-----------------------------------------------------------------------------
// INTERNAL VARIABLES
//-----------------------------------------------------------------------------

/** \brief CS node structure passed to CS. */
static struct CS_Node_Struct ao_node = {
		CSN_AO_NODE_NAME,
		CSN_AO_AVAIL_BIT,
		&CSN_LP_AO_RequestHandler,
		&CSN_LP_AO_PowerModeHandler,
		&CSN_LP_AO_PollHandler
};

struct CSN_AO_Property_Struct
{
	const char* name;
	const char* prop_def;
	int (*callback)(char* response);
	enum BHI160_NDOF_Sensor required_sensor;
};

static const struct CSN_AO_Property_Struct ao_prop[CSN_AO_PROP_CNT] = {
    { "C",   "p/R/h/CAL",  &CSN_AO_C_PropHandler,                                  0 },
    { "O",     "p/R/c/O",   &CSN_AO_O_PropHandler,         BHI160_NDOF_S_ORIENTATION },
    { "G",     "p/R/c/G",   &CSN_AO_G_PropHandler,             BHI160_NDOF_S_GRAVITY },
    { "A",     "p/R/c/A",   &CSN_AO_A_PropHandler, BHI160_NDOF_S_LINEAR_ACCELERATION },
    { "M",     "p/R/c/M",   &CSN_AO_M_PropHandler,      BHI160_NDOF_S_MAGNETIC_FIELD },
    { "AR",   "p/R/c/AR",  &CSN_AO_AR_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION },
    { "H",     "p/R/f/H",   &CSN_AO_H_PropHandler,         BHI160_NDOF_S_ORIENTATION },
    { "P",     "p/R/f/P",   &CSN_AO_P_PropHandler,         BHI160_NDOF_S_ORIENTATION },
    { "R",     "p/R/f/R",   &CSN_AO_R_PropHandler,         BHI160_NDOF_S_ORIENTATION },
    { "GX",   "p/R/f/GX",  &CSN_AO_GX_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION },
    { "GY",   "p/R/f/GY",  &CSN_AO_GY_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION },
    { "GZ",   "p/R/f/GZ",  &CSN_AO_GZ_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION },
    { "AX",   "p/R/f/AX",  &CSN_AO_AX_PropHandler, BHI160_NDOF_S_LINEAR_ACCELERATION },
    { "AY",   "p/R/f/AY",  &CSN_AO_AY_PropHandler, BHI160_NDOF_S_LINEAR_ACCELERATION },
    { "AZ",   "p/R/f/AZ",  &CSN_AO_AZ_PropHandler, BHI160_NDOF_S_LINEAR_ACCELERATION },
    { "MX",   "p/R/f/MX",  &CSN_AO_MX_PropHandler,      BHI160_NDOF_S_MAGNETIC_FIELD },
    { "MY",   "p/R/f/MY",  &CSN_AO_MY_PropHandler,      BHI160_NDOF_S_MAGNETIC_FIELD },
    { "MZ",   "p/R/f/MZ",  &CSN_AO_MZ_PropHandler,      BHI160_NDOF_S_MAGNETIC_FIELD },
    { "ARX", "p/R/f/ARX", &CSN_AO_ARX_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION },
    { "ARY", "p/R/f/ARY", &CSN_AO_ARY_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION },
    { "ARZ", "p/R/f/ARZ", &CSN_AO_ARZ_PropHandler,    BHI160_NDOF_S_RATE_OF_ROTATION }
};

static bhy_data_vector_t orientation = { 0 };
static bhy_data_vector_t gravity = { 0 };
static bhy_data_vector_t lin_accel = { 0 };
static bhy_data_vector_t rate_of_rotation = { 0 };
static bhy_data_vector_t magnetic_field = { 0 };

static uint32_t ao_enabled_sensors = 0;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

struct CS_Node_Struct* CSN_LP_AO_Create(void)
{
    int32_t errcode = 0;

    errcode = BHI160_NDOF_Initialize();
    if (errcode == BHY_SUCCESS)
    {
        errcode = bhy_install_sensor_callback(BHI160_NDOF_S_GRAVITY,
                VS_WAKEUP, CSN_AO_SensorCallback);
        errcode += bhy_install_sensor_callback(BHI160_NDOF_S_LINEAR_ACCELERATION,
                VS_WAKEUP, CSN_AO_SensorCallback);
        errcode += bhy_install_sensor_callback(BHI160_NDOF_S_MAGNETIC_FIELD,
                VS_WAKEUP, CSN_AO_SensorCallback);
        errcode += bhy_install_sensor_callback(BHI160_NDOF_S_ORIENTATION,
                VS_WAKEUP, CSN_AO_SensorCallback);
        errcode += bhy_install_sensor_callback(BHI160_NDOF_S_RATE_OF_ROTATION,
                VS_WAKEUP, CSN_AO_SensorCallback);

        if (errcode == BHY_SUCCESS)
        {
            return &ao_node;
        }
        else
        {
            CSN_AO_Error("Failed to install sensor callbacks.");
        }
    }
    else
    {
        CSN_AO_Error("Failed to initialize BHI160. (err=%d)", errcode);
    }

    // Error occurred.
    return NULL;
}

static void CSN_AO_SensorCallback(bhy_data_generic_t *data,
        bhy_virtual_sensor_t sensor)
{
    switch ((int) sensor)
    {
    case VS_ID_GRAVITY:
    case VS_ID_GRAVITY_WAKEUP:
        memcpy(&gravity, &data->data_vector, sizeof(bhy_data_vector_t));
        break;
    case VS_ID_LINEAR_ACCELERATION:
    case VS_ID_LINEAR_ACCELERATION_WAKEUP:
        memcpy(&lin_accel, &data->data_vector, sizeof(bhy_data_vector_t));
        break;
    case VS_ID_MAGNETOMETER:
    case VS_ID_MAGNETOMETER_WAKEUP:
        memcpy(&magnetic_field, &data->data_vector, sizeof(bhy_data_vector_t));
        break;
    case VS_ID_ORIENTATION:
    case VS_ID_ORIENTATION_WAKEUP:
        memcpy(&orientation, &data->data_vector, sizeof(bhy_data_vector_t));
        break;
    case VS_ID_GYROSCOPE:
    case VS_ID_GYROSCOPE_WAKEUP:
        memcpy(&rate_of_rotation, &data->data_vector, sizeof(bhy_data_vector_t));
        break;
    default:
        CSN_AO_Warn("Unknown virtual sensor type: %d", sensor);
        break;
    }
}

static int CSN_LP_AO_RequestHandler(const struct CS_Request_Struct* request,
        char* response)
{
    // Check request type
    if (request->property_value != NULL)
    {
        CSN_AO_Error("AO properties support only read requests.");
        sprintf(response, "e/ACCESS");
        return CS_OK;
    }

    // AO Data property requests
    for (int i = 0; i < CSN_AO_PROP_CNT; ++i)
    {
        if (strcmp(request->property, ao_prop[i].name) == 0)
        {
            // Wake up the sensor chip
            CSN_LP_AO_PowerModeHandler(CS_POWER_MODE_NORMAL);

            // Enable respective virtual sensor
            CSN_LP_AO_EnableVirtualSensor(ao_prop[i].required_sensor);

            // Fill response with latest data
            if (ao_prop[i].callback(response) != CS_OK)
            {
                sprintf(response, "e/NODE_ERR");
            }
            return CS_OK;
        }
    }

    // PROP property request
    if (strcmp(request->property, "PROP") == 0)
    {
        sprintf(response, "i/%d", CSN_AO_PROP_CNT);
        return CS_OK;
    }

    // NODEx property request
    if (strlen(request->property) > 4
            && memcmp(request->property, "PROP", 4) == 0)
    {
        // check if there are only digits after first 4 characters
        char* c = (char*) &request->property[4];
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
            if (prop_index >= 0 && prop_index < CSN_AO_PROP_CNT)
            {
                sprintf(response, "n/%s", ao_prop[prop_index].prop_def);
                return CS_OK;
            }
            else
            {
                CSN_AO_Error("Out of bound NODEx request.");
                // Invalid property error
            }
        }
        else
        {
            // Invalid property error
        }
    }

    CSN_AO_Error("AO property '%s' does not exist.", request->property);
    sprintf(response, "e/UNK_PROP");
    return CS_OK;
}

static int CSN_LP_AO_PowerModeHandler(enum CS_PowerMode mode)
{
    static bool is_suspended = true;
    int32_t errcode = BHY_SUCCESS;

    switch (mode)
    {
    case CS_POWER_MODE_NORMAL:
        if (is_suspended)
        {
            errcode = BHI160_NDOF_SetPowerMode(BHI160_NDOF_PM_NORMAL);
            if (errcode != BHY_SUCCESS)
            {
                return CS_ERROR;
            }

            is_suspended = false;
            CSN_AO_Info("Entered NORMAL power mode.");
        }
        break;

    case CS_POWER_MODE_SLEEP:
        // Disable all virtual sensors
        errcode = bhy_disable_virtual_sensor(BHI160_NDOF_S_GRAVITY, VS_WAKEUP);
        errcode += bhy_disable_virtual_sensor(BHI160_NDOF_S_LINEAR_ACCELERATION, VS_WAKEUP);
        errcode += bhy_disable_virtual_sensor(BHI160_NDOF_S_MAGNETIC_FIELD, VS_WAKEUP);
        errcode += bhy_disable_virtual_sensor(BHI160_NDOF_S_ORIENTATION, VS_WAKEUP);
        errcode += bhy_disable_virtual_sensor(BHI160_NDOF_S_RATE_OF_ROTATION, VS_WAKEUP);

        ao_enabled_sensors = 0;
        if (errcode != BHY_SUCCESS)
        {
            return CS_ERROR;
        }

        // Put chip into standby mode
        is_suspended = true;
        errcode = BHI160_NDOF_SetPowerMode(BHI160_NDOF_PM_STANDBY);
        if (errcode != BHY_SUCCESS)
        {
            return CS_ERROR;
        }

        // Reset all sensor data.
        memset(&orientation, 0, sizeof(orientation));
        memset(&gravity, 0, sizeof(gravity));
        memset(&lin_accel, 0, sizeof(lin_accel));
        memset(&rate_of_rotation, 0, sizeof(rate_of_rotation));
        memset(&magnetic_field, 0, sizeof(magnetic_field));

        CSN_AO_Info("Entered STANDBY power mode.");
        break;
    }

    return CS_OK;
}

static void CSN_LP_AO_PollHandler(void)
{

}

static void CSN_LP_AO_EnableVirtualSensor(enum BHI160_NDOF_Sensor sensor)
{
    int32_t errcode;

    if ((ao_enabled_sensors & (1 << sensor)) == 0)
    {
        errcode = bhy_enable_virtual_sensor(sensor, VS_WAKEUP,
                CSN_LP_AO_SAMPLE_RATE, 0, VS_FLUSH_NONE, 0, 0);
        if (errcode != BHY_SUCCESS)
        {
            CSN_AO_Error("Failed to enable virtual sensor %d (err=%d)", sensor,
                    errcode);
        }
        else
        {
            CSN_AO_Verbose("Enabled virtual sensor: %d", sensor);
        }

        // sensor IDs range from 1 up to 31
        ao_enabled_sensors |= (1 << sensor);
    }
}

static int CSN_AO_C_PropHandler(char* response)
{
    sprintf(response, "h/%X%X%X%X", 0, gravity.status, rate_of_rotation.status,
            magnetic_field.status);
    return CS_OK;
}

static int CSN_AO_O_PropHandler(char* response)
{
    snprintf(response, 19, "%d,%d,%d",
            (int) (orientation.x / 32768.0f * 360.0f * 10.0f),
            (int) (orientation.y / 32768.0f * 360.0f * 10.0f),
            (int) (orientation.z / 32768.0f * 360.0f * 10.0f));

    return CS_OK;
}

static int CSN_AO_G_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "%d,%d,%d",
            (int) (gravity.x / 32768.0f * dyn_range * 9.80665f * 100.0f),
            (int) (gravity.y / 32768.0f * dyn_range * 9.80665f * 100.0f),
            (int) (gravity.z / 32768.0f * dyn_range * 9.80665f * 100.0f));

    return CS_OK;
}

static int CSN_AO_A_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "%d,%d,%d",
            (int) (lin_accel.x / 32768.0f * dyn_range * 9.80665f * 100.0f),
            (int) (lin_accel.y / 32768.0f * dyn_range * 9.80665f * 100.0f),
            (int) (lin_accel.z / 32768.0f * dyn_range * 9.80665f * 100.0f));

    return CS_OK;
}

static int CSN_AO_M_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetMagDynamicRange();

    snprintf(response, 19, "%d,%d,%d",
            (int) (magnetic_field.x / 32768.0f * dyn_range * 10.0f),
            (int) (magnetic_field.y / 32768.0f * dyn_range * 10.0f),
            (int) (magnetic_field.z / 32768.0f * dyn_range * 10.0f));

    return CS_OK;
}

static int CSN_AO_AR_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetGyroDynamicRange();

    snprintf(response, 19, "%d,%d,%d",
            (int) (rate_of_rotation.x / 32768.0f * dyn_range * 10.0f),
            (int) (rate_of_rotation.y / 32768.0f * dyn_range * 10.0f),
            (int) (rate_of_rotation.z / 32768.0f * dyn_range * 10.0f));

    return CS_OK;
}

static int CSN_AO_H_PropHandler(char* response)
{
    snprintf(response, 19, "f/%f", orientation.x / 32768.0f * 360.0f);

    return CS_OK;
}

static int CSN_AO_P_PropHandler(char* response)
{
    snprintf(response, 19, "f/%f", orientation.y / 32768.0f * 360.0f);

    return CS_OK;
}

static int CSN_AO_R_PropHandler(char* response)
{
    snprintf(response, 19, "f/%f", orientation.z / 32768.0f * 360.0f);

    return CS_OK;
}

static int CSN_AO_GX_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "f/%f", gravity.x / 32768.0f * 9.80665f * dyn_range);

    return CS_OK;
}

static int CSN_AO_GY_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "f/%f", gravity.y / 32768.0f * 9.80665f * dyn_range);

    return CS_OK;
}

static int CSN_AO_GZ_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "f/%f", gravity.z / 32768.0f * 9.80665f * dyn_range);

    return CS_OK;
}

static int CSN_AO_AX_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "f/%f", lin_accel.x / 32768.0f * 9.80665f * dyn_range);

    return CS_OK;
}

static int CSN_AO_AY_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "f/%f", lin_accel.y / 32768.0f * 9.80665f * dyn_range);

    return CS_OK;
}

static int CSN_AO_AZ_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetAccelDynamicRange();

    snprintf(response, 19, "f/%f", lin_accel.z / 32768.0f * 9.80665f * dyn_range);

    return CS_OK;
}

static int CSN_AO_MX_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetMagDynamicRange();

    snprintf(response, 19, "f/%f", magnetic_field.x / 32768.0f * dyn_range);

    return CS_OK;
}

static int CSN_AO_MY_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetMagDynamicRange();

    snprintf(response, 19, "f/%f", magnetic_field.y / 32768.0f * dyn_range);

    return CS_OK;
}

static int CSN_AO_MZ_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetMagDynamicRange();

    snprintf(response, 19, "f/%f", magnetic_field.z / 32768.0f * dyn_range);

    return CS_OK;
}

static int CSN_AO_ARX_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetGyroDynamicRange();

    snprintf(response, 19, "f/%f", rate_of_rotation.x / 32768.0f * dyn_range);

    return CS_OK;
}

static int CSN_AO_ARY_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetGyroDynamicRange();

    snprintf(response, 19, "f/%f", rate_of_rotation.y / 32768.0f * dyn_range);

    return CS_OK;
}

static int CSN_AO_ARZ_PropHandler(char* response)
{
    const uint16_t dyn_range = BHI160_NDOF_GetGyroDynamicRange();

    snprintf(response, 19, "f/%f", rate_of_rotation.z / 32768.0f * dyn_range);

    return CS_OK;
}

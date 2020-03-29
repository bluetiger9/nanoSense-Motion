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

#ifndef ICS_NODE_LP_ADS7142_H_
#define ICS_NODE_LP_ADS7142_H_

#include <ics/CS.h>
#include <stdbool.h>

#include <stimer.h>

#include <RTE_app_config.h>

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

/* \define CSN_BSEC_SAMPLE_RATE
 *
 * \brief
 * Defines the sample rate of BSEC sensor outputs.
 *
 * The standard BSEC sample rates are:
 *
 * * BSEC_SAMPLE_RATE_LP - Low power mode with sample period of 3 seconds.
 * * BSEC_SAMPLE_RATE_ULP - Ultra low power mode with sample period of 5 minutes.
 */

#if RTE_APP_ICS_EV_SAMPLE_RATE == 0

// 3 seconds sample rate
#define CSN_BSEC_SAMPLE_RATE            BSEC_SAMPLE_RATE_LP

#else

// 5 minutes sample rate
#define CSN_BSEC_SAMPLE_RATE            BSEC_SAMPLE_RATE_ULP

#endif /* RTE_APP_ICS_EV_SAMPLE_RATE == 0 */

/* \define CSN_BSEC_REQUESTED_SENSOR_COUNT
 *
 * \brief
 * Total number of desired virtual outputs.
 *
 * This value should match with the number of declared sensors in the
 * CSN_BSEC_REQUESTED_SENSORS below.
 */

/* \define CSN_BSEC_REQUESTED_SENSORS
 *
 * \brief
 * Initialization value for BSEC requested sensor outputs.
 *
 * It consists of desired virtual sensor outputs and their sample rates.
 */

#if RTE_APP_ICS_EV_IAQ_ENABLED == 0

#define CSN_BSEC_REQUESTED_SENSOR_COUNT (3)

#define CSN_BSEC_REQUESTED_SENSORS { \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE }, \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY }, \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_RAW_PRESSURE }, \
}

#else

#define CSN_BSEC_REQUESTED_SENSOR_COUNT (4)

#define CSN_BSEC_REQUESTED_SENSORS { \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_IAQ }, \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE }, \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY }, \
    { CSN_BSEC_SAMPLE_RATE, BSEC_OUTPUT_RAW_PRESSURE }, \
}

#endif /* RTE_APP_ICS_EV_IAQ_ENABLED == 0 */


/* Static temperature offset that will be subtracted from all temperature
 * measurements. It compensates for other heat sources located near BME680.
 */
#define CSN_BSEC_TEMPERATURE_OFFSET     (0.0f)

/* Save BSEC state every after every 100 measurements.
 * Save interval 100 -> BSEC_SAMPLE_RATE_LP  -> save every 5 minutes.
 * Save interval 288 -> BSEC_SAMPLE_RATE_ULP -> save very 24 hours.
 */
#define CSN_BSEC_SAVE_INTERVAL          (100)


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

extern struct CS_Node_Struct* CSN_LP_ADS7142_Create(struct stimer_ctx* ctx);


#endif /* ICS_NODE_LP_ADS7142_H_ */

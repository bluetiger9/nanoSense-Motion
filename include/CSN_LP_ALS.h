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

#ifndef ICS_NODE_LP_ALS_H_
#define ICS_NODE_LP_ALS_H_

#include <ics/CS.h>
#include <stdbool.h>

#include <noa1305.h>
#include <stimer.h>

#include <RTE_app_config.h>

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

/** Number of measurement cycles to delay before reading ambient light value
 * from sensor.
 *
 * NOA1305 requires at least 3 cycles to stabilize output after power on.
 */
#define CSN_LP_ALS_MEASURE_CYCLES      RTE_APP_ICS_AL_CYCLES

/* Integration time setting to be used by this node.
 *
 * Longer integration times will increase measurement precision in low light
 * environments but will use more power.
 */
#define CSN_LP_ALS_INTEG_TIME_SETTING  RTE_APP_ICS_AL_INTEG_TIME

#if RTE_APP_ICS_AL_INTEG_TIME == 0
#define CSN_LP_ALS_INTEG_TIME_US       (800000)
#elif RTE_APP_ICS_AL_INTEG_TIME == 1
#define CSN_LP_ALS_INTEG_TIME_US       (400000)
#elif RTE_APP_ICS_AL_INTEG_TIME == 2
#define CSN_LP_ALS_INTEG_TIME_US       (200000)
#elif RTE_APP_ICS_AL_INTEG_TIME == 3
#define CSN_LP_ALS_INTEG_TIME_US       (100000)
#elif RTE_APP_ICS_AL_INTEG_TIME == 4
#define CSN_LP_ALS_INTEG_TIME_US       (50000)
#elif RTE_APP_ICS_AL_INTEG_TIME == 5
#define CSN_LP_ALS_INTEG_TIME_US       (25000)
#elif RTE_APP_ICS_AL_INTEG_TIME == 6
#define CSN_LP_ALS_INTEG_TIME_US       (12500)
#elif RTE_APP_ICS_AL_INTEG_TIME == 7
#define CSN_LP_ALS_INTEG_TIME_US       (6250)
#else
#error Invalid integration time setting value
#endif


//-----------------------------------------------------------------------------
// EXPORTED FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------

/**
 *
 * \param ctx
 * Timer context required for internal timers.
 */
extern struct CS_Node_Struct* CSN_LP_ALS_Create(struct stimer_ctx *ctx);


#endif /* ICS_NODE_LP_ALS_H_ */

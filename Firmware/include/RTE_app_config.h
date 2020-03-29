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
//!
//!
//-----------------------------------------------------------------------------

#ifndef RTE_APP_CONFIG_H_
#define RTE_APP_CONFIG_H_

// <<< Use Configuration Wizard in Context Menu >>>


// <o> BLE Advertising Interval [ms] <10-1000000>
// <i> Determines how often to send advertising packets.
// <i> Default: 1000 ms
#ifndef RTE_APP_BLE_ADV_INT
#define RTE_APP_BLE_ADV_INT  1000
#endif

// <s.20> BLE Complete Local Name
// <i> Advertised name of this device.
// <i> Default: HB_BLE_Terminal
#ifndef RTE_APP_BLE_COMPLETE_LOCAL_NAME
#define RTE_APP_BLE_COMPLETE_LOCAL_NAME  "HB_BLE_Terminal"
#endif

// <o> Advertising Stop Timeout [s] <1-1000>
// <i> Default: 60 s
#ifndef RTE_APP_ADV_DISABLE_TIMEOUT
#define RTE_APP_ADV_DISABLE_TIMEOUT  10
#endif

// <o> Wake-up Button Check Interval [ms] <10-1000000>
// <i> Default: 1000 ms
#ifndef RTE_APP_BTN_CHECK_TIMEOUT
#define RTE_APP_BTN_CHECK_TIMEOUT  1500
#endif

// <o> I2C Bus Speed
// <i> Default: Fast+
// <0=> Standard
// <1=> Fast
// <2=> Fast+
#ifndef RTE_APP_I2C_BUS_SPEED
#define RTE_APP_I2C_BUS_SPEED  0
//#define RTE_APP_I2C_BUS_SPEED  2
#endif

// <h> IDK Custom Service

// <e> Ambient Light Node (AL)
// <i> Provide ambienlt light data over IDK Custom Service.
// <i> Default: Enabled
#ifndef RTE_APP_ICS_AL_ENABLED
#define RTE_APP_ICS_AL_ENABLED  1
#endif

// <o> Integration Time [ms]
// <i> Length of one measurement cycle.
// <i> Default: 12.5 ms
// <0=> 800
// <1=> 400
// <2=> 200
// <3=> 100
// <4=> 50
// <5=> 25
// <6=> 12.5
// <7=> 6.25
#ifndef RTE_APP_ICS_AL_INTEG_TIME
#define RTE_APP_ICS_AL_INTEG_TIME  6
#endif

// <o> Number of Measurement Cycles [cycles] <1-10>
// <i> NOA1305 recommends to wait 3 measurement cycles to stabilize sensor output.
// <i> Default: 4
#ifndef RTE_APP_ICS_AL_CYCLES
#define RTE_APP_ICS_AL_CYCLES  4
#endif

// </e>


// <e> Environmental Node (EV)
// <i> Provide environmental data over IDK Custom Service
// <i> Outputs: Temperature, rel. humidity, atmospheric pressure, IAQ
// <i> Default: Enabled
#ifndef RTE_APP_ICS_EV_ENABLED
#define RTE_APP_ICS_EV_ENABLED  1
#endif

// <o> BSEC Sample Rate
// <i> BSEC library offers two sample rates.
// <i> 3 second sample rate for Low Power Mode and 5 minute sample rate for Ultra Low Power Mode.
// <i> Default: 5 min
// <0=> 3 sec
// <1=> 5 min
#ifndef RTE_APP_ICS_EV_SAMPLE_RATE
#define RTE_APP_ICS_EV_SAMPLE_RATE  1
#endif

// <q> Indoor Air Quality measurement
// <i> Whether IAQ should be measured using the gas sensor of BME680.
// <i> Default: Disabled
#ifndef RTE_APP_ICS_EV_IAQ_ENABLED
#define RTE_APP_ICS_EV_IAQ_ENABLED  0
#endif

// </e>


// <e> Absolute Orientation Node (AO)
// <i> Provide orientation data over IDK Custom Service.
// <i> Outputs: Linear acceleration, Gravity vector, Absolute orientation, Rate of Rotation, Magnetic Field Intensity
// <i> Default: Enabled
#ifndef RTE_APP_ICS_AO_ENABLED
#define RTE_APP_ICS_AO_ENABLED  1
#endif

// <o> Virtual Sensor Report Rate [Hz]
#ifndef RTE_APP_ICS_AO_REPORT_RATE
#define RTE_APP_ICS_AO_REPORT_RATE  5
#endif

// </e>

// </h>

// <<< end of configuration section >>>

#endif /* RTE_APP_CONFIG_H_ */


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

#ifndef APP_H_
#define APP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <rsl10.h>

#include <HAL.h>
#include <HAL_RTC.h>
#include <BDK_Task.h>
#include <BSP_Components.h>
#include <BLE_Components.h>
#include <ics/CS.h>
#include <ics/CS_Nodes.h>

#include "app_trace.h"
#include "app_timer.h"
#include "app_ble_hooks.h"
#include "app_sleep.h"

/* Configure RF 48 MHz XTAL divided clock frequency in Hz
 * Options: 8, 12, 16, 24, 48 */
#define RFCLK_FREQ                      8000000

/* Define clock divider and flash timings depending on RF clock frequency */
#if (RFCLK_FREQ == 8000000)
#define RF_CK_DIV_PRESCALE_VALUE        CK_DIV_1_6_PRESCALE_6_BYTE
#define SLOWCLK_PRESCALE_VALUE          SLOWCLK_PRESCALE_8
#define BBCLK_PRESCALE_VALUE            BBCLK_PRESCALE_1
#define DCCLK_PRESCALE_BYTE_VALUE       DCCLK_PRESCALE_2_BYTE
#define FLASH_DELAY_VALUE               FLASH_DELAY_FOR_SYSCLK_8MHZ
#define BBCLK_DIVIDER_VALUE             BBCLK_DIVIDER_8
#elif (RFCLK_FREQ == 12000000)
#define RF_CK_DIV_PRESCALE_VALUE        CK_DIV_1_6_PRESCALE_4_BYTE
#define SLOWCLK_PRESCALE_VALUE          SLOWCLK_PRESCALE_12
#define BBCLK_PRESCALE_VALUE            BBCLK_PRESCALE_1
#define DCCLK_PRESCALE_BYTE_VALUE       DCCLK_PRESCALE_3_BYTE
#define FLASH_DELAY_VALUE               FLASH_DELAY_FOR_SYSCLK_12MHZ
#define BBCLK_DIVIDER_VALUE             BBCLK_DIVIDER_12
#elif (RFCLK_FREQ == 16000000)
#define RF_CK_DIV_PRESCALE_VALUE        CK_DIV_1_6_PRESCALE_3_BYTE
#define SLOWCLK_PRESCALE_VALUE          SLOWCLK_PRESCALE_16
#define BBCLK_PRESCALE_VALUE            BBCLK_PRESCALE_2
#define DCCLK_PRESCALE_BYTE_VALUE       DCCLK_PRESCALE_4_BYTE
#define FLASH_DELAY_VALUE               FLASH_DELAY_FOR_SYSCLK_16MHZ
#define BBCLK_DIVIDER_VALUE             BBCLK_DIVIDER_8
#elif (RFCLK_FREQ == 24000000)
#define RF_CK_DIV_PRESCALE_VALUE        CK_DIV_1_6_PRESCALE_2_BYTE
#define SLOWCLK_PRESCALE_VALUE          SLOWCLK_PRESCALE_24
#define BBCLK_PRESCALE_VALUE            BBCLK_PRESCALE_3
#define DCCLK_PRESCALE_BYTE_VALUE       DCCLK_PRESCALE_6_BYTE
#define FLASH_DELAY_VALUE               FLASH_DELAY_FOR_SYSCLK_24MHZ
#define BBCLK_DIVIDER_VALUE             BBCLK_DIVIDER_8
#elif (RFCLK_FREQ == 48000000)
#define RF_CK_DIV_PRESCALE_VALUE        CK_DIV_1_6_PRESCALE_1_BYTE
#define SLOWCLK_PRESCALE_VALUE          SLOWCLK_PRESCALE_48
#define BBCLK_PRESCALE_VALUE            BBCLK_PRESCALE_6
#define DCCLK_PRESCALE_BYTE_VALUE       DCCLK_PRESCALE_12_BYTE
#define FLASH_DELAY_VALUE               FLASH_DELAY_FOR_SYSCLK_48MHZ
#define BBCLK_DIVIDER_VALUE             BBCLK_DIVIDER_8
#endif    /* if (RFCLK_FREQ == 8000000) */

#define APP_STATE_IND_LED_INTERVAL_MS  (50)

enum App_StateStruct
{
    APP_STATE_INIT,
    APP_STATE_START_ADVERTISING,
    APP_STATE_ADVERTISING,
    APP_STATE_SLEEP,
    APP_STATE_START_CONNECTION,
    APP_STATE_CONNECTED
};

extern enum App_StateStruct app_state;

extern struct sleep_mode_env_tag sleep_mode_env;


extern void Device_Initialize(void);

extern void Device_Initialize_WakeUp(void);

extern void BLE_Initialize(void);

extern void App_Env_Initialize(void);

void ledNotif(uint8_t cnt);

void ledNotif2(uint8_t cnt, uint8_t period);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_APP_H_ */

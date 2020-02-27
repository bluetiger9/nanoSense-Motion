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

#ifndef APP_RTC_H_
#define APP_RTC_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


#define HAL_RTC_RELOAD_VALUE           (0x7FFFFFFFU)

#define HAL_RTC_XTAL_FREQ              (32768U)

#define HAL_RTC_MAX_TICK_VALUE         (0xFFFFFFFFU)


#define HAL_RTC_S_TO_TICKS(s)          ( s * HAL_RTC_XTAL_FREQ)
#define HAL_RTC_MS_TO_TICKS(ms)        (ms * HAL_RTC_XTAL_FREQ / 1000U)
#define HAL_RTC_US_TO_TICKS(us)        (us * HAL_RTC_XTAL_FREQ / 1000000U)
#define HAL_RTC_NS_TO_TICKS(ns)        (ns / (1000000000U / HAL_RTC_XTAL_FREQ))

/**
 *
 * 32K XTAL has to be running before initializing RTC.
 */
extern void HAL_RTC_Initialize(void);

/** \brief Wake-up routine to be done after RSL10 wakes up from deep sleep.
 *
 * This has to be the first RTC HAL function to be called after wake-up.
 * Afterwards the global interrupt mask must be cleared before calling any other
 * RTC function so that RTC Alarm ISR can be called if wake-up was caused by
 * RTC.
 */
extern void HAL_RTC_Wakeup(void);

extern uint32_t HAL_RTC_GetTime(void * hint);

extern void HAL_RTC_SetAlarmS(uint32_t sec);

extern void HAL_RTC_SetAlarmMs(uint32_t ms);

extern void HAL_RTC_SetAlarmUs(uint32_t us);

extern void HAL_RTC_SetAlarmTicks(uint32_t ticks);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_APP_RTC_H_ */

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
//
//-----------------------------------------------------------------------------
#ifndef APP_TIMER_H_
#define APP_TIMER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stimer.h>

#include <HAL_RTC.h>

#define APP_TIMER_ALARM_NOW            (0)
#define APP_TIMER_ALARM_SET            (1)
#define APP_TIMER_NO_EVENT             (2)

/** \brief Number of nanoseconds elapsed per single RTC clock tick. */
#define APP_TIMER_NS_PER_RTC_TICK      (1000000000U / HAL_RTC_XTAL_FREQ)

/** If number of ticks remaining till next timer event is below this threshold
 * the \ref Timer_SetWakeupAtNextEvent will treat the timer as expired and
 * return APP_TIMER_ALARM_NOW.
 */
#define APP_TIMER_KEEP_AWAKE_THRESH    (HAL_RTC_MS_TO_TICKS(1))

extern struct stimer_ctx app_timer_ctx;


extern void Timer_Initialize(void);

extern void Timer_Wakeup(void);

static inline struct stimer_ctx * Timer_GetContext(void)
{
    return &app_timer_ctx;
}


extern int32_t Timer_SetWakeupAtNextEvent(void);


#ifdef __cplusplus
}
#endif

#endif /* APP_TIMER_H_ */

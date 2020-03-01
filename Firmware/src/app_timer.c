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

#include <HAL_RTC.h>
#include "app_timer.h"
#include "app_trace.h"

#include <rsl10.h>


//#define _TIMER_DEBUG


struct stimer_ctx app_timer_ctx = { 0 };

static bool is_duration_ge(struct stimer_duration * lhs,
        struct stimer_duration * rhs)
{
    bool is_ge = false;
    if (lhs->seconds > rhs->seconds)
    {
        is_ge = true;
    }
    else if (lhs->seconds == rhs->seconds)
    {
        if (lhs->nanoseconds >= rhs->nanoseconds)
        {
            is_ge = true;
        }
    }
    return is_ge;
}

static inline void duration_diff(struct stimer_duration * lhs,
        struct stimer_duration *rhs, struct stimer_duration *result)
{
    result->seconds = rhs->seconds - lhs->seconds;

    if (rhs->nanoseconds >= lhs->nanoseconds)
    {
        result->nanoseconds = rhs->nanoseconds - lhs->nanoseconds;
    }
    else
    {
        result->nanoseconds = 1000000000U + rhs->nanoseconds - lhs->nanoseconds;
        result->seconds -= 1;
    }
}

static inline bool duration_is_zero(struct stimer_duration *dur)
{
    return (dur->seconds == 0) && (dur->nanoseconds == 0);
}

void Timer_Initialize(void)
{
    HAL_RTC_Initialize();

    stimer_init_context(&app_timer_ctx, NULL, &HAL_RTC_GetTime,
            HAL_RTC_MAX_TICK_VALUE, APP_TIMER_NS_PER_RTC_TICK);
}

void Timer_Wakeup(void)
{
    HAL_RTC_Wakeup();
}

int32_t Timer_SetWakeupAtNextEvent(void)
{
    struct stimer_duration diff;
    int32_t retval = APP_TIMER_NO_EVENT;
    struct stimer_duration ts_next_diff = {UINT32_MAX, UINT32_MAX};
    struct stimer *ts_next = NULL;

    // Update timestamps of all registered timers to the same checkpoint.
    stimer_execute_context(&app_timer_ctx);

    // Find timer which will expire next.
    for (struct stimer *ts = app_timer_ctx.root; ts != NULL; ts = ts->next)
    {
        if (ts->is_running && !duration_is_zero(&ts->expire_interval))
        {
            if (is_duration_ge(&ts->elapsed, &ts->expire_interval))
            {
                // There is timer event that is already expired.
                retval = APP_TIMER_ALARM_NOW;
                return retval;
            }
            else
            {
                duration_diff(&ts->elapsed, &ts->expire_interval, &diff);

                if (!is_duration_ge(&diff, &ts_next_diff))
                {
                    ts_next_diff = diff;
                    ts_next = ts;
                }
            }
        }
    }

    // Set RTC wake up event for found timer event.
    if (ts_next != NULL)
    {
        uint32_t ticks = HAL_RTC_S_TO_TICKS(ts_next_diff.seconds)
                + HAL_RTC_NS_TO_TICKS(ts_next_diff.nanoseconds) + 1;

        if (ticks <= APP_TIMER_KEEP_AWAKE_THRESH)
        {
            retval = APP_TIMER_ALARM_NOW;

#ifdef _TIMER_DEBUG
            TRACE_PRINTF("%s:%d: Next event below threshold.\r\n", __FUNCTION__,
                    __LINE__);
#endif
        }
        else
        {
            HAL_RTC_SetAlarmTicks(ticks);

#ifdef _TIMER_DEBUG
            TRACE_PRINTF("%s:%d: Setting RTC alarm to %lu ticks.\r\n",
                    __FUNCTION__, __LINE__, ticks);
#endif
            TRACE_PRINTF("Next RTC alarm %.4f sec.\r\n", ticks / 32768.0f);

            retval = APP_TIMER_ALARM_SET;
        }
    }

    // No timer event is scheduled
    return retval;
}

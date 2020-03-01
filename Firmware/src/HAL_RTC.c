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

#include <rsl10.h>
#include <HAL_RTC.h>

//#define _HAL_RTC_DEBUG
#ifdef _HAL_RTC_DEUG
#include "api/led_api.h"
#include "app_trace.h"
#endif


/* \brief Incrementing RTC tick counter.
 *
 * Keeps track of elapsed time when RTC counter is restarted with custom value
 * to schedule RTC alarm.
 *
 * \note
 * This value will overflow approx. every 36 hours.
 */
volatile uint32_t rtc_time = 0;

/* Last seen RTC_COUNT value that was read and added to rtc_time.
 * Used to calculate number of RTC ticks since last time read. */
volatile uint32_t rtc_checkpoint = 0;

volatile bool rtc_ignore_irq = false;

/* debug only */
#ifdef _HAL_RTC_DEBUG
volatile uint32_t rtc_irq_calls = 0;
#endif /* _HAL_RTC_DEBUG */


/** \brief Reads value of RTC_COUNT register while accounting for
 *  synchronization between SYSCLK and RTC_CLK clock domains.
 *
 *  As the read of ACS_RTC_COUNT is not atomic (byte wise read in the ACS
 *  bridge), three reads are performed to make sure the counter is not clocked
 *  in the middle of the read.
 *
 *  \pre
 *  All interrupts must be disabled to prevent delays between subsequent reads
 *  of RTC_COUNT register.
 *
 *  \returns
 *  Current value of RTC_COUNT register.
 */
uint32_t HAL_RTC_ReadCount(void)
{
    uint32_t read1 = ACS->RTC_COUNT;
    uint32_t read2 = ACS->RTC_COUNT;

    if (read1 != read2)
    {
        uint32_t read3 = ACS->RTC_COUNT;
        return read3;
    }
    else
    {
        return read1;
    }
}


void HAL_RTC_Initialize(void)
{
    NVIC_DisableIRQ(RTC_ALARM_IRQn);
    NVIC_DisableIRQ(RTC_CLOCK_IRQn);
    NVIC_ClearPendingIRQ(RTC_ALARM_IRQn);
    NVIC_ClearPendingIRQ(RTC_CLOCK_IRQn);

    // Reset RTC counter and checkpoint to default values.
    rtc_time = 0;
    rtc_checkpoint = HAL_RTC_RELOAD_VALUE;

    // Restart of the RTC will cause an RTC alarm interrupt on next RTC clock
    // tick. We want to ignore this interrupt.
    rtc_ignore_irq = true;

    // Set initial reload value. This is big number to minimize number of RTC
    // alarm wake ups.
    ACS->RTC_CFG = HAL_RTC_RELOAD_VALUE;

    // Set RTC clock source to 32kHz oscillator
    // Set RTC alarm generation when RTC_COUNT counts down to 0.
    // Reset RTC_COUNT register to 0 !!!
    // Enable RTC.
    ACS->RTC_CTRL = RTC_RESET | RTC_ALARM_ZERO | RTC_CLK_SRC_XTAL32K
            | RTC_ENABLE;

    NVIC_EnableIRQ(RTC_ALARM_IRQn);
}

void HAL_RTC_Wakeup(void)
{
    // Check if this wake up was caused by RTC alarm.
    if (ACS_WAKEUP_STATE->WAKEUP_SRC_BYTE == WAKEUP_DUE_TO_RTC_ALARM_BYTE)
    {
        // If so the RTC COUNT register value has been reloaded on wake-up
        // Therefore time in this moment is:
        //   rtc_time = rtc_time + rtc_checkpoint + (RTC->CFG - RTC->COUNT)
        rtc_time += rtc_checkpoint + (ACS->RTC_CFG - HAL_RTC_ReadCount());

        // Restore default reload value after RTC wake up.
        if (ACS->RTC_CFG != HAL_RTC_RELOAD_VALUE)
        {
            // Reset CFG and checkpoint to default reload value to prevent
            // unnecessary interrupts / wake-ups until application sets new alarm
            // time.
            rtc_ignore_irq = true;
            rtc_checkpoint = HAL_RTC_RELOAD_VALUE;
            ACS->RTC_CFG = HAL_RTC_RELOAD_VALUE;
            ACS_RTC_CTRL->RESET_ALIAS = RTC_RESET_BITBAND;

            // Clear interrupt flag since RTC was restarted.
            NVIC_ClearPendingIRQ(RTC_ALARM_IRQn);
        }
    }

    // Re-enable RTC Alarm interrupt to catch counter reloads while application
    // is running.
    NVIC_EnableIRQ(RTC_ALARM_IRQn);
}


uint32_t HAL_RTC_GetTime(void * hint)
{
    uint32_t count_check;
    uint32_t count2;
    bool rtc_alarm_pending;

    // BEGIN CRITICAL SECTION

    // Disable all interrupts to make sure this part of code is not preempted.
    __disable_irq();

    // Read RTC_COUNT value for counter reload test
    count_check = HAL_RTC_ReadCount();
    // Read RTC Alarm ISR status
    rtc_alarm_pending = NVIC_GetPendingIRQ(RTC_ALARM_IRQn);
    // Read RTC_COUNT second time
    count2 = HAL_RTC_ReadCount();

    // Determine if RTC counter overflowed.
    // RTC_COUNT is counting down; count2 <= count_check if no reload occurred
    if ((rtc_alarm_pending == true) || (count_check < count2))
    {
        // Manually add time that elapsed between last checkpoint and RTC_COUNT
        // reload.
        rtc_time += rtc_checkpoint;

        // Set new checkpoint to current reload value.
        rtc_checkpoint = ACS->RTC_CFG;

        NVIC_ClearPendingIRQ(RTC_ALARM_IRQn);
    }

    // Add ticks elapsed since last checkpoint to the software counter.
    rtc_time += rtc_checkpoint - count2;
    // Set checkpoint to last read counter value.
    rtc_checkpoint = count2;

    // Reenable interrupts after overflow checks are done.
    __enable_irq();

    // END CRITICAL SECTION



#ifdef _HAL_RTC_DEBUG
    if ((rtc_alarm_pending == true) || (count_check < count2))
    {
        TRACE_PRINTF("%s:%d: IRQ_PEND=%d, count_check=%lu, count2=%lu\r\n\n",
                __FUNCTION__, __LINE__, rtc_alarm_pending, count_check, count2);

        TRACE_PRINTF("%s:%d: T=%lu, CH=%lu, CFG=%lu, CNT=%lu CTRL=0x%lX IRQ=%lu\r\n",
                __FUNCTION__, __LINE__, rtc_time, rtc_checkpoint, ACS->RTC_CFG,
                HAL_RTC_ReadCount(), ACS->RTC_CTRL, rtc_irq_calls);
    }

#endif /* _HAL_RTC_DEBUG */

    return rtc_time;
}

void HAL_RTC_SetAlarmS(uint32_t sec)
{
    HAL_RTC_SetAlarmTicks(HAL_RTC_S_TO_TICKS(sec));
}

void HAL_RTC_SetAlarmMs(uint32_t ms)
{
    HAL_RTC_SetAlarmTicks(HAL_RTC_MS_TO_TICKS(ms));
}

void HAL_RTC_SetAlarmUs(uint32_t us)
{
    HAL_RTC_SetAlarmTicks(HAL_RTC_US_TO_TICKS(us));
}

void HAL_RTC_SetAlarmTicks(uint32_t ticks)
{
    uint32_t count_check;
    uint32_t count2;
    bool rtc_alarm_pending;

    // BEGIN CRITICAL SECTION

    __disable_irq();

    // Read RTC_COUNT value for counter reload test
    count_check = HAL_RTC_ReadCount();
    // Read RTC Alarm ISR status
    rtc_alarm_pending = NVIC_GetPendingIRQ(RTC_ALARM_IRQn);
    // Read RTC_COUNT second time
    count2 = HAL_RTC_ReadCount();

    // Determine if RTC counter overflowed.
    // RTC_COUNT is counting down; count2 <= count_check if no reload occurred
    if ((rtc_alarm_pending == true) || (count_check < count2))
    {
        // Manually add time that elapsed between last checkpoint and RTC_COUNT
        // reload.
        rtc_time += rtc_checkpoint;

        // Set new checkpoint to current reload value.
        rtc_checkpoint = ACS->RTC_CFG;

        NVIC_ClearPendingIRQ(RTC_ALARM_IRQn);
    }

    // Add ticks elapsed since last checkpoint to the software counter.
    rtc_time += rtc_checkpoint - count2;
    // Set new reload value to the number of ticks till next RTC alarm.
    ACS->RTC_CFG = ticks;
    // Reset RTC counter register.
    ACS_RTC_CTRL->RESET_ALIAS = RTC_RESET_BITBAND; // RTC_COUNT is reset to 0.
    // Set checkpoint to new reload value.
    rtc_checkpoint = ticks;
    rtc_ignore_irq = true;

    // END CRITICAL SECTION

    __enable_irq();

#ifdef _HAL_RTC_DEBUG
        TRACE_PRINTF("%s:%d: TIME=%lu, CFG=%lu COUNT=%lu\r\n", __FUNCTION__,
                __LINE__, rtc_time, ACS->RTC_CFG, HAL_RTC_ReadCount());
#endif /* _HAL_RTC_DEBUG */
}

void RTC_ALARM_IRQHandler(void)
{
    if (rtc_ignore_irq == true)
    {
        rtc_ignore_irq = false;
    }
    else
    {

        // Add ticks elapsed between last time read and counter reload.
        rtc_time += rtc_checkpoint;

        // Reset checkpoint to current reload value.
        rtc_checkpoint = ACS->RTC_CFG;

        // Do not modify or reset RTC from ISR.

#ifdef _HAL_RTC_DEBUG
        rtc_irq_calls += 1;
#endif
    }
}

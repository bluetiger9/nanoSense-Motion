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
//! \file SoftwareTimer.h
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup SW_TIMER_GRP Software Timer
//!
//! \brief Allows creation of unlimited number of software timers with Ticker,
//! Timeout and Timer functionality.
//!
//! This library requires one of the General Purpose Timers of RSL10.
//! This can be configured in the RTE_SoftwareTimer.h header in applications
//! that are using the CMSIS-Pack component.
//!
//! \{
//-----------------------------------------------------------------------------
#ifndef SOFTWARE_TIMER_H_
#define SOFTWARE_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "stimer.h"

/** \brief How much time passes between two ticks of TIMERx clock source. */
#define SW_TIMER_TIM1_US_PER_TICK      (2)

/** \brief Maximum value of TIMERx value register.
 *
 * TIMER1 has 24-bit long counter.
 */
#define SW_TIMER_TIM1_MAX_TICKS        (0xFFFFFF)

/** \brief Maximal possible resolution of software timer in microseconds.
 *
 * This corresponds to single TIMERx tick.
 *
 * Resolution determines how often to check active timers expiration.
 */
#define SW_TIMER_MAX_RESOLUTION        (2)

/** \brief Minimal resolution of software timer in microseconds.
 *
 * Computed as time elapsed between two timer ticks times size of timer
 * counter.
 *
 * Resolution determines how often to check active timers expiration.
 */
#define SW_TIMER_MIN_RESOLUTION        (SW_TIMER_TIM1_US_PER_TICK * SW_TIMER_TIM1_MAX_TICKS)

/** \brief Function prototype for timer event callback functions.
 *
 * \see SwTimer_Attach for more details and limitations of callbacks.
 */
typedef void (*SwTimer_Callback)(void* arg);

/** \brief Internal structure of timer. */
struct SwTimer
{
    /** \brief Underlying timer object. */
    struct stimer timer;

    /** \brief Callback to be called when timer expires. */
    SwTimer_Callback event_callback;

    /** \brief Optional argument for event callback. */
    void *event_arg;

    /** \brief Determines whether the callback will be called directly from
     * timer ISR or passed to event queue to be processed by Event Kernel.
     */
    bool scheduled;

    /** \brief * Prevents repeated execution of attached callback until its
     * expiration time is modified.
     */
    bool pending;
};

/** \brief Duration structure for holding of time elapsed since timer
 * started.
 */
struct SwTimer_Duration {
    /** \brief Number of elapsed seconds. */
    uint32_t seconds;

    /** \brief Number of elapsed nanoseconds. */
    uint32_t nanoseconds;
};

//! \{
//! \name Timer context management functions
//!
//! \brief These functions affect operation of all software timers.
//!

/** \brief Allows to set custom software timer resolution.
 *
 * This function allows to set custom timer resolution allowing to adapt timer
 * to different power / performance requirements of applications.
 *
 * If this function is not called before first timer initialization it will use
 * the default tick resolution.
 *
 * \param resolution_us
 * The minimum time interval that software timers will be able to detect.
 * Minimum value is 2us.
 *
 * \pre
 * This function must be called before any timers are initialized by
 * SwTimer_Initialize.
 *
 * \note
 * Selecting a high resolution may require higher SYSCLK frequencies to have
 * enough time to check all timers in the interrupt routine.
 * See BDK_InitializeFreq on how to set higher SYSCLK frequencies.
 */
extern void SwTimer_CTX_Initialize(uint32_t resolution_us);

/** \brief Stops hardware timer, completely stopping all running timers.
 *
 * Running timers cannot detect that their execution was paused since the
 * internal tick counter will not be incremented while paused.
 *
 * This can be used to save power when the CPU is preparing to enter prolonged
 * deep sleep mode and relies on other resources (RTC, ext. wakeup) to provide
 * the wakeup signal.
 *
 */
extern void SwTimer_CTX_Pause(void);

/** \brief Restarts hardware timer, resuming normal operation. */
extern void SwTimer_CTX_Resume(void);

//! \}

/** \brief Initializes timer structure with default values and adds it to list
 * of active timers.
 *
 * Active timer does not guarantee that timer is also counting.
 *
 * \warning
 * Calling any of the SwTimer_* functions before SwTimer_Initialize will result in undefined behavior.
 *
 * \param tim
 * Pointer to timer handle structure.
 */
extern void SwTimer_Initialize(struct SwTimer *tim);

/** \brief Stops timer and remove it from active timer list.
 *
 * This should be called for all temporary timers or if the timer is not being
 * used.
 * SwTimer_Initialize has to be called again to reuse a previously removed
 * timer.
 *
 * \note
 * If there are no active timers left the underlying Timer peripheral will be
 * stopped to save power.
 * The internal time will not advance while there are no active timers.
 *
 * \param tim
 * Pointer to timer handle structure.
 */
extern void SwTimer_Remove(struct SwTimer *tim);

//! \{
//! \name Ticker / Timeout mode functions
//!
//! \brief Can be used for periodic or delayed execution of events by providing
//! callback function.
//!
//! The Ticker and Timeout modes call attached callback function after timer
//! expires.
//!
//! In order to create a repeating timer (Ticker) the callback function has to
//! set next expiration period either by Advance function to reuse last
//! expiration time or use one of ExpireIn functions to define new expiration
//! time.
//!
//! To create an one-shot timer (Timeout) the callback function should not call
//! either Advance or ExpireIn functions.
//! If this timer will not be reused again it should be removed from active
//! timer list by calling SwTimer_Remove instead.
//!

/** \brief Attaches an callback function to be executed when timer expires.
 *
 *
 *
 *
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param callback
 * Callback function to be attached to timer or NULL to remove existing
 * callback.
 *
 * Callback must execute either of Advance, ExpireIn or Stop functions to clear
 * the timer expiration flag.
 * Failing to do so will result in timer callback being called on every timer
 * tick.
 *
 * Timers cannot be removed directly from callback function.
 * This operation should be off loaded to application.
 *
 * \warning
 * Callback functions are executed inside Timer ISR.
 * As such they should require only small amount of computation time and must
 * not use any blocking calls.
 * If timer is used to trigger more complex operations it can use the
 * \ref BDK_TaskSchedule function to off load this task to application context.
 *
 * \deprecated
 * Deprecated as of 0.2.0 and will be removed in public release 1.0.0.
 * This function is superseded by \ref SwTimer_AttachInt and
 * \ref SwTimer_AttachScheduled.
 *
 * \param arg
 * Argument that will be passed to callback or NULL if no argument is provided.
 *
 */
extern void SwTimer_Attach(struct SwTimer *tim, SwTimer_Callback callback,
        void* arg) __attribute__ ((deprecated));

/** \brief Attaches an callback function to be executed when timer expires.
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param callback
 * Callback function to be attached to timer or NULL to remove existing
 * callback.
 *
 * Callback must execute either of Advance, ExpireIn or Stop functions to clear
 * the timer expiration flag.
 * Failing to do so will result in timer callback not being called again.
 *
 * Timers cannot be removed directly from interrupt attached callback function.
 * This operation should be off loaded to application by using
 * \ref SwTimer_AttachScheduled.
 *
 * \warning
 * Callback functions are executed inside Timer ISR.
 * As such they should require only small amount of computation time and must
 * not use any blocking calls.
 * If timer is used to trigger more complex operations it can use the
 * \ref SwTimer_AttachScheduled function to off load this task to application
 * context.
 *
 * \param arg
 * Argument that will be passed to callback or NULL if no argument is provided.
 */
extern void SwTimer_AttachInt(struct SwTimer *tim, SwTimer_Callback callback,
        void* arg);

/** \brief Attaches an callback function to be scheduled when timer expires.
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param callback
 * Callback function to be attached to timer or NULL to remove existing
 * callback.
 *
 * Callback must execute either of Advance, ExpireIn or Stop functions to clear
 * the timer expiration flag.
 * Failing to do so will result in timer callback not being called again.
 *
 *
 * \warning
 * Callback functions are executed executed from Event Kernels message queue.
 * This means it will get executed when application calls either Kernel_Schedule
 * or BDK_Schedule functions.
 *
 * Slight delay may be experienced between timer expiring and callback
 * execution. SwTimer_Advance can be used to set next expiration time precisely
 * based on previous expiration time.
 *
 * \param arg
 * Argument that will be passed to callback or NULL if no argument is provided.
 */
extern void SwTimer_AttachScheduled(struct SwTimer *tim,
		SwTimer_Callback callback, void* arg);

/** \brief Set timer expiration time in seconds.
 *
 * It is used to start timer or to renew it after expiring.
 *
 * Renewing timer does not take the latency between timer expiring and calling
 * of ExpireIn into account.
 * See SwTimer_Advance for creation of stable timer with constant period.
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param sec
 * Timer expiration time in seconds.
 */
extern void SwTimer_ExpireInS(struct SwTimer *tim, uint32_t sec);

/** \brief Set timer expiration time in milliseconds.
 *
 * It is used to start timer or to renew it after expiring.
 *
 * Renewing timer does not take the latency between timer expiring and calling
 * of ExpireIn into account.
 * See SwTimer_Advance for creation of stable timer with constant period.
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param sec
 * Timer expiration time in milliseconds.
 */
extern void SwTimer_ExpireInMs(struct SwTimer *tim, uint32_t ms);

/** \brief Set timer expiration time in microseconds.
 *
 * It is used to start timer or to renew it after expiring.
 *
 * Renewing timer does not take the latency between timer expiring and calling
 * of ExpireIn into account.
 * See SwTimer_Advance for creation of stable timer with constant period.
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param sec
 * Timer expiration time in microseconds.
 */
extern void SwTimer_ExpireInUs(struct SwTimer *tim, uint32_t us);

/** \brief Returns whether this timer is expired.
 *
 * This function can be used within program main loop to check if timer has
 * expired.
 *
 * \returns
 * \b true - If timer is running longer than specified expire interval.
 * \b false - If timer
 */
extern bool SwTimer_IsExpired(struct SwTimer *tim);

/** \brief Renew timer expiration time with constant period.
 *
 * This function reuses last expiration time set by one of ExpireIn functions.
 *
 * \param tim
 * Pointer to timer handle structure.
 */
extern void SwTimer_Advance(struct SwTimer *tim);

//! \}
//! \{
//! \name Timer mode functions
//!
//! \brief Can be used to count time elapsed since timer started.
//!

/** \brief Starts timer counter.
 *
 * \param tim
 * Pointer to timer handle structure.
 */
extern void SwTimer_Start(struct SwTimer *tim);

/** \brief Stops timer counter.
 *
 * \param tim
 * Pointer to timer handle structure.
 */
extern void SwTimer_Stop(struct SwTimer *tim);

/** \brief Retrieve time elapsed since timer started.
 *
 * Can be called multiple times.
 *
 * \param tim
 * Pointer to timer handle structure.
 *
 * \param t
 * Timer duration structure to be filled with currently elapsed time.
 */
extern void SwTimer_GetElapsed(struct SwTimer *tim, struct SwTimer_Duration *t);

/** \brief Restarts running timer.
 *
 * Elapsed time is reset back to 0.
 *
 * \param tim
 * Pointer to timer handle structure.
 */
extern void SwTimer_Restart(struct SwTimer *tim);

//! \}

#endif /* SOFTWARE_TIMER_H_ */

//! \}
//! \}

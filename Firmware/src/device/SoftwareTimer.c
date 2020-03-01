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
//! \file SoftwareTimer.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup SW_TIMER_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "BDK.h"

#include "RTE_SoftwareTimer.h"
#include "SoftwareTimer.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

#define SW_TIMER_TIMER_IRQN            (TIMER0_IRQn + RTE_SW_TIMER_INSTANCE)
#define SW_TIMER_TIMER_SELECT          (1 << RTE_SW_TIMER_INSTANCE)

#if RTE_SW_TIMER_INSTANCE == 0
#define SW_TIMER_IRQ_HANDLER_NAME      TIMER0_IRQHandler
#elif RTE_SW_TIMER_INSTANCE == 1
#define SW_TIMER_IRQ_HANDLER_NAME      TIMER1_IRQHandler
#elif RTE_SW_TIMER_INSTANCE == 2
#define SW_TIMER_IRQ_HANDLER_NAME      TIMER2_IRQHandler
#elif RTE_SW_TIMER_INSTANCE == 3
#define SW_TIMER_IRQ_HANDLER_NAME      TIMER3_IRQHandler
#else
#error Invalid Timer instance for software timer
#endif

#define SW_TIMER_ENTER_CRITICAL() \
	NVIC_DisableIRQ(SW_TIMER_TIMER_IRQN); \
	__DSB(); \
	__ISB()

#define SW_TIMER_EXIT_CRITICAL() \
	NVIC_EnableIRQ(SW_TIMER_TIMER_IRQN)

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

void SW_TIMER_IRQ_HANDLER_NAME(void);
static inline void HW_TIMER_Initialize(const uint32_t timeout_val);
static inline void HW_TIMER_Pause(void);
static inline void HW_TIMER_Resume(void);
static uint32_t SwTimer_GetTime(void *hint);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

struct stimer_ctx sw_timer_ctx = { 0 };
volatile uint16_t sw_timer_counter = 0;
bool sw_timer_initialized = false;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void SwTimer_CTX_Initialize(uint32_t resolution_us)
{
    ASSERT_DEBUG(resolution_us >= SW_TIMER_MAX_RESOLUTION && resolution_us <= SW_TIMER_MIN_RESOLUTION);

    if (sw_timer_initialized == false)
    {
        uint32_t tim1_timeout_val = (resolution_us >> 1)
                & TIMER_VAL_TIMER_VALUE_Mask;

        stimer_init_context(&sw_timer_ctx,
                NULL, &SwTimer_GetTime, UINT16_MAX,
                (tim1_timeout_val << 1) * 1000);

        HW_TIMER_Initialize(tim1_timeout_val);

        sw_timer_initialized = true;
    }
}

void SwTimer_CTX_Pause(void)
{
    if (sw_timer_initialized == true)
    {
        HW_TIMER_Pause();
    }
}

void SwTimer_CTX_Resume(void)
{
    if (sw_timer_initialized == true)
    {
        HW_TIMER_Resume();
    }
}

void SwTimer_Initialize(struct SwTimer *tim)
{
    if (tim != NULL)
    {
        if (sw_timer_initialized == false)
        {
            SwTimer_CTX_Initialize(RTE_SW_TIMER_RESOLUTION);
        }

        SW_TIMER_ENTER_CRITICAL();

        tim->event_callback = NULL;
        tim->event_arg = NULL;
        tim->scheduled = false;
        tim->pending = false;

        stimer_init(&tim->timer, &sw_timer_ctx);

        SW_TIMER_EXIT_CRITICAL();
    }
}

void SwTimer_Remove(struct SwTimer *tim)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_remove(&tim->timer);

    if (sw_timer_ctx.root == NULL)
    {
    	SwTimer_CTX_Pause();
    	sw_timer_initialized = false;
    }


    SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_Attach(struct SwTimer *tim, SwTimer_Callback callback,
        void *arg)
{
	SW_TIMER_ENTER_CRITICAL();

    tim->event_callback = callback;
    tim->event_arg = arg;
    tim->scheduled = false;
    tim->pending = false;

    SW_TIMER_EXIT_CRITICAL();
}

extern void SwTimer_AttachInt(struct SwTimer *tim, SwTimer_Callback callback,
        void *arg)
{
	SW_TIMER_ENTER_CRITICAL();

    tim->event_callback = callback;
    tim->event_arg = arg;
    tim->scheduled = false;

    SW_TIMER_EXIT_CRITICAL();
}

extern void SwTimer_AttachScheduled(struct SwTimer *tim,
		SwTimer_Callback callback, void *arg)
{
	SW_TIMER_ENTER_CRITICAL();

	tim->event_callback = callback;
	tim->event_arg = arg;
	tim->scheduled = true;

	SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_ExpireInS(struct SwTimer *tim, uint32_t sec)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_expire_from_now_s(&tim->timer, sec);
    tim->pending = false;

    SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_ExpireInMs(struct SwTimer *tim, uint32_t ms)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_expire_from_now_ms(&tim->timer, ms);
    tim->pending = false;

    SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_ExpireInUs(struct SwTimer *tim, uint32_t us)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_expire_from_now_us(&tim->timer, us);
    tim->pending = false;

    SW_TIMER_EXIT_CRITICAL();
}

bool SwTimer_IsExpired(struct SwTimer *tim)
{
    bool is_expired = false;

    SW_TIMER_ENTER_CRITICAL();

    is_expired = stimer_is_expired(&tim->timer);

    SW_TIMER_EXIT_CRITICAL();

    return is_expired;
}

void SwTimer_Advance(struct SwTimer *tim)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_advance(&tim->timer);
    tim->pending = false;

    SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_Start(struct SwTimer *tim)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_start(&tim->timer);

    SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_Stop(struct SwTimer *tim)
{
	SW_TIMER_ENTER_CRITICAL();

    stimer_stop(&tim->timer);
    tim->pending = false;

    SW_TIMER_EXIT_CRITICAL();
}

void SwTimer_GetElapsed(struct SwTimer *tim, struct SwTimer_Duration *t)
{
    stimer_get_elapsed_time(&tim->timer, (struct stimer_duration*) t);
}

void SwTimer_Restart(struct SwTimer *tim)
{
    stimer_restart_from_now(&tim->timer);
}

void SW_TIMER_IRQ_HANDLER_NAME(void)
{
    /* Increment timebase. */
    sw_timer_counter += 1;

    /* Check all linked timers for expiration. */
    struct stimer *ts;
    struct SwTimer *tim;

    for (ts = sw_timer_ctx.root; ts != NULL; ts = ts->next)
    {
        tim = (struct SwTimer*) ts;

        /* * Only check timers that have attached callbacks.
         *   Timers without callbacks either do not have expiration period or
         *   are checked by application using SwTimer_IsExpired.
         * * Check if this is the first time this timer is deemed as expired.
         *   This check is necessary for Event Kernel scheduled callbacks
         *   which may not be executed by the time this interrupt is generated
         *   again.
         */
        if (tim->event_callback != NULL && tim->pending == false)
        {
            if (ts->is_running == true && stimer_is_expired(ts) == true)
            {
                tim->pending = true;

                if (tim->scheduled == true)
                {
                    BDK_TaskSchedule(tim->event_callback, tim->event_arg);
                }
                else
                {
                    tim->event_callback(tim->event_arg);
                }
            }
        }
    }
}

static inline void HW_TIMER_Initialize(const uint32_t timeout_val)
{
    /* Make sure SLOWCLK is running at 1MHz. */
    ASSERT_DEBUG(SystemCoreClock / ((CLK->DIV_CFG0 & CLK_DIV_CFG0_SLOWCLK_PRESCALE_Mask) + 1) == 1000000);

    /* Schedule timeout every (timeout_val * 2) microseconds. */
    Sys_Timer_Set_Control(RTE_SW_TIMER_INSTANCE,
            TIMER_FREE_RUN | TIMER_SLOWCLK_DIV2 | TIMER_PRESCALE_1
                    | timeout_val);

    /* Enable Timer1 interrupts */
    NVIC_ClearPendingIRQ(SW_TIMER_TIMER_IRQN);
    NVIC_EnableIRQ(SW_TIMER_TIMER_IRQN);

    /* Start timer. */
    Sys_Timers_Start(SW_TIMER_TIMER_SELECT);
}

static inline void HW_TIMER_Pause(void)
{
    Sys_Timers_Stop(SW_TIMER_TIMER_SELECT);
    NVIC_DisableIRQ(SW_TIMER_TIMER_IRQN);
    NVIC_ClearPendingIRQ(SW_TIMER_TIMER_IRQN);
}

static inline void HW_TIMER_Resume(void)
{
    NVIC_ClearPendingIRQ(SW_TIMER_TIMER_IRQN);
    NVIC_EnableIRQ(SW_TIMER_TIMER_IRQN);
    Sys_Timers_Start(SW_TIMER_TIMER_SELECT);
}

static uint32_t SwTimer_GetTime(void *hint)
{
    return sw_timer_counter;
}

//! \}
//! \}

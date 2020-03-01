/**
 * Copyright (c) 2016 Bradley Kim Schleusner < bradschl@gmail.com >
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stddef.h>
#include <stdlib.h>

#include "stimer.h"
#include "timermath.h"

// -------------------------------------------------------------- Private types



// ---------------------------------------------------------- Private functions

// ------------ Time duration functions

// NOTE: had to change ns_advance to 64 bit integer to accommodate for longer
// delays between timer updates.
static inline void
advance_duration_ns(struct stimer_duration * td, uint64_t ns_advance)
{
    // Need to be really careful about overflows in here, hence strange math
    uint32_t headroom = 1000000000U - td->nanoseconds;

    while (ns_advance >= 1000000000U)
    {
        td->seconds += 1;
        ns_advance -= 1000000000U;
    }

    if (ns_advance < headroom) {
        td->nanoseconds += ns_advance;
    } else {
        td->seconds += 1;
        td->nanoseconds = ns_advance - headroom;
    }
}


static inline bool
is_duration_ge(struct stimer_duration * lhs, struct stimer_duration * rhs)
{
    bool is_ge = false;
    if (lhs->seconds > rhs->seconds) {
        is_ge = true;
    } else if (lhs->seconds == rhs->seconds) {
        if (lhs->nanoseconds >= rhs->nanoseconds) {
            is_ge = true;
        }
    }
    return is_ge;
}


static inline void
set_duration_s(struct stimer_duration * td, uint32_t s)
{
    td->seconds = s;
    td->nanoseconds = 0;
}


static inline void
set_duration_ms(struct stimer_duration * td, uint32_t ms)
{
    td->seconds = ms / 1000;
    td->nanoseconds = (ms % 1000) * 1000000;
}


static inline void
set_duration_us(struct stimer_duration * td, uint32_t us)
{
    td->seconds = us / 1000000;
    td->nanoseconds = (us % 1000000) * 1000;
}


static inline void
set_duration_ns(struct stimer_duration * td, uint32_t ns)
{
    td->seconds = ns / 1000000000;
    td->nanoseconds = ns % 1000000000;
}


// -------------------- Timer functions

static void
link_timer(struct stimer_ctx * ctx, struct stimer * ts)
{
    ts->ctx = ctx;

    if (NULL == ctx->root) {
        ts->next = NULL;
    } else {
        ts->next = ctx->root;
    }

    ctx->root = ts;
}


static void
unlink_timer(struct stimer * ts)
{
    struct stimer * next = ts->next;
    struct stimer_ctx * ctx = ts->ctx;
    ts->next = NULL;
    ts->ctx = NULL;

    if (NULL != ctx) {
        if (ctx->root == ts) {
            ctx->root = next;
        } else {
            struct stimer * prev = NULL;
            for (prev = ctx->root; NULL != prev; prev = prev->next) {
                if (ts == prev->next) {
                    prev->next = next;
                    break;
                }
            }
        }
    }
}


static inline void
checkpoint_timer(struct stimer * ts, struct tm_math * tm, uint32_t now)
{
    if (ts->is_running) {
        int32_t diff = tm_get_diff(tm, now, ts->checkpoint);
        if (diff > 0) {
            uint64_t ns_advance = (uint64_t)diff * (uint64_t)ts->ctx->ns_per_count;
            advance_duration_ns(&ts->elapsed, ns_advance);
            ts->checkpoint = now;
        }
    }
}


static inline void
checkpoint_timer_2(struct stimer * ts)
{
    if (ts->is_running) {
        struct stimer_ctx * ctx = ts->ctx;
        uint32_t now = ctx->get_time_fn(ctx->hint);
        checkpoint_timer(ts, &ctx->tm, now);
    }
}


static inline void
start_and_checkpoint_timer(struct stimer * ts)
{
    ts->checkpoint = ts->ctx->get_time_fn(ts->ctx->hint);
    ts->is_running = true;

    ts->elapsed.seconds = 0;
    ts->elapsed.nanoseconds = 0;
}


static inline void
timer_subtract_from_elapsed(struct stimer * ts, struct stimer_duration * td)
{
    if (is_duration_ge(&ts->elapsed, td)) {
        ts->elapsed.seconds -= td->seconds;
        if (ts->elapsed.nanoseconds >= td->nanoseconds) {
            ts->elapsed.nanoseconds -= td->nanoseconds;
        } else {
            ts->elapsed.seconds -= 1;
            ts->elapsed.nanoseconds += (1000000000 - td->nanoseconds);
        }
    } else {
        ts->elapsed.seconds = 0;
        ts->elapsed.nanoseconds = 0;
    }
}


// ----------------------------------------------------------- Public functions

// ---------------------- Timer context

struct stimer_ctx *
stimer_alloc_context(void * hint,
                     stimer_get_time_fn get_time_fn,
                     uint32_t max_time,
                     uint32_t ns_per_count)
{
    struct stimer_ctx * ctx = (struct stimer_ctx *) malloc(sizeof(struct stimer_ctx));

    if (NULL != ctx) {
        ctx->root = NULL;

        tm_initialize(&ctx->tm, max_time);

        ctx->ns_per_count = ns_per_count;
        ctx->get_time_fn = get_time_fn;
        ctx->hint = hint;
    }

    return ctx;
}

void
stimer_init_context(struct stimer_ctx *ctx,
                    void * hint,
                    stimer_get_time_fn get_time_fn,
                    uint32_t max_time,
                    uint32_t ns_per_count)
{
    if (NULL != ctx) {
        ctx->root = NULL;

        tm_initialize(&ctx->tm, max_time);

        ctx->ns_per_count = ns_per_count;
        ctx->get_time_fn = get_time_fn;
        ctx->hint = hint;
    }
}


void
stimer_free_context(struct stimer_ctx * ctx)
{
    if (NULL != ctx) {
        while (NULL != ctx->root) {
            unlink_timer(ctx->root);
        }

        free(ctx);
    }
}


void
stimer_execute_context(struct stimer_ctx * ctx)
{
    if (NULL != ctx) {
        uint32_t now = ctx->get_time_fn(ctx->hint);

        struct stimer * ts;
        for (ts = ctx->root; NULL != ts; ts = ts->next) {
            checkpoint_timer(ts, &ctx->tm, now);
        }
    }
}


// ------------------------------ Timer

struct stimer *
stimer_alloc(struct stimer_ctx * ctx)
{
    struct stimer * ts = NULL;

    if (NULL != ctx) {
        ts = (struct stimer *) malloc(sizeof(struct stimer));
        if (NULL != ts) {
            ts->ctx = NULL;
            ts->next = NULL;

            ts->checkpoint = 0;

            ts->expire_interval.seconds = 0;
            ts->expire_interval.nanoseconds = 0;

            ts->elapsed.seconds = 0;
            ts->elapsed.nanoseconds = 0;
            ts->is_running = false;

            link_timer(ctx, ts);
        }
    }

    return ts;
}

void
stimer_init(struct stimer *ts, struct stimer_ctx * ctx)
{
    if (NULL != ctx) {
        if (NULL != ts) {
            ts->ctx = NULL;
            ts->next = NULL;

            ts->checkpoint = 0;

            ts->expire_interval.seconds = 0;
            ts->expire_interval.nanoseconds = 0;

            ts->elapsed.seconds = 0;
            ts->elapsed.nanoseconds = 0;
            ts->is_running = false;

            link_timer(ctx, ts);
        }
    }
}


void
stimer_free(struct stimer * ts)
{
    if (NULL != ts) {
        unlink_timer(ts);
        free(ts);
    }
}

void
stimer_remove(struct stimer * ts)
{
    if (NULL != ts) {
        unlink_timer(ts);
    }
}


// ------------ Elapsed timer functions

void
stimer_start(struct stimer * ts)
{
    if ((NULL != ts) && (NULL != ts->ctx)) {
        start_and_checkpoint_timer(ts);
    }
}


void
stimer_stop(struct stimer * ts)
{
    if ((NULL != ts) && (NULL != ts->ctx)) {
        if (ts->is_running) {
            checkpoint_timer_2(ts);
            ts->is_running = false;
        }
    }
}


void
stimer_get_elapsed_time(struct stimer * ts, struct stimer_duration * t)
{
    if ((NULL != ts) && (NULL != t)) {
        if (NULL != ts->ctx) {
            checkpoint_timer_2(ts);
        }

        *t = ts->elapsed;
    }
}


// ------------- Expire timer functions

void
stimer_expire_from_now(struct stimer * ts, struct stimer_duration * t)
{
    if ((NULL != ts) && (NULL != ts->ctx) && (NULL != t)) {
        start_and_checkpoint_timer(ts);
        ts->expire_interval = *t;
    }
}


void
stimer_expire_from_now_s(struct stimer * ts, uint32_t s)
{
    if ((NULL != ts) && (NULL != ts->ctx)) {
        start_and_checkpoint_timer(ts);
        set_duration_s(&ts->expire_interval, s);
    }
}


void
stimer_expire_from_now_ms(struct stimer * ts, uint32_t ms)
{
    if ((NULL != ts) && (NULL != ts->ctx)) {
        start_and_checkpoint_timer(ts);
        set_duration_ms(&ts->expire_interval, ms);
    }
}


void
stimer_expire_from_now_us(struct stimer * ts, uint32_t us)
{
    if ((NULL != ts) && (NULL != ts->ctx)) {
        start_and_checkpoint_timer(ts);
        set_duration_us(&ts->expire_interval, us);
    }
}


void
stimer_expire_from_now_ns(struct stimer * ts, uint32_t ns)
{
    if ((NULL != ts) && (NULL != ts->ctx)) {
        start_and_checkpoint_timer(ts);
        set_duration_ns(&ts->expire_interval, ns);
    }
}


bool
stimer_is_expired(struct stimer * ts)
{
    bool expired = false;
    if (NULL != ts) {
        if (NULL != ts->ctx) {
            checkpoint_timer_2(ts);
        }
        expired = is_duration_ge(&ts->elapsed, &ts->expire_interval);
    }
    return expired;
}


void
stimer_restart_from_now(struct stimer * ts)
{
    if ((NULL != ts) && (NULL != ts->ctx) && (ts->is_running)) {
        start_and_checkpoint_timer(ts);
    }
}


void
stimer_advance(struct stimer * ts)
{
    if ((NULL != ts) && (NULL != ts->ctx) && (ts->is_running)) {
        checkpoint_timer_2(ts);
        timer_subtract_from_elapsed(ts, &ts->expire_interval);
    }
}

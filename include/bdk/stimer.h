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

#ifndef STIMER_H_
#define STIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "timermath.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


// ----------------------------------------------------------- Timer structures

/**
 * @brief Function pointer prototype for getting the current time
 * @details The returned time must be an incrementing type, starting at 0,
 *          incrementing to its maximum value, and then rolling back to 0. The
 *          exact units do not matter.
 *          If the implementation references a count down time source, then it
 *          should invert the time source value, i.e.
 *              return (TIMER_MAX_VALUE - timer_value);
 *
 * @param hint Optional hint parameter. This pointer may be used for whatever
 *          the implementation wants, such as a this pointer. It can be set to
 *          NULL if it isn't used.
 * @return Current time
 */
typedef uint32_t (*stimer_get_time_fn)(void * hint);

struct stimer;

/**
 * Time elapsed structure
 */
struct stimer_duration {
    uint32_t seconds;
    uint32_t nanoseconds;
};


// ---------------------- Timer context
struct stimer_ctx {
    // Timer linked list root
    struct stimer *                     root;


    // Timer math
    struct tm_math                      tm;
    uint32_t                            ns_per_count;


    // Time function
    stimer_get_time_fn                  get_time_fn;
    void *                              hint;
};


// ----------------------- Timer handle
struct stimer {
    // Context
    struct stimer_ctx *                 ctx;


    // Linked list
    struct stimer *                     next;


    // Last get_time_fn checkpoint
    uint32_t                            checkpoint;


    // Expire period
    struct stimer_duration              expire_interval;


    // Elapsed time
    struct stimer_duration              elapsed;
    bool                                is_running;
};


// -------------------------------------------------------------- Timer context




/**
 * @brief Allocates a timer context structure on the heap
 *
 * @param hint Optional hint parameter for the get_time_fn function. The
 *          get_time_fn will always be called with this parameter. If unused,
 *          set to NULL
 * @param get_time_fn Get time function pointer
 * @param max_time Maximum value that can be returned by the get_time_fn
 * @param ns_per_count Nanoseconds per get_time_fn tick
 * @return Timer context, or NULL on an error
 */
struct stimer_ctx *
stimer_alloc_context(void * hint,
                     stimer_get_time_fn get_time_fn,
                     uint32_t max_time,
                     uint32_t ns_per_count);

/**
 * @brief initializes a preallocated a timer context structure
 *
 * @param hint Optional hint parameter for the get_time_fn function. The
 *          get_time_fn will always be called with this parameter. If unused,
 *          set to NULL
 * @param get_time_fn Get time function pointer
 * @param max_time Maximum value that can be returned by the get_time_fn
 * @param ns_per_count Nanoseconds per get_time_fn tick
 * @return Timer context, or NULL on an error
 */
void
stimer_init_context(struct stimer_ctx *ctx,
                    void * hint,
                    stimer_get_time_fn get_time_fn,
                    uint32_t max_time,
                    uint32_t ns_per_count);

/**
 * @brief Deallocates a timer context
 *
 * @param ctx Timer context to free
 */
void
stimer_free_context(struct stimer_ctx * ctx);


/**
 * @brief Periodic call to drive all of the timers
 * @details This must be called periodically to increment the timers. This must
 *          be called at a rate at least 4 times faster than the get_time_fn
 *          value rollover. Optionally, this can be skipped if you know that
 *          all timers in use are periodically checked at least 4 times faster
 *          the get_time_fn value rollover
 *
 * @param ctx Timer context to execute
 */
void
stimer_execute_context(struct stimer_ctx * ctx);


// --------------------------------------------------------------- Timer handle

/**
 * @brief Allocates a timer handle on the heap
 *
 * @param ctx Timer context to source the timer handle from
 * @return Timer handle
 */
struct stimer *
stimer_alloc(struct stimer_ctx * ctx);

/**
 * @brief initialize a preallocated a timer handle
 *
 * @param ts Pointer to timer handle
 * @param ctx Timer context to source the timer handle from
 * @return Timer handle
 */
void
stimer_init(struct stimer *ts, struct stimer_ctx * ctx);

/**
 * @brief Deallocates a timer handle
 *
 * @param ts Timer handle to unlink and deallocate
 */
void
stimer_free(struct stimer * ts);

/**
 * @brief Unlink a timer handle
 *
 * @param ts Timer handle to unlink
 */
void
stimer_remove(struct stimer * ts);

// ---------------------------------------------------- Elapsed timer functions

/**
 * @brief Starts the timer to measuring an elapsed time
 * @details This will reset any accumulated elapsed time in the timer
 *
 * @param ts Timer handle
 */
void
stimer_start(struct stimer * ts);


/**
 * @brief Stops the accumulation of time in the timer
 *
 * @param ts Timer handle to stop
 */
void
stimer_stop(struct stimer * ts);


/**
 * @brief Gets the amounts of time elapsed on a timer
 * @details This can be called on a timer previously started with the
 *          stimer_start function or any of the stimer_expire_from_now_*
 *          functions. The timer does not need to be stopped first before
 *          calling this
 *
 * @param ts Timer handle
 * @param t Timer duration structure to put elapsed time into
 */
void
stimer_get_elapsed_time(struct stimer * ts, struct stimer_duration * t);


// ----------------------------------------------------- Expire timer functions

/**
 * @brief Sets the timer up to expire at a point in time from now
 *
 * @param ts Timer handle
 * @param t Time duration until expiration
 */
void
stimer_expire_from_now(struct stimer * ts, struct stimer_duration * t);


/**
 * @brief Sets the timer up to expire at a point in time from now
 *
 * @param ts Timer handle
 * @param s Seconds until expiration
 */
void
stimer_expire_from_now_s(struct stimer * ts, uint32_t s);


/**
 * @brief Sets the timer up to expire at a point in time from now
 *
 * @param ts Timer handle
 * @param ms Milliseconds until expiration
 */
void
stimer_expire_from_now_ms(struct stimer * ts, uint32_t ms);


/**
 * @brief Sets the timer up to expire at a point in time from now
 *
 * @param ts Timer handle
 * @param us Microseconds until expiration
 */
void
stimer_expire_from_now_us(struct stimer * ts, uint32_t us);


/**
 * @brief Sets the timer up to expire at a point in time from now
 *
 * @param ts Timer handle
 * @param ns Nanoseconds until expiration
 */
void
stimer_expire_from_now_ns(struct stimer * ts, uint32_t ns);


/**
 * @brief Checks if a timer has expired
 *
 * @param ts Timer handle
 * @return true if the timer has expired, else false
 */
bool
stimer_is_expired(struct stimer * ts);


/**
 * @brief Restarts a timer to expire at a point in the future from now.
 * @details This reuses the expiration duration previously set with one of the
 *          stimer_expire_from_now_* functions.
 *
 * @param ts TImer handle
 */
void
stimer_restart_from_now(struct stimer * ts);


/**
 * @brief Advances a timer to expire relative to its previous expiration time
 * @details This is similar to the stimer_restart_from_now function, except
 *          that it reloads the timer from the exact point that the timer
 *          previously expired at. This can be used to create a steady timer.
 *          This reuses the expiration duration previously set with one of the
 *          stimer_expire_from_now_* functions.
 *
 * @param ts Timer handle
 */
void
stimer_advance(struct stimer * ts);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STIMER_H_ */

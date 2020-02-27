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

#ifndef TIMERMATH_H_
#define TIMERMATH_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * This header implements math functions for dealing with continuous timers.
 * Having timers that roll over to 0 after a reachable finite amount of time is
 * quite common in embedded systems. This header implements the essential
 * functions for comparing timer values, taking the difference between timer
 * values, and incrementing / decrementing timer values.
 *
 * Most of the math here can be thought of as operating on a circle of values.
 * Time is assumed to increment from 0 to a maximum value before returning to
 * 0.
 *
 * For example, if a timer with a maximum value of 0x07 is used:
 *
 *     0
 *  7  |  1
 *    \ /
 * 6 -   - 2
 *    / \
 *  5  |  3
 *     4
 *
 * struct tm_math tm;
 * tm_initialize(&tm, 0x07);
 *
 * tm_get_diff(&tm, 0, 7);  // (0 - 7) == 1
 * tm_get_diff(&tm, 4, 6);  // (4 - 6) == -2
 * tm_is_gt(&tm, 0, 7);     // (0 > 7) == true
 * tm_is_le(&tm, 2, 5);     // (2 <= 5) == true
 *
 */

// ---------------------------------------------------------------------- Types

typedef bool (*tm_eq_fn)(uint32_t lhs, uint32_t rhs, uint32_t max_value);
typedef bool (*tm_lt_fn)(uint32_t lhs, uint32_t rhs, uint32_t max_value);
typedef int32_t (*tm_diff_fn)(uint32_t lhs, uint32_t rhs, uint32_t max_value);
typedef uint32_t (*tm_offset_fn)(uint32_t t, int32_t offset, uint32_t max_value);

struct tm_math {
    tm_eq_fn eq;
    tm_lt_fn lt;
    tm_diff_fn diff;
    tm_offset_fn offset;
    uint32_t max_value;
};


// -------------------------------------------------------- Top level functions

/**
 * @brief Initializes a timer math structure
 * @details This must be called before the struct can be used. This will
 *          determine which math functions can be used for the given maximum
 *          timer value. The most efficient implementation uses bit masking, so
 *          maximum values that coincide with (2**n - 1) should be preferred.
 *          If the maximum timer value does not coincide with an even bit
 *          boundary, then the slower math functions will be used.
 *          Note that strange behavior may happen when comparing value at
 *          exactly ceil(max_value / 2), since that is where (-0) exists.
 *
 * @param tm_math Structure to initialize
 * @param max_value Maximum value that the timer (counter) can hold before
 *          rolling back to 0
 */
static inline void
tm_initialize(struct tm_math * tm, uint32_t max_value);

/**
 * @brief Check if (lhs == rhs)
 *
 * @param tm_math Math structure
 * @param lhs left had side of the operator
 * @param rhs right hand side of the operator
 */
static inline bool
tm_is_eq(struct tm_math * tm, uint32_t lhs, uint32_t rhs);

/**
 * @brief Check if (lhs < rhs)
 *
 * @param tm_math Math structure
 * @param lhs left had side of the operator
 * @param rhs right hand side of the operator
 */
static inline bool
tm_is_lt(struct tm_math * tm, uint32_t lhs, uint32_t rhs);

/**
 * @brief Check if (lhs > rhs)
 *
 * @param tm_math Math structure
 * @param lhs left had side of the operator
 * @param rhs right hand side of the operator
 */
static inline bool
tm_is_gt(struct tm_math * tm, uint32_t lhs, uint32_t rhs);

/**
 * @brief Check if (lhs >= rhs)
 *
 * @param tm_math Math structure
 * @param lhs left had side of the operator
 * @param rhs right hand side of the operator
 */
static inline bool
tm_is_ge(struct tm_math * tm, uint32_t lhs, uint32_t rhs);

/**
 * @brief Check if (lhs <= rhs)
 *
 * @param tm_math Math structure
 * @param lhs left had side of the operator
 * @param rhs right hand side of the operator
 */
static inline bool
tm_is_le(struct tm_math * tm, uint32_t lhs, uint32_t rhs);

/**
 * @brief Gets the difference between two timer values, performing
 * @details This performs (lhs - rhs) with roll over math
 *
 * @param tm_math Math structure
 * @param lhs left had side of the operator
 * @param rhs right hand side of the operator
 */
static inline int32_t
tm_get_diff(struct tm_math * tm, uint32_t lhs, uint32_t rhs);

/**
 * @brief Offsets the given timer value by a positive or negative offset
 * @details This performs (t + offset) with roll over math
 *
 * @param tm_math Math structure
 * @param t Timer value
 * @param offset Positive or negative offset to apply
 */
static inline uint32_t
tm_offset(struct tm_math * tm, uint32_t t, int32_t offset);


// ------------------------------------------------------------- Implementation


// --------------------- Mask operators

static inline bool
_tm_mask_lt(uint32_t lhs, uint32_t rhs, uint32_t max_value)
{
    return (((rhs - lhs) - 1) & max_value) < ((max_value >> 1) + 1);
}

static inline bool
_tm_mask_eq(uint32_t lhs, uint32_t rhs, uint32_t max_value)
{
    return (lhs & max_value) == (rhs & max_value);
}

static inline int32_t
_tm_mask_diff(uint32_t lhs, uint32_t rhs, uint32_t max_value)
{
    uint32_t half = (max_value >> 1) + 1;
    uint32_t diff = (rhs - lhs) & max_value;

    if (diff > half) {
        diff = (lhs - rhs) & max_value;
    }

    if(_tm_mask_lt(lhs, rhs, max_value)) {
        return diff * -1;
    } else {
        return diff;
    }
}

static inline uint32_t
_tm_mask_offset(uint32_t t, int32_t offset, uint32_t max_value)
{
    return (t  + offset) & max_value;
}


// ----------------- Non-mask operators

static inline bool
_tm_nm_lt(uint32_t lhs, uint32_t rhs, uint32_t max_value)
{
    uint32_t distance_abs = 0;
    if (lhs > rhs) {
        distance_abs = lhs - rhs;
    } else {
        distance_abs = rhs - lhs;
    }

    bool lt =  false;
    uint32_t half = (max_value >> 1) + 1;
    if (distance_abs > half) {
        lt = lhs > rhs;
    } else {
        lt = lhs < rhs;
    }

    return lt;
}

static inline bool
_tm_nm_eq(uint32_t lhs, uint32_t rhs, uint32_t max_value)
{
    if (lhs > max_value) {
        lhs = lhs % (max_value + 1);
    }
    if(rhs > max_value) {
        rhs = rhs % (max_value + 1);
    }

    return lhs == rhs;
}

static inline int32_t
_tm_nm_diff(uint32_t lhs, uint32_t rhs, uint32_t max_value)
{
    uint32_t distance_abs = 0;
    if (lhs > rhs) {
        distance_abs = lhs - rhs;
    } else {
        distance_abs = rhs - lhs;
    }

    int32_t diff = 0;
    uint32_t half = (max_value >> 1) + 1;
    if (distance_abs < half) {
        if (lhs >= rhs) {
            diff = lhs - rhs;
        } else {
            diff = (rhs - lhs) * -1;
        }
    } else {
        if (lhs > rhs) {
            diff = -1 * (rhs + (max_value - lhs) + 1);
        } else {
            diff = lhs + (max_value - rhs) + 1;
        }
    }

    return diff;
}

static inline uint32_t
_tm_nm_offset(uint32_t t, int32_t offset, uint32_t max_value)
{
    if (offset > 0) {
        int32_t distance = max_value - t;
        if (offset > distance) {
            t = (offset - distance) - 1;
        } else {
            t = t + offset;
        }
    } else if (offset < 0) {
        uint32_t offset_abs = offset * -1;
        if (offset_abs > t) {
            t = (max_value - (offset_abs - t)) + 1;
        } else {
            t += offset;
        }
    }
    return t;
}


// ---------------- Top level functions

static inline void
tm_initialize(struct tm_math * tm, uint32_t max_value)
{
    tm->max_value = max_value;

    uint8_t ones = 0;
    max_value += 1;
    while(max_value > 0) {
        ones += (max_value & 0x01);
        max_value >>= 1;
    }

    if(ones < 2) {
        tm->eq = _tm_mask_eq;
        tm->lt = _tm_mask_lt;
        tm->diff = _tm_mask_diff;
        tm->offset = _tm_mask_offset;
    } else {
        tm->eq = _tm_nm_eq;
        tm->lt = _tm_nm_lt;
        tm->diff = _tm_nm_diff;
        tm->offset = _tm_nm_offset;
    }
}

static inline bool
tm_is_eq(struct tm_math * tm, uint32_t lhs, uint32_t rhs)
{
    return tm->eq(lhs, rhs, tm->max_value);
}

static inline bool
tm_is_lt(struct tm_math * tm, uint32_t lhs, uint32_t rhs)
{
    return tm->lt(lhs, rhs, tm->max_value);
}

static inline bool
tm_is_gt(struct tm_math * tm, uint32_t lhs, uint32_t rhs)
{
    return tm->lt(rhs, lhs, tm->max_value);
}

static inline bool
tm_is_ge(struct tm_math * tm, uint32_t lhs, uint32_t rhs)
{
    return !tm->lt(lhs, rhs, tm->max_value);
}

static inline bool
tm_is_le(struct tm_math * tm, uint32_t lhs, uint32_t rhs)
{
    return tm->eq(lhs, rhs, tm->max_value) || tm->lt(lhs, rhs, tm->max_value);
}

static inline int32_t
tm_get_diff(struct tm_math * tm, uint32_t lhs, uint32_t rhs)
{
    return tm->diff(lhs, rhs, tm->max_value);
}

static inline uint32_t
tm_offset(struct tm_math * tm, uint32_t t, int32_t offset)
{
    return tm->offset(t, offset, tm->max_value);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TIMERMATH_H_ */

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
// TODO: Automatically detect trace method based on selected RTE components.
//-----------------------------------------------------------------------------
#ifndef APP_TRACE_H_
#define APP_TRACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef APP_TRACE_DISABLED

#include <stdio.h>

#define TRACE_PRINTF(...) printf(__VA_ARGS__)
#define TRACE_VPRINTF(fmt, va_args) vprintf(fmt, va_args)

#else

#define TRACE_PRINTF(...)
#define TRACE_VPRINTF(fmt, va_args)

#endif

#define ASSERT(msg)                                     \
    do                                                  \
    {                                                   \
        if (!(msg))                                     \
        {                                               \
            assert_error(__FILE__, __LINE__, #msg);   \
        }                                               \
    } while (0)


extern void trace_init(void);

extern void trace_deinit(void);

extern void assert_error(const char *file, const int line, const char *msg);


#ifdef __cplusplus
}
#endif

#endif /* APP_TRACE_H_ */

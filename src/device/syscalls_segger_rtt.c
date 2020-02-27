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

#include "SEGGER_RTT.h"

enum {
    STDIN_FILENO,
    STDOUT_FILENO,
    STDERR_FILENO
};

int __attribute__((weak)) _app_write_hook(int file, char* ptr, int len)
{
    return len;
}

//int _read(int file, char* ptr, int len)
//{
//
//}

int _write(int file, char* ptr, int len)
{
    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        return SEGGER_RTT_Write(0, ptr,len);
    }
    else
    {
        return _app_write_hook(file, ptr, len);
    }
}


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
#include <stdio.h>

#include "app.h"


void App_PeerDeviceConnected(void)
{
    TRACE_PRINTF("PEER DEVICE CONNECTED\r\n");

    app_state = APP_STATE_CONNECTED;
}

void App_PeerDeviceDisconnected(void)
{
    TRACE_PRINTF("PEER DEVICE DISCONNECTED\r\n");

    CS_SetPowerMode(CS_POWER_MODE_SLEEP);

    app_state = APP_STATE_START_ADVERTISING;
}

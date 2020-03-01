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
//! \file HAL_clock.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//! \addtogroup CLK_GRP
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <HAL.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void HAL_SetClockPrescalers(const enum HAL_ClockConfiguration clk)
{
    uint32_t rf_clk_prescale;
    uint32_t slowclk_prescale;
    uint32_t bbclk_prescale;
    uint32_t bbclk_div_prescale;
    uint32_t power_clk_prescale;

    switch (clk)
    {
    case HAL_CLK_CONF_48MHZ:
        rf_clk_prescale = CK_DIV_1_6_PRESCALE_1_BYTE; /* 48MHz */
        slowclk_prescale = SLOWCLK_PRESCALE_48; /* 1MHz */
        bbclk_prescale = BBCLK_PRESCALE_6; /* 8MHz */
        bbclk_div_prescale = BBCLK_DIVIDER_8; /* 1MHz */
        power_clk_prescale = DCCLK_PRESCALE_6 | CPCLK_PRESCALE_8; /* 8MHz ; 125kHz */
        break;

    case HAL_CLK_CONF_24MHZ:
        rf_clk_prescale = CK_DIV_1_6_PRESCALE_2_BYTE; /* 24MHz */
        slowclk_prescale = SLOWCLK_PRESCALE_24; /* 1MHz */
        bbclk_prescale = BBCLK_PRESCALE_3; /* 8MHz */
        bbclk_div_prescale = BBCLK_DIVIDER_8; /* 1MHz */
        power_clk_prescale = DCCLK_PRESCALE_3 | CPCLK_PRESCALE_8; /* 8MHz ; 125kHz */
        break;

    case HAL_CLK_CONF_16MHZ:
        rf_clk_prescale = CK_DIV_1_6_PRESCALE_3_BYTE; /* 16MHz */
        slowclk_prescale = SLOWCLK_PRESCALE_16; /* 1MHz */
        bbclk_prescale = BBCLK_PRESCALE_2; /* 8MHz */
        bbclk_div_prescale = BBCLK_DIVIDER_8; /* 1MHz */
        power_clk_prescale = DCCLK_PRESCALE_2 | CPCLK_PRESCALE_8; /* 8MHz ; 125kHz */
        break;

    default:
    case HAL_CLK_CONF_8MHZ:
        rf_clk_prescale = CK_DIV_1_6_PRESCALE_6_BYTE; /* 8MHz */
        slowclk_prescale = SLOWCLK_PRESCALE_8; /* 1MHz */
        bbclk_prescale = BBCLK_PRESCALE_1; /* 8MHz */
        bbclk_div_prescale = BBCLK_DIVIDER_8; /* 1MHz */
        power_clk_prescale = DCCLK_PRESCALE_2 | CPCLK_PRESCALE_8; /* 4MHz ; 125kHz */
        break;
    }

    RF_REG2F->CK_DIV_1_6_CK_DIV_1_6_BYTE = rf_clk_prescale;

    /* Wait until 48 MHz oscillator is started */
    while (RF_REG39->ANALOG_INFO_CLK_DIG_READY_ALIAS
            != ANALOG_INFO_CLK_DIG_READY_BITBAND)
        ;

    /* Switch to RFCLK as source for SYSCLK */
    Sys_Clocks_SystemClkConfig(
            SYSCLK_CLKSRC_RFCLK | EXTCLK_PRESCALE_1 | JTCK_PRESCALE_1);

    Sys_Clocks_SystemClkPrescale0(
            slowclk_prescale | bbclk_prescale | USRCLK_PRESCALE_1);

    Sys_Clocks_SystemClkPrescale2(power_clk_prescale);

    BBIF->CTRL = BB_CLK_ENABLE | BB_DEEP_SLEEP | bbclk_div_prescale;

    // Prescale1 is configured on peripheral level.
}

//! \}
//! \}

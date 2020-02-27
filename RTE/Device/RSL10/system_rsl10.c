/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * system_rsl10.c
 * - CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *
 *   Provides the required system_<Device>.c implementation for CMSIS
 *   compatibility
 * ----------------------------------------------------------------------------
 * $Revision: 1.21 $
 * $Date: 2018/10/26 15:54:06 $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include "rsl10.h"
#include <rsl10_flash.h>

/* ----------------------------------------------------------------------------
 * Variables
 * ------------------------------------------------------------------------- */
/* Required system clock frequency variable (Cortex-M3 clock frequency) */
#if defined ( __CC_ARM )
uint32_t SystemCoreClock __attribute__((section(".systemclock"), zero_init));
#elif defined ( __ICCARM__ )
#pragma location=".systemclock"
 __root uint32_t SystemCoreClock;
#else
uint32_t SystemCoreClock __attribute__((section(".systemclock")));
#endif /* #if defined ( __CC_ARM ) */

/* ----------------------------------------------------------------------------
 * CMSIS Internal Functions
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * IFunction     : unsigned int System_GetTargetForTrim(uint32_t *calib_info_base,
 *                                                      uint8_t trim,
 *                                                      uint16_t *target)
 * ----------------------------------------------------------------------------
 * Description   : Search for the target frequency corresponding to the
 *                 specified trim in NVR4.
 * Inputs        : - calib_info_base   - The base register for the specified
 *                                       calibration information.
 *                 - trim              - the trim value to be searched for
 *                 - target            - A pointer to the variable that will
 *                                       hold the target frequency, if found.
 * Outputs       : - return value      - A code indicating whether an error has
 *                                       occurred.
 *                 - target            - The target frequency corresponding to
 *                                       the specified trim value, if found.
 * Assumptions   : Flash is assumed to be enabled.
 * ------------------------------------------------------------------------- */
unsigned int System_GetTargetForTrim(uint32_t *calib_info_ptr,
                                     uint8_t trim,
                                     uint16_t *target)
{
    uint8_t count;

    unsigned int data[4];
    unsigned int result;

    /* Read calibration data from NVR4. We are passing in 4 because that is the
     * maximum number of calibration targets available. */
    result = Sys_ReadNVR4((unsigned int)calib_info_ptr, 4, data);
    if (result != ERRNO_NO_ERROR) { return result; }

    for (count = 0; count < 4; count++)
    {
        if ((uint8_t)data[count] == trim)
        {
            *target = (uint16_t)(data[count] >> MANU_CAL_INFO_TARGET_POS);
            return ERRNO_NO_ERROR;
        }
    }

    return ERRNO_GENERAL_FAILURE;
}

/* ----------------------------------------------------------------------------
 * CMSIS Required and Recommended Functions
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Function      : SystemInit
 * ----------------------------------------------------------------------------
 * Description   : Setup the system core clock variable; assumes the ROM has
 *                 previously initialized the system.
 * Inputs        : None
 * Outputs       : None
 * Register mods : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void SystemInit(void)
{
    if ((SystemCoreClock < 25000) || (SystemCoreClock > 48000000))
    {
        SystemCoreClock = DEFAULT_FREQ;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void SystemCoreClockUpdate(void)
 * ----------------------------------------------------------------------------
 * Description   : Updates the variable SystemCoreClock and the
 *                 FLASH_DELAY_CTRL register. This function must be called 
 *                 whenever the core clock is changed during program execution.
 * Inputs        : None
 * Outputs       : None
 * Register mods : None
 * Assumptions   : - It is safe to treat undefined clock configurations
 *                   as if they are sourced from the RC oscillator.
 *                 - EXTCLK and JTCK should be scaled from their maximum
 *                   frequencies.
 *                 - It is safe to assume a STANDBYCLK frequency of 32768 Hz
 * ------------------------------------------------------------------------- */
void SystemCoreClockUpdate(void)
{
    uint32_t temp = CLK->SYS_CFG;
    uint32_t flash_delay;
    uint16_t target;
    uint8_t trim;
    unsigned int result;

    switch (temp & CLK_SYS_CFG_SYSCLK_SRC_SEL_Mask)
    {

    /* SYSCLK is sourced from the standby oscillator or 32 kHz crystal.
     * We assume 32768 is close enough in this case, since there isn't
     * significant variation for this value. */
    case SYSCLK_CLKSRC_STANDBYCLK:
        SystemCoreClock = STANDBYCLK_DEFAULT_FREQ;
        break;

    /* SYSCLK is sourced from a divided form of the RF crystal oscillator */
    case SYSCLK_CLKSRC_RFCLK:
        SystemCoreClock = RFCLK_BASE_FREQ / (RF_REG2F->CK_DIV_1_6_CK_DIV_1_6_BYTE);
        break;

    /* SYSCLK is sourced from a divided form of the EXTCLK signal */
    case SYSCLK_CLKSRC_EXTCLK:
        SystemCoreClock = EXTCLK_MAX_FREQ /
                          (CLK_SYS_CFG->EXTCLK_PRESCALE_BYTE + 1);
        break;

    /* SYSCLK is sourced from a divided form of the JTAG JTCK signal */
    case SYSCLK_CLKSRC_JTCK:
        SystemCoreClock = JTCK_MAX_FREQ /
                          (CLK_SYS_CFG->JTCK_PRESCALE_BYTE + 1);
        break;

    /* If SYSCLK is sourced from the RC oscillator, check the multiplier
     * control bit to see if we're running at 3 MHz or 12 MHz. Assume the RC
     * oscillator is used if the source is undefined.
     */
    case SYSCLK_CLKSRC_RCCLK:
    default:
        trim = (uint8_t)((ACS->RCOSC_CTRL & ACS_RCOSC_CTRL_FTRIM_START_Mask)
                        >> ACS_RCOSC_CTRL_FTRIM_START_Pos);
        
        if (ACS_RCOSC_CTRL->CLOCK_MULT_ALIAS == RC_START_OSC_12MHZ_BITBAND)
        {
            result = System_GetTargetForTrim((uint32_t *)MANU_INFO_OSC_RC_MULT,
                                            trim, &target);

            if (result == ERRNO_NO_ERROR)
            {
                /* The returned target is multiplied by 1000 because the
                 * frequency stored in NVR4 is in kHz whereas the one in
                 * SystemCoreClock is in Hz. */
                SystemCoreClock = target*1000;
            }
            else
            {
                SystemCoreClock = RCOSC_MAX_FREQ;
            }
        }
        else
        {
            result = System_GetTargetForTrim((uint32_t *)MANU_INFO_OSC_RC,
                                trim, &target);

            if (result == ERRNO_NO_ERROR)
            {
                /* The returned target is multiplied by 1000 because the
                 * frequency stored in NVR4 is in kHz whereas the one in
                 * SystemCoreClock is in Hz. */
                SystemCoreClock = target*1000;
            }
            else
            {
                SystemCoreClock = DEFAULT_FREQ;
            }
        }
    }

    /* Configure the flash delays required for proper flash operation
       and set the SLOWCLK prescaler to generate a 1 MHz clock */
    if (SystemCoreClock <= 1000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 0;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_3MHZ;
    }
    else if (SystemCoreClock <= 2000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 1;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_3MHZ;
    }
    else if (SystemCoreClock <= 3000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 2;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_3MHZ;
    }
    else if (SystemCoreClock <= 5000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 4;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_5MHZ;
    }
    else if (SystemCoreClock <= 8000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 7;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_8MHZ;
    }
    else if (SystemCoreClock <= 10000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 9;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_12MHZ;
    }
    else if (SystemCoreClock <= 12000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 11;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_12MHZ;
    }
    else if (SystemCoreClock <= 16000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 15;

        if((temp & CLK_SYS_CFG_SYSCLK_SRC_SEL_Mask) == SYSCLK_CLKSRC_RCCLK)
        {
            flash_delay = FLASH_DELAY_FOR_SYSCLK_20MHZ;
        }
        else
        {
            flash_delay = FLASH_DELAY_FOR_SYSCLK_16MHZ;
        }
    }
    else if (SystemCoreClock <= 20000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 19;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_20MHZ;
    }
    else if (SystemCoreClock <= 24000000)
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 23;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_24MHZ;
    }
    else
    {
        CLK_DIV_CFG0->SLOWCLK_PRESCALE_BYTE = 47;
        flash_delay = FLASH_DELAY_FOR_SYSCLK_48MHZ;
    }

    FLASH->DELAY_CTRL = (FLASH->DELAY_CTRL & ~FLASH_DELAY_CTRL_SYSCLK_FREQ_Mask) |
                        flash_delay;

    return;
}


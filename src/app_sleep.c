/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 * This module is derived in part from example code provided by RivieraWaves
 * and as such the underlying code is the property of RivieraWaves [a member
 * of the CEVA, Inc. group of companies], together with additional code which
 * is the property of ON Semiconductor. The code (in whole or any part) may not
 * be redistributed in any form without prior written permission from
 * ON Semiconductor.
 *
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ------------------------------------------------------------------------- */

#include "app.h"


/* ----------------------------------------------------------------------------
 * Function      : void Sleep_Mode_Configure(
 *                         struct sleep_mode_env_tag *sleep_mode_env)
 * ----------------------------------------------------------------------------
 * Description   : Configure the sleep mode
 * Inputs        : Pre-defined parameters and configurations
 *                 for the sleep mode
 * Outputs       : sleep_mode_env   - Parameters and configurations
 *                                    for the sleep mode
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sleep_Mode_Configure(struct sleep_mode_env_tag *sleep_env)
{
    struct sleep_mode_init_env_tag sleep_mode_init_env;

    /* Set the clock source for RTC */
    sleep_mode_init_env.rtc_ctrl = RTC_CLK_SRC;

    /* if RTC clock source is XTAL 32 kHz oscillator */
//    if (RTC_CLK_SRC == RTC_CLK_SRC_XTAL32K)
//    {
        /* Enable XTAL32K oscillator amplitude control
         * Set XTAL32K load capacitance to 0x38: 22.4 pF
         * Enable XTAL32K oscillator */
        ACS->XTAL32K_CTRL = \
            (XTAL32K_XIN_CAP_BYPASS_DISABLE                                |
             XTAL32K_AMPL_CTRL_ENABLE                                      |
             XTAL32K_NOT_FORCE_READY                                       |
             (XTAL32K_CLOAD_TRIM_VALUE << ACS_XTAL32K_CTRL_CLOAD_TRIM_Pos) |
             (XTAL32K_ITRIM_VALUE << ACS_XTAL32K_CTRL_ITRIM_Pos)           |
             XTAL32K_IBOOST_DISABLE                                        |
             XTAL32K_ENABLE);

        /* Wait for XTAL32K oscillator to be ready */
        while (ACS_XTAL32K_CTRL->READY_ALIAS != XTAL32K_OK_BITBAND);

        LowPowerClock_Source_Set(0);
//    }

    /* Set delay and wake-up sources, use
     *    WAKEUP_DELAY_[ 1 | 2 | 4 | ... | 128],
     *    WAKEUP_DCDC_OVERLOAD_[ENABLE | DISABLE],
     *    WAKEUP_WAKEUP_PAD_[RISING | FALLING],
     *    WAKEUP_DIO*_[RISING | FALLING],
     *    WAKEUP_DIO*_[ENABLE | DISABLE] */
    sleep_mode_init_env.wakeup_cfg = WAKEUP_DELAY_64          |
                                     WAKEUP_WAKEUP_PAD_RISING |
                                     WAKEUP_DIO3_DISABLE      |
                                     WAKEUP_DIO2_DISABLE      |
                                     WAKEUP_DIO1_DISABLE      |
                                     WAKEUP_DIO0_DISABLE;

    /* Set wake-up control/status registers, use
     *    PADS_RETENTION_[ENABLE | DISABLE],
     *    BOOT_FLASH_APP_REBOOT_[ENABLE | DISABLE],
     *    BOOT_[CUSTOM | FLASH_XTAL_*],
     *    WAKEUP_DCDC_OVERLOAD_CLEAR,
     *    WAKEUP_PAD_EVENT_CLEAR,
     *    WAKEUP_RTC_ALARM_CLEAR,
     *    WAKEUP_BB_TIMER_CLEAR,
     *    WAKEUP_DIO3_EVENT_CLEAR,
     *    WAKEUP_DIO2_EVENT_CLEAR,
     *    WAKEUP_DIO1_EVENT_CLEAR],
     *    WAKEUP_DIO0_EVENT_CLEAR */
    sleep_env->wakeup_ctrl = PADS_RETENTION_ENABLE         |
                                  BOOT_FLASH_APP_REBOOT_DISABLE |
                                  BOOT_CUSTOM                   |
                                  WAKEUP_DCDC_OVERLOAD_CLEAR    |
                                  WAKEUP_PAD_EVENT_CLEAR        |
                                  WAKEUP_RTC_ALARM_CLEAR        |
                                  WAKEUP_BB_TIMER_CLEAR         |
                                  WAKEUP_DIO3_EVENT_CLEAR       |
                                  WAKEUP_DIO2_EVENT_CLEAR       |
                                  WAKEUP_DIO1_EVENT_CLEAR       |
                                  WAKEUP_DIO0_EVENT_CLEAR;

    /* Set wake-up application start address (LSB must be set) */
    sleep_mode_init_env.app_addr =
        (uint32_t)(&Wakeup_From_Sleep_Application_asm) | 1;

    /* Set wake-up restore address */
    sleep_mode_init_env.wakeup_addr = (uint32_t)(DRAM2_TOP + 1 -
                                                 POWER_MODE_WAKEUP_INFO_SIZE);

    /* Configure memory retention */
    sleep_env->mem_power_cfg = (
            DSP_DRAM0_POWER_ENABLE |
//            DSP_DRAM1_POWER_ENABLE |
//            DSP_DRAM2_POWER_ENABLE |
//            DSP_DRAM3_POWER_ENABLE |
//            DSP_DRAM4_POWER_ENABLE |
//            DSP_DRAM5_POWER_ENABLE |
            DRAM0_POWER_ENABLE |
            DRAM1_POWER_ENABLE |
            DRAM2_POWER_ENABLE |
            BB_DRAM0_POWER_ENABLE |
            BB_DRAM1_POWER_ENABLE |
            FLASH_POWER_ENABLE
            );

    /* Configure memory at wake-up (PROM must be part of this) */
    sleep_mode_init_env.mem_power_cfg_wakeup = (
            PROM_POWER_ENABLE |
            DSP_DRAM0_POWER_ENABLE |
//            DSP_DRAM1_POWER_ENABLE |
//            DSP_DRAM2_POWER_ENABLE |
//            DSP_DRAM3_POWER_ENABLE |
//            DSP_DRAM4_POWER_ENABLE |
//            DSP_DRAM5_POWER_ENABLE |
            DRAM0_POWER_ENABLE |
            DRAM1_POWER_ENABLE |
            DRAM2_POWER_ENABLE |
            BB_DRAM0_POWER_ENABLE |
            BB_DRAM1_POWER_ENABLE
            );

    /* Set DMA channel used to save/restore RF registers
     * in each sleep/wake-up cycle */
    sleep_mode_init_env.DMA_channel_RF = DMA_CHAN_SLP_WK_RF_REGS_COPY;

    /* Perform initializations required for sleep mode */
    Sys_PowerModes_Sleep_Init_2Mbps(&sleep_mode_init_env);
}

/* ----------------------------------------------------------------------------
 * Function      : void Wakeup_From_Sleep_Application(void)
 * ----------------------------------------------------------------------------
 * Description   : Restore system states from retention RAM and continue
 *                 application from flash
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Wakeup_From_Sleep_Application(void)
{
    Sys_PowerModes_Wakeup_2Mbps();

    /* The system is awake from this point, continue application from flash */
//    Continue_Application();
    Device_Initialize_WakeUp();
}

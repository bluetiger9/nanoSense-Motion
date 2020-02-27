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
//! \file HAL.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <HAL.h>
#include <calibration.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

/** \brief HAL uses TIMER0 to keep track of time with 1 ms precision. */
void TIMER0_IRQHandler(void);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

/** \brief Internal variable for keeping time.
 *
 * It is 32-bit unsigned type incremented every milli second and therefore will
 * overflow every ~49 days.
 */
volatile uint32_t hal_time_counter = 0;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void HAL_Init(enum HAL_ClockConfiguration clk_conf)
{
    /* Mask all interrupts */
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);

    /* Disable all interrupts and clear any pending interrupts */
    Sys_NVIC_DisableAllInt();
    Sys_NVIC_ClearAllPendingInt();

    /* Test recovery DIO (DIO12) to pause the program to make it easy to
     * re-flash after power up.
     */
    DIO->CFG[PIN_RECOVERY] = DIO_MODE_INPUT | DIO_WEAK_PULL_UP | DIO_LPF_DISABLE
            | DIO_6X_DRIVE;
    while (DIO_DATA->ALIAS[PIN_RECOVERY] == 0);
    DIO->CFG[PIN_RECOVERY] = DIO_MODE_DISABLE | DIO_NO_PULL;

    /* Calibrate the board
     * The supplemental calibrated values are loaded by the user-defined
     * initialization function during the system boot process for supplemental mode.*/
    if (Load_Trim_Values_And_Calibrate_MANU_CALIB() != VOLTAGES_CALIB_NO_ERROR)
    {
        /* Hold here to notify error(s) in voltage calibrations */
        while (true)
        {
            Sys_Watchdog_Refresh();
        }
    }

    /* Configure the current trim settings for VCC, VDDA */
    ACS_VCC_CTRL->ICH_TRIM_BYTE = VCC_ICHTRIM_80MA_BYTE;
////    ACS_VCC_CTRL->ICH_TRIM_BYTE = VCC_ICHTRIM_16MA_BYTE;
////    ACS_VDDA_CP_CTRL->PTRIM_BYTE = VDDA_PTRIM_16MA_BYTE;

    /* Start 48 MHz XTAL oscillator */
    ACS_VDDRF_CTRL->ENABLE_ALIAS = VDDRF_ENABLE_BITBAND;
    ACS_VDDRF_CTRL->CLAMP_ALIAS = VDDRF_DISABLE_HIZ_BITBAND;

    /* Wait until VDDRF supply has powered up */
    while (ACS_VDDRF_CTRL->READY_ALIAS != VDDRF_READY_BITBAND)
        ;

    /* Enable/disable buck converter */
    ACS_VCC_CTRL->BUCK_ENABLE_ALIAS = VCC_BUCK_BITBAND;

//    /* Disable RF TX power amplifier supply voltage and
//     * connect the switched output to VDDRF regulator */
//    ACS_VDDPA_CTRL->ENABLE_ALIAS = VDDPA_DISABLE_BITBAND;
//    ACS_VDDPA_CTRL->VDDPA_SW_CTRL_ALIAS = VDDPA_SW_VDDRF_BITBAND;

    /* Enable RF power switches */
    SYSCTRL_RF_POWER_CFG->RF_POWER_ALIAS = RF_POWER_ENABLE_BITBAND;

    /* Remove RF isolation */
    SYSCTRL_RF_ACCESS_CFG->RF_ACCESS_ALIAS = RF_ACCESS_ENABLE_BITBAND;

    /* Start the 48 MHz oscillator without changing the other register bits */
    RF->XTAL_CTRL = ((RF->XTAL_CTRL & ~XTAL_CTRL_DISABLE_OSCILLATOR) |
                     XTAL_CTRL_REG_VALUE_SEL_INTERNAL);

//    /* Wait until 48 MHz oscillator is started */
    while (RF_REG39->ANALOG_INFO_CLK_DIG_READY_ALIAS !=
           ANALOG_INFO_CLK_DIG_READY_BITBAND);

    HAL_SetClockPrescalers(clk_conf);

    /* Disable JTAG debugging interface to allow control over DIO pads 13, 14, 15. */
    DIO->JTAG_SW_PAD_CFG = JTMS_NO_PULL | JTMS_6X_DRIVE | JTCK_NO_PULL
            | CM3_JTAG_TRST_ENABLED | CM3_JTAG_DATA_DISABLED | JTMS_LPF_ENABLED
            | JTCK_LPF_ENABLED;

    HAL_TICK_Init();

    srand(1);

    /* Customized parameters for the LLD SLEEP module
         * respect to OSC wake-up timings in us */
    struct lld_sleep_params_t desired_lld_sleep_params;
    /* Customized parameters for the LLD SLEEP module
         * respect to OSC wake-up timings in us */
//    if (RTC_CLK_SRC == RTC_CLK_SRC_RC_OSC)
//    {
//        desired_lld_sleep_params.twosc = TWOSC_RC_OSC;
//    }
//    else
//    {
        desired_lld_sleep_params.twosc = 1400;
//    }
    BLE_LLD_Sleep_Params_Set(desired_lld_sleep_params);

    /* Trim RC oscillator to 3 MHz (required by Sys_PowerModes_Wakeup) */
    Sys_Clocks_OscRCCalibratedConfig(3000);

    /* BLE not in sleep mode and ready for normal operations */
    BLE_Is_Awake_Flag_Set();

    /* Stop masking interrupts */
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
    __set_FAULTMASK(FAULTMASK_ENABLE_INTERRUPTS);
}

uint32_t HAL_Time(void)
{
    return hal_time_counter;
}

void HAL_Delay(const uint32_t ms)
{
    ASSERT_DEBUG(HAL_IsInterrupt() == false);

    uint32_t time_start = HAL_Time();
    while (HAL_Time() - time_start < ms)
    {
        SYS_WAIT_FOR_INTERRUPT;
    }
}

bool HAL_IsInterrupt(void)
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

void HAL_TICK_Init(void)
{
    const uint32_t slowclk_freq = SystemCoreClock
            / ((CLK->DIV_CFG0 & CLK_DIV_CFG0_SLOWCLK_PRESCALE_Mask) + 1);

    // Configure Timer0 to run at 1kHz
    const uint32_t tim0_cfg = TIMER_FREE_RUN | TIMER_SLOWCLK_DIV2
            | TIMER_PRESCALE_1;
    const uint32_t tim0_val = (slowclk_freq / 2) / HAL_TIME_RESOLUTION_US;
    Sys_Timer_Set_Control(0, tim0_cfg | tim0_val);

    NVIC_ClearPendingIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER0_IRQn);

    Sys_Timers_Start(SELECT_TIMER0);
}

void TIMER0_IRQHandler(void)
{
    hal_time_counter += 1;
}

//! \}
//! \}

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

#include "app.h"
#include "RTE_app_config.h"

#include "calibration.h"


struct sleep_mode_env_tag sleep_mode_env;


static void ConfigurePowerSupply(void)
{
    uint8_t calib_status;

    /* Load Manufacturing calibration data for internal power supplies based on
     * selected TX power level defined in calibration.h
     */
    calib_status = Load_Trim_Values_And_Calibrate_MANU_CALIB();
    ASSERT(calib_status == VOLTAGES_CALIB_NO_ERROR);

    /* Configure the inductor charge current trimming setting for VCC */
    ACS_VCC_CTRL->ICH_TRIM_BYTE = VCC_ICHTRIM_80MA_BYTE;
    /* Enable buck converter for VBAT > 1.4V */
    ACS_VCC_CTRL->BUCK_ENABLE_ALIAS = VCC_BUCK_BITBAND;

    /* Start 48 MHz XTAL oscillator */
    ACS_VDDRF_CTRL->ENABLE_ALIAS = VDDRF_ENABLE_BITBAND;
    ACS_VDDRF_CTRL->CLAMP_ALIAS = VDDRF_DISABLE_HIZ_BITBAND;
    /* Wait until VDDRF supply has powered up */
    while (ACS_VDDRF_CTRL->READY_ALIAS != VDDRF_READY_BITBAND);

    /* Enable RF power switches (VDDRF) */
    SYSCTRL_RF_POWER_CFG->RF_POWER_ALIAS = RF_POWER_ENABLE_BITBAND;

    /* Enable access to RF registers to start 48MHz oscillator & configure
     * RF power amplifier. */
    SYSCTRL_RF_ACCESS_CFG->RF_ACCESS_ALIAS = RF_ACCESS_ENABLE_BITBAND;

    /* Set radio output power of RF */
    Sys_RFFE_SetTXPower(Load_Tx_Power_Value());
}

static void ConfigureSystemClocks(void)
{
    /* Start the 48 MHz oscillator without changing the other register bits */
    RF->XTAL_CTRL = ((RF->XTAL_CTRL & ~XTAL_CTRL_DISABLE_OSCILLATOR)
            | XTAL_CTRL_REG_VALUE_SEL_INTERNAL);

    /* Set RF clock prescaler to desired value */
    RF_REG2F->CK_DIV_1_6_CK_DIV_1_6_BYTE = RF_CK_DIV_PRESCALE_VALUE;

    /* Wait until 48 MHz oscillator is started */
    while (RF_REG39->ANALOG_INFO_CLK_DIG_READY_ALIAS
            != ANALOG_INFO_CLK_DIG_READY_BITBAND)
        ;

    /* Set SYSCLK clock source to prescaled RFCLK clock and apply FLASH
     * delay setting. */
    Sys_Clocks_SystemClkConfig(
            SYSCLK_CLKSRC_RFCLK | JTCK_PRESCALE_1 | EXTCLK_PRESCALE_1);

    /* Set prescalers for SLOWCLK, BBCLK and USRCLK.
     *
     * SLOWCLK - typically set to 1MHz
     * BBCLK - recommended 8MHz or 16MHz
     * USRCLK - application specific
     */
    Sys_Clocks_SystemClkPrescale0(
            SLOWCLK_PRESCALE_VALUE | BBCLK_PRESCALE_VALUE | USRCLK_PRESCALE_1);

    /* Set power clock prescalers for DCDC converter and VDDA charge pump. */
    Sys_Clocks_SystemClkPrescale2(DCCLK_PRESCALE_BYTE_VALUE | CPCLK_PRESCALE_8);

    /* Enable and set BBCLK_DIV clock to 1MHz. */
    BBIF->CTRL = BB_CLK_ENABLE | BB_DEEP_SLEEP | BBCLK_DIVIDER_VALUE;
}

void Device_Initialize(void)
{
    /* Mask all interrupts */
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);

    /* Disable all interrupts and clear any pending interrupts */
    Sys_NVIC_DisableAllInt();
    Sys_NVIC_ClearAllPendingInt();

    ConfigurePowerSupply();

    ConfigureSystemClocks();

    trace_init();

    /* Seed the random number generator */
    srand(1);

    /* Disable JTAG debugging interface to allow control over DIO pads 13, 14, 15. */
    DIO->JTAG_SW_PAD_CFG = JTMS_NO_PULL | JTMS_6X_DRIVE | JTCK_NO_PULL
            | CM3_JTAG_TRST_ENABLED | CM3_JTAG_DATA_DISABLED | JTMS_LPF_ENABLED
            | JTCK_LPF_ENABLED;

    /* Customized parameters for the LLD SLEEP module
     * respect to OSC wake-up timings in us for XTAL32*/
    struct lld_sleep_params_t desired_lld_sleep_params;
    desired_lld_sleep_params.twosc = 1400;
    BLE_LLD_Sleep_Params_Set(desired_lld_sleep_params);

    /* Trim RC oscillator to 3 MHz (required by Sys_PowerModes_Wakeup) */
    Sys_Clocks_OscRCCalibratedConfig(3000);

    /* BLE not in sleep mode and ready for normal operations */
    BLE_Is_Awake_Flag_Set();

    /* Initialize the baseband and BLE stack */
    BLE_Initialize();

    /* Set radio output power of RF */
    Sys_RFFE_SetTXPower(Load_Tx_Power_Value());

    /* Initialize sleep configuration. */
    Sleep_Mode_Configure(&sleep_mode_env);

    /* RTC has to be enabled after sleep mode and the XTAL32 are configured. */
    Timer_Initialize();

    /* Initialize environment */
    App_Env_Initialize();

    /* Stop masking interrupts */
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
    __set_FAULTMASK(FAULTMASK_ENABLE_INTERRUPTS);
}

void Device_Initialize_WakeUp(void)
{
    /* Mask all interrupts */
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);

    /* Lower drive strength (required when VDDO > 2.7)*/
    DIO->PAD_CFG = PAD_LOW_DRIVE;

    /* Disable JTAG pads */
    DIO->JTAG_SW_PAD_CFG = JTMS_NO_PULL | JTMS_6X_DRIVE | JTCK_NO_PULL
            | CM3_JTAG_TRST_ENABLED | CM3_JTAG_DATA_DISABLED | JTMS_LPF_ENABLED
            | JTCK_LPF_ENABLED;

    /* Restore DIO pad configuration before disabling pad retention. */
    LED_Initialize(LED_RED);
    LED_Initialize(ANALOG_POWER);

    /* Turn off pad retention */
    ACS_WAKEUP_CTRL->PADS_RETENTION_EN_BYTE = PADS_RETENTION_ENABLE_BYTE;

    /* Configure clock dividers after wake-up. */
    ConfigureSystemClocks();

    /* Restart 1ms tick timer. */
    HAL_TICK_Init();

    /* Wake-up application timer and the underlying RTC libraries. */
    Timer_Wakeup();

    /* Stop masking interrupts. */
    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);

    /* Mask all interrupts */
    BBIF_CTRL->WAKEUP_REQ_ALIAS = 1;
    __disable_irq();

    while (!(BLE_Is_Awake()))
    {
        /* Process interrupt */
        __enable_irq();
        __disable_irq();
    }
    BBIF_CTRL->WAKEUP_REQ_ALIAS = 0;

    /* Stop masking interrupts */
    __enable_irq();

    /* Initialize debug trace if available. */
    trace_init();

    /* Print wake-up source. */
#ifndef APP_TRACE_DISABLED
    TRACE_PRINTF("\r\n\nWakeup source: ");
    switch (ACS_WAKEUP_STATE->WAKEUP_SRC_BYTE)
    {
    case WAKEUP_DUE_TO_RTC_ALARM_BYTE:
    	ledNotif(1);
        TRACE_PRINTF("RTC\r\n");
        break;
    case WAKEUP_DUE_TO_BB_TIMER_BYTE:
    	ledNotif(2);
        TRACE_PRINTF("BB_TIM\r\n");
        break;
    default:
    	ledNotif(3);
        TRACE_PRINTF("other (0x%x)\r\n", ACS_WAKEUP_STATE->WAKEUP_SRC_BYTE);
        break;
    }
#endif

    /* Main application loop */
    Main_Loop();
}

void BLE_Initialize(void)
{
    Kernel_Init(0);

    BDK_BLE_Initialize();

    /* Set desired min / max advertising intervals. */
    BDK_BLE_SetAdvertisementInterval(RTE_APP_BLE_ADV_INT / 0.625f,
            RTE_APP_BLE_ADV_INT / 0.625f);

    /* Set custom device name.
     * The name needs to contain one of 'IDK', 'BDK', 'RSL10' or 'BLE_Terminal'
     * patterns to be recognized by RSL10 Sense & control mobile application.
     * Default: 'HB_BLE_Terminal'
     */
     BDK_BLE_SetLocalName(RTE_APP_BLE_COMPLETE_LOCAL_NAME);

     /* Initialize IDK Custom Service and associated libraries. */
     CS_Init();
}

void analogOff() {
	LED_Off(ANALOG_POWER);
    Sys_DIO_Config(PIN_ADS7142_ALERT, DIO_MODE_DISABLE );
    Sys_DIO_Config(PIN_ADS7142_READY, DIO_MODE_DISABLE );
}

void analogOn() {
    Sys_DIO_Config(PIN_ADS7142_READY, DIO_MODE_GPIO_IN_0 | DIO_WEAK_PULL_UP );
    Sys_DIO_Config(PIN_ADS7142_ALERT, DIO_MODE_GPIO_IN_0 /* | DIO_WEAK_PULL_UP */ );
	LED_On(ANALOG_POWER);
}

void App_Env_Initialize(void)
{
    /* Initialize all LEDs. */
    LED_Initialize(LED_RED);
    LED_Initialize(ANALOG_POWER);

    Sys_DIO_Config(PIN_ADS7142_ALERT, DIO_MODE_DISABLE );
    Sys_DIO_Config(PIN_ADS7142_READY, DIO_MODE_DISABLE );

    /* Start application task. */
    BDK_TaskStart();

    /* Set I2C bus speed based on HW limitations.
     * Speed setting is retained across deep sleep and
     * peripheral init / deinit cycles.
     */

    HAL_I2C_SetBusSpeed(RTE_APP_I2C_BUS_SPEED);

    /* Initialize 1ms timer for basic timing of application. */
    HAL_TICK_Init();

    /* Initialize and put on-board sensors to sleep modes. */
    TRACE_PRINTF("Initializing sensors.\r\n");

    // reset
	analogOff();
	HAL_Delay(500);

	analogOn();
	HAL_Delay(500);

    /** init ADS7142 **/
#if RTE_APP_ICS_EV_ENABLED == 1
    CS_RegisterNode(CSN_LP_ADS7142_Create(Timer_GetContext()));
#endif

    TRACE_PRINTF("Initializing sensors done.\r\n");
}

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
//-----------------------------------------------------------------------------
#include <NOA1305_ALS.h>

#include <BDK.h>

#include <EventCallback.h>
#include <SoftwareTimer.h>

#include <RTE_HB_NOA1305_ALS.h>

#if RTE_NOA1305_ALS_INT_ENABLED == 1

#if RTE_NOA1305_ALS_INT_SRC == 0
#define NOA1305_ALS_IRQn                      (DIO0_IRQn)
#define NOA1305_ALS_ISR                       DIO0_IRQHandler
#elif RTE_NOA1305_ALS_INT_SRC == 1
#define NOA1305_ALS_IRQn                      (DIO1_IRQn)
#define NOA1305_ALS_ISR                       DIO1_IRQHandler
#elif RTE_NOA1305_ALS_INT_SRC == 2
#define NOA1305_ALS_IRQn                      (DIO2_IRQn)
#define NOA1305_ALS_ISR                       DIO2_IRQHandler
#elif RTE_NOA1305_ALS_INT_SRC == 3
#define NOA1305_ALS_IRQn                      (DIO3_IRQn)
#define NOA1305_ALS_ISR                       DIO3_IRQHandler
#endif

#endif /* RTE_NOA1305_ALS_INT_ENABLED == 1 */

static uint16_t noa_it_values[] = {
        800,
        400,
        200,
        100,
        50,
        25,
        13,
        7
};

static struct noa1305_t noa_dev;

static NOA1305_ALS_ReadCallback noa_read_cb = NULL;

static struct SwTimer noa_timer;

#if RTE_NOA1305_ALS_INT_ENABLED == 1

static void NOA1305_ALS_InteruptEventTask(void *arg)
{
    static int32_t last_int_level = 1;

    int32_t int_level = ((int32_t)arg) != 0;

    if (noa_read_cb != NULL && int_level != last_int_level)
    {
        last_int_level = int_level;

        uint32_t lux = 0;
        noa1305_convert_als_data_lux(&lux, &noa_dev);
        noa_read_cb(lux, true);
    }
}

void NOA1305_ALS_ISR(void)
{
    int32_t int_level = DIO->DATA & (1 << RTE_NOA1305_ALS_DIO_INT_PIN);

    BDK_TaskSchedule(&NOA1305_ALS_InteruptEventTask, (void*)int_level);
}

#endif /* RTE_NOA1305_ALS_INT_ENABLED == 1 */

static void NOA1305_ALS_OneShotTimerCallback(void *arg)
{
    int32_t retval = NOA1305_OK;
    uint32_t lux = 0;

    SwTimer_Remove(&noa_timer);

    if (noa_read_cb != NULL)
    {
        // read lux
        retval = noa1305_convert_als_data_lux(&lux, &noa_dev);
        if (retval == NOA1305_OK)
        {
            // power down chip
            noa1305_set_power_mode(NOA1305_POWER_DOWN, &noa_dev);

            // call attached callback
            noa_read_cb(lux, false);
        }

    }
}

static void NOA1305_ALS_ContinuousTimerCallback(void *arg)
{
    SwTimer_Advance(&noa_timer);

    if (noa_read_cb != NULL)
    {
        uint32_t lux = 0;
        noa1305_convert_als_data_lux(&lux, &noa_dev);
        noa_read_cb(lux, false);
    }
}

static int32_t NOA1305_ALS_BusRead(uint8_t dev_id, uint8_t addr, uint8_t *value)
{
    if (HAL_I2C_Write(dev_id, &addr, 1, false) != HAL_OK)
    {
        return NOA1305_COMM_ERROR;
    }

    if (HAL_I2C_Read(dev_id, value, 1, false) != HAL_OK)
    {
        return NOA1305_COMM_ERROR;
    }

    return NOA1305_COMM_OK;
}

static int32_t NOA1305_ALS_BusWrite(uint8_t dev_id, uint8_t addr, uint8_t value)
{
    uint8_t data[2] = { addr, value };

    if (HAL_I2C_Write(dev_id, data, 2, false) != HAL_OK)
    {
        return NOA1305_COMM_ERROR;
    }

    return NOA1305_COMM_OK;
}

int32_t NOA1305_ALS_InitializeParam(uint32_t I_k, uint8_t T_i)
{
    int32_t retval = NOA1305_OK;

    noa_dev.id = NOA1305_I2C_ADDR;
    noa_dev.integration_constatnt = I_k;

    noa_dev.read_func = &NOA1305_ALS_BusRead;
    noa_dev.write_func = &NOA1305_ALS_BusWrite;
    noa_dev.delay_func = &HAL_Delay;

    retval = noa1305_init(&noa_dev);
    if (retval == NOA1305_OK)
    {
        retval = noa1305_set_int_select(NOA1305_INT_INACTIVE, &noa_dev);
        if (retval == NOA1305_OK)
        {
            retval = noa1305_set_integration_time(T_i, &noa_dev);
            if (retval == NOA1305_OK)
            {
                // reset ALS-GEVB shield io expander
//              retval = PCA9655E_Init(&noa_ioexp, NOA1305_ALS_IOEXP_ADDR);
//              if (retval != HAL_OK)
//              {
//                  retval = NOA1305_E_COMM;
//              }
            }
        }
    }

    return retval;
}

int32_t NOA1305_ALS_StartOneShot(NOA1305_ALS_ReadCallback cb)
{
    int32_t retval = NOA1305_OK;

    uint8_t it_setting = 0;

    retval = noa1305_set_power_mode(NOA1305_POWER_ON, &noa_dev);
    if (retval == NOA1305_OK)
    {
        retval = noa1305_get_integration_time(&it_setting, &noa_dev);
        if (retval == NOA1305_OK)
        {
            // start timeout until valid data are available
            SwTimer_Initialize(&noa_timer);
            SwTimer_AttachScheduled(&noa_timer,
                    &NOA1305_ALS_OneShotTimerCallback, NULL);
            noa_read_cb = cb;
            SwTimer_ExpireInMs(&noa_timer, 4 * noa_it_values[it_setting]);
        }
    }

    return retval;
}

int32_t NOA1305_ALS_StartContinuous(uint32_t period_ms,
        NOA1305_ALS_ReadCallback cb)
{
    int32_t retval = NOA1305_OK;

    retval = noa1305_set_power_mode(NOA1305_POWER_ON, &noa_dev);
    if (retval == NOA1305_OK)
    {
        if (cb != NULL)
        {
            noa_read_cb = cb;

            if (period_ms != 0)
            {
                SwTimer_Initialize(&noa_timer);
                SwTimer_AttachScheduled(&noa_timer,
                        &NOA1305_ALS_ContinuousTimerCallback, NULL);
                SwTimer_ExpireInMs(&noa_timer, period_ms);
            }
        }
    }

    return retval;
}

int32_t NOA1305_ALS_Stop(void)
{
    int32_t retval = NOA1305_OK;

    retval = noa1305_set_power_mode(NOA1305_POWER_DOWN, &noa_dev);

    // disable timers

    return retval;
}

int32_t NOA1305_ALS_ReadLux(uint32_t *lux)
{
    int32_t retval = NOA1305_OK;

    uint8_t pm;
    retval = noa1305_get_power_mode(&pm, &noa_dev);
    ASSERT_DEBUG(pm == NOA1305_POWER_ON);

    if (lux != NULL)
    {
        retval = noa1305_convert_als_data_lux(lux, &noa_dev);
    }
    else
    {
        retval = NOA1305_E_NULL_PTR;
    }

    return retval;
}

int32_t NOA1305_ALS_EnableInterrupt(uint32_t lux_threshold)
{
    int32_t retval = NOA1305_OK;

    NVIC_DisableIRQ(NOA1305_ALS_IRQn);

#if RTE_NOA1305_ALS_INT_ENABLED == 1
    // Set int threshold
    retval = noa1305_set_int_threshold_lux(lux_threshold, &noa_dev);
    if (retval == NOA1305_OK)
    {
        // Enable noa1305 INT pin
        retval = noa1305_set_int_select(NOA1305_INT_HIGH_TO_LOW, &noa_dev);
        if (retval == NOA1305_OK)
        {
            // Configure DIO pad
            Sys_DIO_Config(RTE_NOA1305_ALS_DIO_INT_PIN,
                    DIO_NO_PULL | DIO_MODE_GPIO_IN_0 | DIO_LPF_DISABLE);

            Sys_DIO_IntConfig(RTE_NOA1305_ALS_INT_SRC,
                    DIO_EVENT_TRANSITION | DIO_DEBOUNCE_ENABLE
                            | RTE_NOA1305_ALS_DIO_INT_PIN,
                    DIO_DEBOUNCE_SLOWCLK_DIV32, 10);

            NVIC_ClearPendingIRQ(NOA1305_ALS_IRQn);
            NVIC_EnableIRQ(NOA1305_ALS_IRQn);
        }
    }
#else
    retval = NOA1305_UNSUPPORTED;
#endif

    return retval;
}

int32_t NOA1305_ALS_DisableInterrupt(void)
{
    int32_t retval = NOA1305_OK;

#if RTE_NOA1305_ALS_INT_ENABLED == 1
    NVIC_DisableIRQ(NOA1305_ALS_IRQn);

    // Disable interrupt pin of NOA1305 -> INT pin always high
    retval = noa1305_set_int_select(NOA1305_INT_INACTIVE, &noa_dev);
#else
    retval = NOA1305_UNSUPPORTED;
#endif

    return retval;
}

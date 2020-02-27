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
//! \file BHI160_NDOF.c
//!
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "BDK.h"

#include <BHI160_NDOF.h>
#include <bhy1_firmware.h>

#include <RTE_HB_BHI160_NDOF.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

#if RTE_HB_BHI160_NDOF_INT_SRC == 0
#define BHI160_NDOF_IRQn                      (DIO0_IRQn)
#define BHI160_NDOF_ISR                       DIO0_IRQHandler
#elif RTE_HB_BHI160_NDOF_INT_SRC == 1
#define BHI160_NDOF_IRQn                      (DIO1_IRQn)
#define BHI160_NDOF_ISR                       DIO1_IRQHandler
#elif RTE_HB_BHI160_NDOF_INT_SRC == 2
#define BHI160_NDOF_IRQn                      (DIO2_IRQn)
#define BHI160_NDOF_ISR                       DIO2_IRQHandler
#elif RTE_HB_BHI160_NDOF_INT_SRC == 3
#define BHI160_NDOF_IRQn                      (DIO3_IRQn)
#define BHI160_NDOF_ISR                       DIO3_IRQHandler
#endif

#define FIFO_SIZE                      300
#define MAX_PACKET_LENGTH              18
#define OUT_BUFFER_SIZE                60

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static uint32_t bhi160_timestamp = 0;

static uint16_t bhi160_accel_dyn_range = 4;
static uint16_t bhi160_gyro_dyn_range = 2000;
static uint16_t bhi160_mag_dyn_range = 1000;

uint8_t bhi160_fifo[FIFO_SIZE];
uint8_t bhi160_bytes_left_in_fifo = 0;
uint16_t bytes_remaining = 0;




//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

static void BHI160_NDOF_TimestampCallback(bhy_data_scalar_u16_t *data)
{
    bhy_update_system_timestamp(data, &bhi160_timestamp);
}

static void BHI160_NDOF_DynamicRangeCallback(bhy_data_meta_event_t *data, bhy_meta_event_type_t event)
{
    if (event == BHY_META_EVENT_TYPE_DYNAMIC_RANGE_CHANGED)
    {
        struct accel_physical_status_t accel;
        struct gyro_physical_status_t gyro;
        struct mag_physical_status_t mag;

        int32_t retval = bhy_get_physical_sensor_status(&accel, &gyro, &mag);
        ASSERT_DEBUG(retval == BHY_SUCCESS);

        if (retval == BHY_SUCCESS)
        {
            bhi160_accel_dyn_range = accel.accel_dynamic_range;
            bhi160_gyro_dyn_range = gyro.gyro_dynamic_range;
            bhi160_mag_dyn_range = mag.mag_dynamic_range;
        }
    }
}

static void BHI160_NDOF_FifoRoutine(void *arg)
{
    (void)arg;
    uint8_t *fifoptr = bhi160_fifo;
    bhy_data_type_t packet_type = BHY_DATA_TYPE_PADDING;
    bhy_data_generic_t fifo_packet;
    uint16_t bytes_read = 0;
    BHY_RETURN_FUNCTION_TYPE result;

//    while (!(DIO->DATA & (1 << PIN_INT_BHI160)) && !bytes_remaining)
//    {
//
//    }

    bhy_read_fifo(bhi160_fifo + bhi160_bytes_left_in_fifo,
            FIFO_SIZE - bhi160_bytes_left_in_fifo, &bytes_read,
            &bytes_remaining);
    bytes_read += bhi160_bytes_left_in_fifo;

    do
    {
        /* this function will call callbacks that are registered */
        result = bhy_parse_next_fifo_packet(&fifoptr, &bytes_read, &fifo_packet,
                &packet_type);

        /* prints all the debug packets */
        if (packet_type == BHY_DATA_TYPE_DEBUG)
        {
            bhy_print_debug_packet(&fifo_packet.data_debug, bhy_printf);
        }

        /* the logic here is that if doing a partial parsing of the fifo, then we should not parse  */
        /* the last 18 bytes (max length of a packet) so that we don't try to parse an incomplete   */
        /* packet */
    } while ((result == BHY_SUCCESS)
            && (bytes_read > (bytes_remaining ? MAX_PACKET_LENGTH : 0)));

    bhi160_bytes_left_in_fifo = 0;

    if (bytes_remaining)
    {
        /* shifts the remaining bytes to the beginning of the buffer */
        while (bhi160_bytes_left_in_fifo < bytes_read)
        {
            bhi160_fifo[bhi160_bytes_left_in_fifo++] = *(fifoptr++);
        }
    }
}

void BHI160_NDOF_ISR(void)
{
    BDK_TaskSchedule(&BHI160_NDOF_FifoRoutine, NULL);
}

int32_t BHI160_NDOF_Initialize(void)
{
    int32_t retval = BHY_SUCCESS;

    /* Configure BHI160 interrupt pin. */
    NVIC_DisableIRQ(BHI160_NDOF_IRQn);

    Sys_DIO_Config(PIN_INT_BHI160, DIO_MODE_GPIO_IN_0 | DIO_NO_PULL);

    Sys_DIO_IntConfig(RTE_HB_BHI160_NDOF_INT_SRC,
            DIO_DEBOUNCE_ENABLE | DIO_EVENT_RISING_EDGE | PIN_INT_BHI160,
            DIO_DEBOUNCE_SLOWCLK_DIV32, 1);

    /* Check I2C communication and upload RAM patch with BMM150 support into
     * BHI160. */
    retval = bhy_driver_init(bhy1_fw);
    if (retval != BHY_SUCCESS)
    {
        return retval;
    }

    /* Wait for BMI160 restart to complete. */
    while ((DIO->DATA & (1 << PIN_INT_BHI160)) == 1);
    while ((DIO->DATA & (1 << PIN_INT_BHI160)) == 0);

    /* Remap sensor axes based on relative placement of BHI160 and BMM150 on the
     * board */
    int8_t bhi160_mapping[9] = BHI160_NDOF_BHI160_AXIS_MAPPING;
    int8_t bmm150_mapping[9] = BHI160_NDOF_BMM150_AXIS_MAPPING;

    retval = bhy_mapping_matrix_set(PHYSICAL_SENSOR_INDEX_ACC, bhi160_mapping);
    if (retval != BHY_SUCCESS)
    {
        return retval;
    }
    retval = bhy_mapping_matrix_set(PHYSICAL_SENSOR_INDEX_GYRO, bhi160_mapping);
    if (retval != BHY_SUCCESS)
    {
        return retval;
    }
    retval = bhy_mapping_matrix_set(PHYSICAL_SENSOR_INDEX_MAG, bmm150_mapping);
    if (retval != BHY_SUCCESS)
    {
        return retval;
    }

    /* Set callback for processing of system timestamps. */
    retval = bhy_install_timestamp_callback(VS_NON_WAKEUP,
            &BHI160_NDOF_TimestampCallback);
    if (retval != BHY_SUCCESS)
    {
        return retval;
    }

    /* Listen for dynamic range changes of physical sensors. */
    retval = bhy_install_meta_event_callback(BHY_META_EVENT_TYPE_DYNAMIC_RANGE_CHANGED, &BHI160_NDOF_DynamicRangeCallback);
    if (retval != BHY_SUCCESS)
    {
        return retval;
    }

    NVIC_ClearPendingIRQ(BHI160_NDOF_IRQn);
    NVIC_EnableIRQ(BHI160_NDOF_IRQn);

    BDK_TaskSchedule(&BHI160_NDOF_FifoRoutine, NULL);

    return retval;
}

void BHI160_NDOF_PadRestore(void)
{
    Sys_DIO_Config(PIN_INT_BHI160, DIO_MODE_GPIO_IN_0 | DIO_NO_PULL);

    Sys_DIO_IntConfig(RTE_HB_BHI160_NDOF_INT_SRC,
            DIO_DEBOUNCE_ENABLE | DIO_EVENT_RISING_EDGE | PIN_INT_BHI160,
            DIO_DEBOUNCE_SLOWCLK_DIV32, 1);

    NVIC_EnableIRQ(BHI160_NDOF_IRQn);
}

uint32_t BHI160_NDOF_GetTimestamp(void)
{
    return bhi160_timestamp;
}

int32_t BHI160_NDOF_SetPowerMode(enum BHI160_NDOF_PowerMode power_mode)
{
    int32_t retval = BHY_SUCCESS;

    switch (power_mode)
    {
    case BHI160_NDOF_PM_NORMAL:
        retval = bhy_set_host_interface_control(BHY_HOST_ALGO_STANDBY_REQUEST,
                0);
        if (retval == BHY_SUCCESS)
        {
            retval = bhy_set_host_interface_control(BHY_HOST_AP_SUSPEND, 0);
        }
        break;
    case BHI160_NDOF_PM_AP_SUSPEND:
        retval = bhy_set_host_interface_control(BHY_HOST_ALGO_STANDBY_REQUEST,
                0);
        if (retval == BHY_SUCCESS)
        {
            retval = bhy_set_host_interface_control(BHY_HOST_AP_SUSPEND, 1);
        }
        break;
    case BHI160_NDOF_PM_STANDBY:
        retval = bhy_set_host_interface_control(BHY_HOST_ALGO_STANDBY_REQUEST,
                1);
        break;
    }

    return retval;
}

int32_t BHI160_NDOF_EnableSensor(enum BHI160_NDOF_Sensor sensor,
        BHI160_NDOF_SensorCallback cb, uint16_t sample_rate)
{
    int32_t retval = BHY_SUCCESS;

    retval = bhy_install_sensor_callback(sensor, VS_WAKEUP, cb);
    if (retval == BHY_SUCCESS)
    {
        retval = bhy_enable_virtual_sensor(sensor, VS_WAKEUP,
                sample_rate, 0, VS_FLUSH_NONE, 0, 0);
    }

    return retval;
}

uint16_t BHI160_NDOF_GetAccelDynamicRange(void)
{
    return bhi160_accel_dyn_range;
}

uint16_t BHI160_NDOF_GetGyroDynamicRange(void)
{
    return bhi160_gyro_dyn_range;
}

uint16_t BHI160_NDOF_GetMagDynamicRange(void)
{
    return bhi160_mag_dyn_range;
}

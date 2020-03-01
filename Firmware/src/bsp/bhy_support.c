/*!
  * Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  * 
  * Redistributions of source code must retain the above copyright
  * notice, this list of conditions and the following disclaimer.
  * 
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  * 
  * Neither the name of the copyright holder nor the names of the
  * contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  * 
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
  * OR CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
  * OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  * ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
  * 
  * The information provided is believed to be accurate and reliable.
  * The copyright holder assumes no responsibility
  * for the consequences of use
  * of such information nor for any infringement of patents or
  * other rights of third parties which may result from its use.
  * No license is granted by implication or otherwise under any patent or
  * patent rights of the copyright holder.
  *
  *
  * @file              bhy_support.c
  *
  * @date              12/19/2016
  *
  * @brief             driver on MCU for bhy
  *
  *
  */


/********************************************************************************/
/*                                  HEADER FILES                                */
/********************************************************************************/
#include "bhy_support.h"
#include "bhy_uc_driver_config.h"

#include <stdio.h>
#include <BDK.h>

/********************************************************************************/
/*                                STATIC VARIABLES                              */
/********************************************************************************/
static struct bhy_t bhy;
//static uint8_t *version = BHY_MCU_REFERENCE_VERSION;

/********************************************************************************/
/*                         EXTERN FUNCTION DECLARATIONS                         */
/********************************************************************************/
static int8_t sensor_i2c_write(uint8_t addr, uint8_t reg, uint8_t *p_buf, uint16_t size)
{
    int32_t status;

    uint8_t *write_data = malloc(size + 1);
    if (write_data == NULL)
    {
        return -1;
    }
    write_data[0] = reg;
    memcpy(write_data + 1, p_buf, size);

    status = HAL_I2C_Write(addr, write_data, size + 1, false);

    free(write_data);
    return status;
}

static int8_t sensor_i2c_read(uint8_t addr, uint8_t reg, uint8_t *p_buf, uint16_t size)
{
	int32_t status;

	status = HAL_I2C_Write(addr, &reg, 1, false);
	if (status != HAL_OK)
	{
		return status;
	}

	return HAL_I2C_Read(addr, p_buf, size, false);
}

/********************************************************************************/
/*                             FUNCTION DECLARATIONS                            */
/********************************************************************************/
/*!
* @brief        Initializes BHY smart sensor and its required connections
*
*/
int8_t bhy_initialize_support(void)
{
    uint8_t tmp_retry = RETRY_NUM;

    bhy.bus_write = &sensor_i2c_write;
    bhy.bus_read = &sensor_i2c_read;
    bhy.delay_msec  = &bhy_delay_msec;
    bhy.device_addr = BHY_I2C_SLAVE_ADDRESS;

    bhy_init(&bhy);

    bhy_set_reset_request(BHY_RESET_ENABLE);

    while(tmp_retry--)
    {
        bhy_get_product_id(&bhy.product_id);

        if(PRODUCT_ID_7183 == bhy.product_id)
        {
            return BHY_SUCCESS;
        }

        bhy_delay_msec(BHY_PARAMETER_ACK_DELAY);
    }

    return BHY_PRODUCT_ID_ERROR;
}
/*!
* @brief        Initiates a delay of the length of the argument in milliseconds
*
* @param[in]    msec    Delay length in terms of milliseconds
*
*/
void bhy_delay_msec(u32 msec)
{
    HAL_Delay(msec);
}
/*!
 * @brief provides a print function to the bhy driver on DD2.0 platform
 */
void bhy_printf(const u8 * string)
{
    printf("%s",string);
}
/*!
 * @brief provides the mcu reference code version
 */
//uint8_t * bhy_get_version(void)
//{
//    return (version);
//}
/** @}*/

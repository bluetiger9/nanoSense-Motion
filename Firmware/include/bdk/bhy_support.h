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
  * @file              bhy_support.h
  *
  * @date              12/19/2016
  *
  * @brief             driver on MCU for bhy
  *
  *
  */

#ifndef BHY_SUPPORT_H_
#define BHY_SUPPORT_H_

/********************************************************************************/
/*                                  HEADER FILES                                */
/********************************************************************************/
#include "bhy.h"
#include <stdint.h>

/********************************************************************************/
/*                                     MACROS                                   */
/********************************************************************************/
#define RETRY_NUM                   3

/*! determines the I2C slave address of BHy
* The default I2C address of the BHy device is 0101000b (0x28). */
/* 0x28 CONFLICTS ON ATMEL DEV KITS WITH THE ONBOARD EDBG!!!!   */
#define BHY_I2C_SLAVE_ADDRESS       BHY_I2C_ADDR1
/*! the delay required to wait for BHY chip to reset */
#define BHY_RESET_DELAY_MS          UINT32_C(50)

/*! these two macros are defined for i2c read/write limitation of host */
/*! users must modify these two macros according to their own IIC hardware design */
#define I2C_ONCE_WRITE_MAX_COUNT   (8)
#define I2C_ONCE_READ_MAX_COUNT    (8)

//#define BHY_DEBUG 1

/********************************************************************************/
/*                             FUNCTION DECLARATIONS                            */
/********************************************************************************/
/*!
* @brief        Initializes BHY smart sensor and its required connections
*
*/
int8_t bhy_initialize_support(void);

/*!
* @brief        Initiates a delay of the length of the argument in milliseconds
*
* @param[in]    msec    Delay length in terms of milliseconds
*
*/
void bhy_delay_msec(u32 msec);

/*!
 * @brief provides a print function to the bhy driver on DD2.0 platform
 */
void bhy_printf (const u8 * string);

/*!
 * @brief provides the mcu reference code version
 */
//uint8_t *bhy_get_version(void);


#endif /* BHY_SUPPORT_H_ */

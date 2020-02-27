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
//! \file I2CEeprom.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup I2C_EEPROM_GRP I2C Eeprom Library
//!
//! \brief Simple library for reading and writing of data into I2C EEPROM
//! memories.
//!
//! \{
//-----------------------------------------------------------------------------
#ifndef I2C_EEPROM_H_
#define I2C_EEPROM_H_

#include <stdint.h>
#include <stdlib.h>

/** \brief Library call was successful. */
#define I2C_EEPROM_OK                     (0)

/** \brief Invalid pointer argument was passed to library function. */
#define I2C_EEPROM_E_NULL_PTR             (-1)

/** \brief Transfer function indicated communication error. */
#define I2C_EEPROM_E_COMM                 (-2)

/** \brief Revision ID register content does not match. */
#define I2C_EEPROM_E_DEV_NOT_FOUND        (-3)

/** \brief Argument passed to library function has invalid value. */
#define I2C_EEPROM_E_OUT_OF_RANGE         (-4)


#define I2C_EEPROM_MAX_CHIP_SIZE       (8192)

#define I2C_EEPROM_M24RF04_CHIP_SIZE   (512)
#define I2C_EEPROM_M24RF16_CHIP_SIZE   (2018)
#define I2C_EEPROM_M24RF64_CHIP_SIZE   (8192)

#define I2C_EEPROM_M24RFxx_PAGE_SIZE   (4)

typedef struct
{
    uint8_t i2c_address;
    size_t page_size;
} I2CEeprom;


extern int32_t I2CEeprom_Initialize(uint8_t i2c_address, size_t page_size,
        I2CEeprom *obj);

extern int32_t I2CEeprom_Read(size_t addr, uint8_t *buf, size_t size, I2CEeprom *obj);

extern int32_t I2CEeprom_Write(size_t addr, uint8_t *buf, size_t size, I2CEeprom *obj);


#endif /* I2C_EEPROM_H_ */

//! \}
//! \}
//! \}

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
//! \file I2CEeprom.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup COMPONENTS
//! \{
//! \addtogroup I2C_EEPROM_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include <BDK.h>

#include <I2CEeprom.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static int32_t I2CEeprom_WaitForWrite(I2CEeprom *obj);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

static int32_t I2CEeprom_WaitForWrite(I2CEeprom *obj)
{
    int32_t retval = HAL_OK;
    uint8_t dummy = 0;

    do {
        retval = HAL_I2C_Write(obj->i2c_address, &dummy, 1, false);
    } while (retval == HAL_ERROR_I2C_NACK);

    return retval;
}

int32_t I2CEeprom_Initialize(uint8_t i2c_address, size_t page_size,
        I2CEeprom *obj)
{
    int32_t retval = I2C_EEPROM_OK;

    if (obj != NULL)
    {
        obj->i2c_address = i2c_address;
        obj->page_size = page_size;
    }
    else
    {
        retval = I2C_EEPROM_E_NULL_PTR;
    }

    return retval;
}

int32_t I2CEeprom_Read(size_t addr, uint8_t *buf, size_t size, I2CEeprom *obj)
{
    int32_t retval = I2C_EEPROM_OK;

    if (obj != NULL)
    {
        uint8_t val[2] = { (addr >> 8) & 0xFF, addr & 0xFF };

        retval = HAL_I2C_Write(obj->i2c_address, val, 2, false);
        if (retval == HAL_OK)
        {
            retval = HAL_I2C_Read(obj->i2c_address, buf, size, false);
            if (retval != HAL_OK)
            {
                retval = I2C_EEPROM_E_COMM;
            }
        }
        else
        {
            retval = I2C_EEPROM_E_COMM;
        }
    }
    else
    {
        retval = I2C_EEPROM_E_NULL_PTR;
    }

    return retval;
}

int32_t I2CEeprom_Write(size_t addr, uint8_t *buf, size_t size, I2CEeprom *obj)
{
    int32_t retval = I2C_EEPROM_OK;
    uint8_t *write_buf = NULL;

    if (obj != NULL)
    {
        const uint8_t *page = buf;
        size_t left = size;
        write_buf = malloc(obj->page_size + 2);

        while (left != 0)
        {
            size_t to_write;

            if ((addr % obj->page_size) != 0)
            {
                to_write = (((addr / obj->page_size) + 1) * obj->page_size)
                        - addr;
                if (to_write > size)
                {
                    to_write = size;
                }
            }
            else
            {
                if (left <= obj->page_size)
                {
                    to_write = left;
                }
                else
                {
                    to_write = obj->page_size;
                }
            }

            write_buf[0] = (addr >> 8) & 0xFF;
            write_buf[1] = addr & 0xFF;
            memcpy(write_buf + 2, page, to_write);

            retval = HAL_I2C_Write(obj->i2c_address, write_buf,
                    to_write + 2, false);
            if (retval == HAL_OK)
            {
                retval = I2CEeprom_WaitForWrite(obj);
                if (retval != HAL_OK)
                {
                    retval = I2C_EEPROM_E_COMM;
                    break;
                }

                left -= to_write;
                addr += to_write;
                page += to_write;
            }
            else
            {
                retval = I2C_EEPROM_E_COMM;
                break;
            }
        }
    }
    else
    {
        retval = I2C_EEPROM_E_NULL_PTR;
    }

    if (write_buf != NULL)
    {
        free(write_buf);
    }

    return retval;
}

//! \}
//! \}
//! \}

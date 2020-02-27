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
//! \file PCA9655E.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup API
//! \{
//! \addtogroup PCA9655E_GRP PCA9655E IO Expander
//!
//! \brief Remote 16-bit I/O Expander for I2C Bus with Interrupt
//!
//! \section pca9655e_events Shared Interrupt signal events
//!
//! If interrupt generation from INT line is enabled in RTE configuration
//! (required by most other components) it will also configure selected DIO pad
//! as interrupt source and enables interrupt generation for it.
//!
//! \ref EventCallback is used within the interrupt routine to allow multiple
//! handlers to be attached to the INT signal events.
//! In order to listen for INT signal changes the application has to register
//! an event callback with the event ID configured in RTE configuration.
//!
//! Example for registering multiple custom handlers for INT line events:
//! \include pca9655e_event_example.c
//!
//!
//! \section pca9655e_limitations I2C communication limitations
//!
//! Because RSL10 CMSIS-Driver for I2C does not support generation of repeated
//! start condition it is possible to read only content of two PCA9655E Input
//! Port registers (registers 0, 1) which are continently the only register that
//! can change based on Port pin level.
//! The rest of the register values are cached when written and this value is
//! used when attempting to read these registers.
//! As such creating of multiple PCAE9655E objects for the same chip will result
//! in undefined behavior.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BDK_PCA9655E_H_
#define BDK_PCA9655E_H_

#include "stdbool.h"
#include "stdint.h"

/** \brief Lowest possible 7-bit I2C address of PCA9655E. */
#define PCA9655E_FIRST_ADDRESS           (0x10)

/** \brief Largest possible 7-bit I2C address of PCA9655E. */
#define PCA9655E_LAST_ADDRESS            (0x77)

/**  \brief Number of addressable I2C registers of PCA9655E. */
#define PCA9655E_PORT_COUNT 8

/** \brief Available PCA9655E I2C Port register addresses. */
enum PCA9655E_Ports
{
    PCA9655E_PORT_INPUT0,
    PCA9655E_PORT_INPUT1,
    PCA9655E_PORT_OUTPUT0,
    PCA9655E_PORT_OUTPUT1,
    PCA9655E_PORT_POLARITY0,
    PCA9655E_PORT_POLARITY1,
    PCA9655E_PORT_CONFIG0,
    PCA9655E_PORT_CONFIG1
};

/** \brief Port pins bit definitions. */
enum PCA9655E_Pins
{
    PCA9655E_PIN0 = 0x01,
    PCA9655E_PIN1 = 0x02,
    PCA9655E_PIN2 = 0x04,
    PCA9655E_PIN3 = 0x08,
    PCA9655E_PIN4 = 0x10,
    PCA9655E_PIN5 = 0x20,
    PCA9655E_PIN6 = 0x40,
    PCA9655E_PIN7 = 0x80,
};

typedef struct
{
    /** \brief 7-bit I2C address of this IO Expander. */
    uint8_t addr;

    /** \brief Cached values of all port registers. */
    uint8_t ports[PCA9655E_PORT_COUNT];
} PCA9655E_Type;

/** \brief Initializes an IO Expander chip on the I2C bus.
 *
 * This function is used to initialize both the handle structure and the chip
 * into a known state.
 *
 * This function rewrites all PCA9655E registers with their default restart
 * values and if configured enables INT signal interrupts.
 *
 * \param obj
 * Pointer to uninitialized PCA9655E handle structure.
 *
 * \param i2c_address
 * 7-bit I2C address of this IO Expander.
 *
 * \returns
 * \b HAL_OK - When chip was successfully initialized.<br>
 * \b HAL_E_* - Error code on failure.
 */
extern int32_t PCA9655E_Init(PCA9655E_Type *obj, uint8_t i2c_address);

/** \brief Reads current value of given port register.
 *
 * \param obj
 * Pointer to PCA9655E handle structure.
 *
 * \param port
 * Port address of I2C register to be read.
 * Allowed values are one of the \b PCA9655E_PORT_* defines.
 *
 * \returns
 * Port value in the 8 LSB bits on success.<br>
 * Negative \b HAL_E_* error code on failure.
 */
extern int32_t PCA9655E_ReadPort(PCA9655E_Type *obj, enum PCA9655E_Ports port);

/** \brief Reads current value of specific pin of given port register.
 *
 * \param obj
 * Pointer to PCA9655E handle structure.
 *
 * \param port
 * Port address of I2C register to be read.
 * Allowed values are one of the \b PCA9655E_PORT_* defines.
 *
 * \param pin_mask
 * Pin selection mask.
 * Can be one of PCA9655E_PIN* defines.
 *
 * \returns
 * \b 0 - If bit is low.
 * \b 1 - If bit is high.
 * Negative \b HAL_E_* error code on failure.
 */
extern int32_t PCA9655E_ReadPortBit(PCA9655E_Type *obj,
        enum PCA9655E_Ports port, uint8_t pin_mask);

/** \brief Writes new value into given port register.
 *
 * \param obj
 * Pointer to PCA9655E handle structure.
 *
 * \param port
 * Port address of I2C register to be read.
 * Allowed values are one of the \b PCA9655E_PORT_* defines.
 *
 * \param val
 * New value to be written.
 *
 * \returns
 * \b HAL_OK - On success.
 * Negative \b HAL_E_* error code on failure.
 */
extern int32_t PCA9655E_WritePort(PCA9655E_Type *obj, enum PCA9655E_Ports port,
        uint8_t val);

/** \brief Writes new value to specified pins of port register without changing
 * other bits.
 *
 * \param obj
 * Pointer to PCA9655E handle structure.
 *
 * \param port
 * Port address of I2C register to be read.
 * Allowed values are one of the \b PCA9655E_PORT_* defines.
 *
 * \param pin_mask
 * Selects one or multiple port pins to be changed.
 *
 * \param pin_val
 * New value for pins selected by \p pin_mask .
 *
 * \returns
 * \b HAL_OK - On success.
 * Negative \b HAL_E_* error code on failure.
 */
extern int32_t PCA9655E_WritePortBit(PCA9655E_Type *obj,
        enum PCA9655E_Ports port, uint8_t pin_mask,
        bool pin_val);

/** \brief Helper function that sets given port pin to output direction.
 *
 * \param obj
 * Pointer to PCA9655E handle structure.
 *
 * \param port
 * Which IO port does this pin(s) belong to.
 * Can be either \b 0 or \b 1.
 *
 * \param pin_mask
 * Selects one or more pins to be set into output direction.
 *
 * \returns
 * \b HAL_OK - On success.<br>
 * \b HAL_E_* - Error code on failure.
 */
extern int32_t PCA9655E_SetOutputDir(PCA9655E_Type *obj, uint8_t port,
        uint8_t pin_mask);

/** \brief Helper function that sets given port pin output level.
 *
 * \param obj
 * Pointer to PCA9655E handle structure.
 *
 * \param port
 * Which IO port does this pin(s) belong to.
 * Can be either \b 0 or \b 1.
 *
 * \param pin_mask
 * Selects one or more pins whose level has to be changed.
 *
 * \param level
 * Selects whether pin should be set high (\b 1) or low (\b 0).
 *
 * \returns
 * \b HAL_OK - On success.<br>
 * \b HAL_E_* - Error code on failure.
 */
extern int32_t PCA9655E_SetOutputLevel(PCA9655E_Type *obj, uint8_t port,
        uint8_t pin_mask, bool level);

#endif /* BDK_PCA9655E_H_ */

//! \}
//! \}
//! \}

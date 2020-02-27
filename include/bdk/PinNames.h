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
//! \addtogroup BDK_GRP
//! \{
//!
//! \addtogroup TGT_GRP Target
//!
//! \brief Evaluation board specific definitions.
//!
//! \{
//!
//! \addtogroup TGT_RSL10_SENSE_GRP RSL10-SENSE-GEVB
//! \{
//-----------------------------------------------------------------------------

#ifndef PIN_NAMES_H_
#define PIN_NAMES_H_

/** \brief Pin mappings used by the HB-GEVB board.
 *
 * These DIO pad definitions are used by HAL drivers and board support libraries
 * when referring to GPIO pins.
 */
typedef enum {
    NC = -1,

    /** All available GPIO pins of RSL10. */
    PIN_DIO0 = 0,
    PIN_DIO1,
    PIN_DIO2,
    PIN_DIO3,
    PIN_DIO4,
    PIN_DIO5,
    PIN_DIO6,
    PIN_DIO7,
    PIN_DIO8,
    PIN_DIO9,
    PIN_DIO10,
    PIN_DIO11,
    PIN_DIO12,
    PIN_DIO13,
    PIN_DIO14,
    PIN_DIO15,

    /* I2C Peripheral pins */
    PIN_I2C_SCK = PIN_DIO5, /**< I2C Clock signal */
    PIN_I2C_SDA = PIN_DIO4, /**< I2C Data signal*/

    /* SPI Peripheral pins */
    PIN_SPI_SCK = NC, /**< NOT CONNECTED - SPI Clock signal */
    PIN_SPI_MOSI = NC, /**< NOT CONNECTED - SPI MOSI signal */
    PIN_SPI_MISO = NC, /**< NOT CONNECTED - SPI MISO signal */
    PIN_SPI_CS = NC, /**< NOT CONNECTED - SPI CS signal. */

    /* UART Peripheral pins */
    PIN_UART_TX = PIN_DIO3, /**< NOT CONNECTED - UART TX signal */
    PIN_UART_RX = PIN_DIO7, /**< NOT CONNECTED - UART RX signal */

    /* Board specific pins */
    PIN_RECOVERY = PIN_DIO12, /**< Pin used for deep sleep recovery in HAL. */
    PIN_BUTTON0 = PIN_DIO15, /**< On-board user button PB1 (PCB edge). */
	PIN_BUTTON1 = PIN_DIO12, /**< On-board user button PB2 (Board center). */

    PIN_LED_RED = PIN_DIO2,
	PIN_LED_GREEN = PIN_DIO1,
	PIN_LED_BLUE = PIN_DIO0,

	PIN_GIO_SPARE = PIN_DIO3, /**< GPIO pin for expansion header. */
	PIN_INT_NOA1305 = PIN_DIO13, /**< IRQ signal from on-board NOA1305 ALS. */
	PIN_INT_BHI160 = PIN_DIO9, /**< IRQ signal from on-board BHI160 sensor. */

    /* ERRATA: CLK and DIO pins are swapped on mic pads */
	PIN_MEMS_CLK = PIN_DIO10, /**< Clock for on-board MEMS microphone. */
	PIN_MEMS_DOUT = PIN_DIO6 /**< Data input from on-board MEMS microphone. */
} PinName;

#endif /* PIN_NAMES_H_ */

//! \}
//! \}
//! \}

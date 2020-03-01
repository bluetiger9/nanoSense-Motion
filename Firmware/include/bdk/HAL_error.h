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
//! \file HAL_error.h
//!
//! Contains error code definitions of all HAL functions and ASSERT macros
//! for handling of non recoverable error states.
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//-----------------------------------------------------------------------------

#ifndef HAL_ERROR_H_
#define HAL_ERROR_H_


#ifdef __cplusplus
extern "C" {
#endif


#define HAL_OK                           (0)
#define HAL_ERROR                        (-1)
#define HAL_ERROR_BUSY                   (-2)
#define HAL_ERROR_TIMEOUT                (-3)
#define HAL_ERROR_UNSUPPORTED            (-4)
#define HAL_ERROR_PARAMETER              (-5)
#define HAL_ERROR_SPECIFIC               (-6)
#define HAL_ERROR_I2C_NACK               (-7)
#define HAL_ERROR_I2C_ARBITRATION_LOST   (-8)
#define HAL_ERROR_I2C_BUS_ERROR          (-9)
#define HAL_ERROR_SPI_DATA_LOST          (-10)

/** \brief This macro evaluates given expression (condition) and if it evaluates
 * to false it stops program execution.
 *
 * This macro will always be evaluated regardless of optimization settings.
 */
#define ASSERT_ALWAYS(expr) \
	if (expr) {} \
	else { HAL_Failed(__FILE__, __LINE__, #expr); }

/** \brief This macro evaluates given expression (condition) and if it evaluates
 * to false it stops program execution.
 *
 * This macro will always be evaluated only if debugging is enabled.
 */
#ifndef NDEBUG
#define ASSERT_DEBUG(expr) \
	if (expr) {} \
	else { HAL_Failed(__FILE__, __LINE__, #expr); }
#else
#define ASSERT_DEBUG(expr)
#endif /* NDEBUG */


/** \brief This function is called if an assertion fails and program enters
 * unrecoverable state.
 *
 * This function is called if an assertion condition from ASSERT_DEBUG or
 * ASSERT_ALWAYS macros evaluates to false.
 *
 * Failure is indicated by periodically blinking one of the on board LEDs every
 * second.
 * LED identified by the name of LED_FAILURE will be used.
 *
 * Additionally information about the point of failure is printed if SEGGER RTT
 * output redirection is used it will print it to the RTT Console.
 * The following message format is used in these messages:
 * \code
 * BDK assertion failed: $expr, file: $file, line: $line \r\n
 * \endcode
 *
 * \param file
 * Path to the source file from which this function is called.
 * Can be obtained by using the `__FILE__` macro.
 *
 * \param line
 * Line number where the error occured.
 * Can be obtained by using the `__LINE__` macro.
 *
 * \param expr
 * String containing the failed assertion expression or any message describing the error.
 *
 * \returns
 * This function will not return.
 */
extern void HAL_Failed(const char* file, int line, const char* expr) __attribute__ ((noreturn));


#ifdef __cplusplus
}
#endif


#endif /* HAL_ERROR_H_ */

//! }
//! }

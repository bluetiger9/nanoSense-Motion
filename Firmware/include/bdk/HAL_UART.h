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
//! \file HAL_UART.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup HAL_GRP
//! \{
//! \addtogroup UART_GRP UART
//!
//! \brief UART interface for communication with connected shields.
//!
//! Contains wrapper functions for USART CMSIS Driver that allow both
//! synchronous and asynchronous operation of the peripheral in UART mode.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Default baud rate used by UART driver.
 *
 * \see HAL_UART_SetBaudRate to change baud rate to different speeds.
 */
#define HAL_UART_DEFAULT_BAUDRATE      115200

/** \brief Timeout for synchronous receive operation to wait for characters
 * forever.
 */
#define HAL_UART_WAIT_FOREVER          0

/** \brief User defined callback that will be called after asynchronous
 * receive / transmit operation finishes.
 *
 * \param[in] data
 * Pointer to data array that was passed to the original async call.
 *
 * \param[in] len
 * Total amount of characters that was successfully sent / received.
 */
typedef void (*HAL_UART_AsyncCallback)(char* data, uint32_t len);

/** \brief Prepares UART peripheral for use.
 *
 * Configures GPIO pins used by UART peripheral.
 * The default configuration for UART is:
 *   * Baud rate: 115200
 *   * Data bits: 8
 *   * Stop bits: 1
 *   * Parity: None
 *   * Flow control: None
 *
 * From the above parameters only change of baud rate is supported by RSL10
 * UART driver.
 *
 * \note This function is called automatically from all HAL_UART_* calls.
 * Therefore it is not required to call it explicitly.
 *
 * \see HAL_Pinmap.h
 * Header for used TX & RX GPIO pin configuration.
 */
extern void HAL_UART_Init(void);

/** \brief Powers down UART peripheral and disables RX & TX pins.
 *
 * This function will both disable the UART peripheral and disconnect RX &
 * TX pins.
 *
 * Baud rate configuration is preserved after power down and will be reused
 * for next initialization.
 *
 * \warning If called while peripheral is busy it will abort all ongoing
 * RX /TX operations.
 */
extern void HAL_UART_DeInit(void);

/** \brief Allows to set baud rate of UART communication.
 *
 * Baud rate setting is preserved between UART peripheral initialization
 * cycles.
 *
 * \param baud
 * New baud rate to be used.
 *
 * \returns
 * HAL_OK - On success. Baud rate was changed.<br>
 * HAL_ERROR_BUSY - If UART peripheral is busy. Baud rate is not changed.
 * HAL_ERROR_* - On other errors. Baud rate is not changed.
 *
 * \see HAL_UART_DEFAULT_BAUDRATE
 * For default value used if this function is not called.
 *
 * \see HAL_UART_GetBaudRate
 */
extern int32_t HAL_UART_SetBaudRate(uint32_t baud);

/** \brief Returns currently configured baud rate.
 *
 * \returns
 * Current baud rate configuration.
 *
 * \see HAL_UART_SetBaudRate
 */
extern uint32_t HAL_UART_GetBaudRate(void);

/** \brief Returns whether UART peripheral is busy transmitting or waiting for
 *  data to be received.
 *
 *  \returns
 *  true - If either transmit or receive operation is in progress.<br>
 *  false - If no operation is in progress.
 */
extern bool HAL_UART_IsBusy(void);

/** \brief Waits for data to be received over UART.
 *
 * This function is blocking until either requested number of bytes is
 * received or timeout is reached.
 *
 * \pre This function cannot be called from an interrupt service routine.
 *
 * \param[out] data
 * Poiter to memory address where received bytes will be stored.
 *
 * \param[in] num
 * Maximum number of bytes to receive.
 *
 * \param[in] timeout
 * Maximum time to wait for new data in milliseconds or
 * HAL_UART_WAIT_FOREVER to disable timeout.
 *
 * \returns
 * On succes number of bytes stored in provided data buffer is returned.
 * If timeout was reached number of read bytes will be smaller than \p num.<br>
 * HAL_ERROR_* error code on failure.
 */
extern int32_t HAL_UART_Receive(char *data, uint32_t num, uint32_t timeout);

/** \brief Transmits data over UART.
 *
 * This function is blocking until all data bytes were passed to UART
 * peripheral.
 *
 * \pre This function cannot be called from an interrupt service routine.
 *
 * \param[in] data
 * Pointer to beginning of data to be sent.
 * \param[in] num
 * Number of bytes to be sent.
 * \returns
 * HAL_OK - On succes. All bytes were successfully transmitted.<br>
 * HAL_ERROR_* - On error.
 */
extern int32_t HAL_UART_Send(const char *data, uint32_t num);

/** \brief Starts listening for data on UART and calls application callback
 * once input buffer is full.
 *
 * This function is not blocking and can be called from interrupt handler e.g.
 * from provided callback function.
 *
 * \param data
 * Pointer to memory address where received bytes will be stored.
 * \param num
 * Number of bytes to receive.
 * \param cb
 * Callback to be called once input buffer is filled with received data.
 *
 * <b>Example:</b><br>
 * This example asynchronously receives N characters and then uses scheduler to
 * echo them back.
 * \include hal_receive_async_example.c
 *
 * \returns
 * HAL_OK - On success. UART will store all incoming bytes in data array.<br>
 * HAL_ERROR_BUSY - When another receive operation is already in progress.<br>
 * HAL_ERROR_* - On other errors.
 */
extern int32_t HAL_UART_ReceiveAsync(char *data, uint32_t num,
        HAL_UART_AsyncCallback cb);

/** \brief Aborts currently ongoing receive operation.
 *
 * Can be used to abort receive operation started by \ref HAL_UART_ReceiveAsync
 * .
 *
 * \param execute_cb
 * Whether application callback passed when asynchronous receive was started
 * should be executed.
 *
 * \returns
 * Number of bytes received since the asynchronous receive operation started.<br>
 * HAL_ERROR_SPECIFIC - When no receive operation was in progress.
 * */
extern int32_t HAL_UART_ReceiveAsyncAbort(bool execute_cb);

/** \brief Start transmit operation and return control to application.
 *
 * This function in not blocking.
 * Data are transmitted asynchronously without application waiting until
 * transfer is finished.
 * Application can set up custom callback function to be executed once transmit
 * is completed.
 *
 * <b>Example:</b><br>
 * Dynamically allocated memory is used to transmit data and then automatically
 * free'd when transfer completes.
 * \include hal_send_async_example.c
 *
 * \param data
 * Pointer to beginning of data to be sent.
 * This data has to remain valid for the whole duration of transmit operation.
 * \param num
 * Number of bytes to be transmitted.
 * \param cb
 * Application callback that will be called once transfer operation is
 * finished.
 *
 * \return
 * HAL_OK - On success. Transmit operation was started. <br>
 * HAL_ERROR_BUSY - When another transmit operation is already in progress.<br>
 * HAL_ERROR_* - On other errors.
 */
extern int32_t HAL_UART_SendAsync(const char *data, uint32_t num,
        HAL_UART_AsyncCallback cb);

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H_ */

//! \}
//! \}

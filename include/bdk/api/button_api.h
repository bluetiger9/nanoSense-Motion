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
//! \file button_api.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup API
//! \{
//! \addtogroup BTN_API Button
//!
//! \brief API for receiving input from on-board buttons.
//!
//! This API is implemented by the Board Support Package (BSP) to provide basic
//! functionality for all on-board buttons.
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef BUTTON_API_H_
#define BUTTON_API_H_

#include <stdbool.h>
#include <stdint.h>

// Board specific Button names provided by evaluation board BSP
#include <ButtonNames.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define BTN_PRESSED                    (true)
#define BTN_RELEASED                   (false)

/** \brief List of possible button state change events. */
enum BTN_Edge
{
    /** \brief Can be used to disable interrupt generation from push button. */
    BTN_EVENT_NONE,

    /** \brief Interrupt will be generated only when button is pressed. */
    BTN_EVENT_PRESSED,

    /** \brief Interrupt will be generated only when button is released. */
    BTN_EVENT_RELEASED,

    /** \brief Interrupt will be generated when button changes state. */
    BTN_EVENT_TRANSITION
};

/** \brief Function prototype for */
typedef void (*BTN_Callback)(void *arg);

/** \brief Initializes the DIO pad of selected button.
 *
 * \note
 * Calling any other BTN_* functions before BTN_Initialize will result in
 * undefined behavior.
 */
extern void BTN_Initialize(ButtonName btn);

/** \brief Returns current state of given button.
 *
 * This function is safe to call from ISR.
 *
 * \returns
 * \b BTN_PRESSED - When given button is pressed.<br>
 * \b BTN_RELEASED - When button is not pressed.
 */
extern bool BTN_Read(ButtonName btn);

/** Returns `true` if it is possible to attach interrupt to this button.
 *
 * The RSL10 has only 4 interrupt sources for all 16 GPIO pads.
 * As such the implementer of this API will provide an RTE configuration file
 * where the user can determine which interrupt sources to use for which button.
 * This way it is possible to assign the interrupt source to different
 * functionality if not required by buttons.
 *
 * \returns
 * \b true - An interrupt can be attached to this button.<br>
 * \b false - Interrupt cannot be assigned to this button.
 */
extern bool BTN_IsIntCapable(ButtonName btn);

/** \brief Attaches and interrupt handler to given button that will be executed
 * directly in the ISR.
 *
 * \param edge
 * Determines what kind of event will trigger the attaches callback.
 * Use one of BTN_EVENT_[ NONE | PRESSED | RELEASED | TRANSITION ]
 *
 * \param cb
 * Callbacks function that will be called when selected button event occurs.
 *
 * \param arg
 * Optional argument to pass to the callback function.
 * NULL if no argument is provided.
 *
 * \param btn
 * The name of button.
 *
 * \returns
 * \b 0 - On success.<br>
 * \b -1 - On failure.<br>
 * \b -2 - If button does not have interrupt capability enabled.
 */
extern int32_t BTN_AttachInt(enum BTN_Edge edge, BTN_Callback cb, void *arg,
        ButtonName btn);

/** \brief Attaches and interrupt handler to given button that will be executed
 * by Event Kernel.
 *
 * Event Kernel events are executed from main loop when BDK_Schedule is called.
 *
 * \param edge
 * Determines what kind of event will trigger the attaches callback.
 * Use one of BTN_EVENT_[ NONE | PRESSED | RELEASED | TRANSITION ]
 *
 * \param cb
 * Callbacks function that will be called when selected button event occurs.
 *
 * \param arg
 * Optional argument to pass to the callback function.
 * NULL if no argument is provided.
 *
 * \param btn
 * The name of button.
 *
 * \returns
 * \b 0 - On success.<br>
 * \b -1 - On failure.<br>
 * \b -2 - If button does not have interrupt capability enabled.
 */
extern int32_t BTN_AttachScheduled(enum BTN_Edge edge, BTN_Callback cb, void *arg,
        ButtonName btn);


#ifdef __cplusplus
}
#endif

#endif /* BUTTON_API_H_ */

//! \}
//! \}
//! \}

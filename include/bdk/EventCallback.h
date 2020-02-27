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
//! \file EventCallback.h
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup EVENT_CALLBACK_GRP Event Callback
//!
//! \brief Library for attaching multiple callback functions (listeners) to
//! single event source.
//!
//! This simple library allows to attach multiple callback functions to single
//! or multiple event sources.
//!
//! The event callbacks are stored in a linked list in the same order they were
//! registered.
//! Each event callback contains information about event ID it belongs to, the
//! callback itself and an optional parameter to pass to the callback.
//!
//! Event ID can be arbitrary 16-bit unsigned number.
//!
//! Example how to set up event callback (listener):
//! \code
//! EventCallback_Type my_listener;
//!
//! EventCallback_Init(&my_listener, 3456, &my_callback, &my_argument);
//! EventCallback_Register(&my_listener);
//! \endcode
//!
//! Example how to execute all event callback registered for given event ID:
//! \code
//! EventCallback_Execute(3456);
//! \endcode
//!
//! \{
//-----------------------------------------------------------------------------

#ifndef EVENT_CALLBACK_H_
#define EVENT_CALLBACK_H_

#include <stdbool.h>
#include <stdint.h>

#include <HAL.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \brief Prototype for event callback functions. */
typedef void (*EventCallback_Prototype)(void *arg);

/** \brief Contains information about registered callback.
 *
 * \warning This is an internal structure and should not be modified in any way
 * without the use of EventCallback_* API.
 */
struct _EventCallback_Struct
{
    /** \brief Pointer to function that will be called if event with matching
     * event_id occurs.
     */
    EventCallback_Prototype callback;

    /** \brief Pointer to next registered event callback handler. */
    struct _EventCallback_Struct *next;

    /** \brief Optional application specific argument that will be passed to
     * event callback function.
     */
    void *arg;

    /** \brief Event id identifying to which event this handler belongs.
     *
     * There can be multiple handlers for the same event id.
     *
     * \see EventCallback_Call
     */
    uint16_t event_id;
};

typedef struct _EventCallback_Struct EventCallback_Type;

/** \brief Initializes callback structure for use.
 *
 * \param handle
 * Pointer to event callback handle that will be initialized.
 *
 * \param event_id
 * ID of event that this handle belongs to.
 *
 * \param callback
 * Callback function that will be executed when EventCallback_Call is called
 * with matching event_id.
 *
 * \param arg
 * Optional argument that will be passed to callback function.
 *
 */
extern void EventCallback_Init(EventCallback_Type *handle, uint16_t event_id,
        EventCallback_Prototype callback, void *arg);

/** \brief Inserts given event handle to the end of linked list composed of all
 * registered event handles.
 *
 * Caller is responsible for memory management of given event handle and
 * ensures that it wont be deallocated while handle is registered.
 */
extern void EventCallback_Register(EventCallback_Type *handle);

/** \brief Removes given handle from list of registered event handlers.
 *
 * Caller is responsible for memory management of given handle.
 */
extern void EventCallback_Remove(EventCallback_Type *handle);

/** \brief Execute all registered callbacks whose event id matches.
 *
 * \param event_id
 * ID of event that has occurred.
 * If there are any registered event callbacks with matching ID they will be
 * called from within this function in the order they were registered.
 */
extern void EventCallback_Call(uint16_t event_id);

#ifdef __cplusplus
}
#endif

#endif /* EVENT_CALLBACK_H_ */

//! \}
//! \}

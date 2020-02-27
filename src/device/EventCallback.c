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
//! \file EventCallback.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup EVENT_CALLBACK_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "EventCallback.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static EventCallback_Type *head_handle = NULL;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void EventCallback_Init(EventCallback_Type *handle, uint16_t event_id,
        EventCallback_Prototype callback, void *arg)
{
    ASSERT_DEBUG(handle != NULL && callback != NULL);

    handle->event_id = event_id;
    handle->callback = callback;
    handle->arg = arg;
    handle->next = NULL;
}

void EventCallback_Register(EventCallback_Type *handle)
{
    ASSERT_DEBUG(handle != NULL);

    if (head_handle == NULL)
    {
        head_handle = handle;
    }
    else
    {
        EventCallback_Type *h = head_handle;
        while (h->next != NULL)
        {
            h = h->next;
        }
        h->next = handle;
    }
}

void EventCallback_Remove(EventCallback_Type *handle)
{
    ASSERT_DEBUG(handle != NULL);

    if (handle == head_handle)
    {
        // Remove handle from list if it was first registered handle.
        head_handle = head_handle->next;
    }
    else
    {
        if (head_handle != NULL)
        {
            EventCallback_Type *h = head_handle;

            // Find handle taht is before the handle to be removed.
            while (h->next != handle)
            {
                if (h->next == NULL)
                {
                    // This handle was not registered.
                    return;
                }
                // Move to next handle.
                h = h->next;
            }

            // Remove handle from list.
            h->next = handle->next;

            // Erase any pointer to other handlers from removed handle.
            handle->next = NULL;
        }
    }
}

void EventCallback_Call(uint16_t event_id)
{
    EventCallback_Type *h;

    // Iterate over all registered event handles.
    for (h = head_handle; h != NULL; h = h->next)
    {
        if (h->event_id == event_id && h->callback != NULL)
        {
            h->callback(h->arg);
        }
    }
}

//! \}
//! \}

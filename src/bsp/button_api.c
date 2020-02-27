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
//!
//! Implementation of LED API for HB-GEVB evaluation board.
//!
//-----------------------------------------------------------------------------

#include <BDK.h>
#include <EventCallback.h>
#include <api/button_api.h>

#include <RTE_HB_Button.h>

#if RTE_BUTTON_HB_BTN0_INT_ENABLE == 1

#if RTE_BUTTON_HB_BTN0_INT_SRC == 0
#define BTN0_IRQn                      (DIO0_IRQn)
#define BTN0_ISR                       DIO0_IRQHandler
#elif RTE_BUTTON_HB_BTN0_INT_SRC == 1
#define BTN0_IRQn                      (DIO1_IRQn)
#define BTN0_ISR                       DIO1_IRQHandler
#elif RTE_BUTTON_HB_BTN0_INT_SRC == 2
#define BTN0_IRQn                      (DIO2_IRQn)
#define BTN0_ISR                       DIO2_IRQHandler
#elif RTE_BUTTON_HB_BTN0_INT_SRC == 3
#define BTN0_IRQn                      (DIO3_IRQn)
#define BTN0_ISR                       DIO3_IRQHandler
#endif

#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */


#if RTE_BUTTON_HB_BTN1_INT_ENABLE == 1

#if RTE_BUTTON_HB_BTN1_INT_SRC == 0
#define BTN1_IRQn                      (DIO0_IRQn)
#define BTN1_ISR                       DIO0_IRQHandler
#elif RTE_BUTTON_HB_BTN1_INT_SRC == 1
#define BTN1_IRQn                      (DIO1_IRQn)
#define BTN1_ISR                       DIO1_IRQHandler
#elif RTE_BUTTON_HB_BTN1_INT_SRC == 2
#define BTN1_IRQn                      (DIO2_IRQn)
#define BTN1_ISR                       DIO2_IRQHandler
#elif RTE_BUTTON_HB_BTN1_INT_SRC == 3
#define BTN1_IRQn                      (DIO3_IRQn)
#define BTN1_ISR                       DIO3_IRQHandler
#endif

#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */


struct BTN_ButtonResources {
    EventCallback_Type event;
    BTN_Callback cb;
    void *arg;
    bool scheduled;
};

static struct BTN_ButtonResources _btn[2] = { 0 };

static void BTN_EventHandler(struct BTN_ButtonResources *btn_res)
{
    if (btn_res->cb != NULL)
    {
        if (btn_res->scheduled == true)
        {
            BDK_TaskSchedule((BDK_TaskCallback) btn_res->cb, btn_res->arg);
        }
        else
        {
            btn_res->cb(btn_res->arg);
        }
    }
}

static void BTN_InitializeInt(enum BTN_Edge edge, uint32_t dio_pad,
        int int_index)
{
    uint32_t int_event = DIO_EVENT_NONE;

    switch (edge)
    {
    default:
        break;
    case BTN_EVENT_PRESSED:
        int_event = DIO_EVENT_FALLING_EDGE;
        break;
    case BTN_EVENT_RELEASED:
        int_event = DIO_EVENT_RISING_EDGE;
        break;
    case BTN_EVENT_TRANSITION:
        int_event = DIO_EVENT_TRANSITION;
        break;
    }

    Sys_DIO_IntConfig(int_index, dio_pad | int_event | DIO_DEBOUNCE_DISABLE,
            DIO_DEBOUNCE_SLOWCLK_DIV32, 0);
}

#if RTE_BUTTON_HB_BTN0_INT_ENABLE == 1
void BTN0_ISR(void)
{
    EventCallback_Call(RTE_BUTTON_HB_BTN0_EVENT_ID);
}
#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */

#if RTE_BUTTON_HB_BTN1_INT_ENABLE == 1
void BTN1_ISR(void)
{
    EventCallback_Call(RTE_BUTTON_HB_BTN1_EVENT_ID);
}
#endif /* RTE_BUTTON_HB_BTN1_INT_ENABLE == 1 */

void BTN_Initialize(ButtonName btn)
{
    if (btn == BTN0 || btn == BTN1)
    {
        int index = (btn == BTN0) ? 0 : 1;

        EventCallback_Remove(&_btn[index].event);
        memset(&_btn[index], 0, sizeof(struct BTN_ButtonResources));

        Sys_DIO_Config(btn, DIO_NO_PULL | DIO_LPF_ENABLE | DIO_MODE_GPIO_IN_0);
    }
}

bool BTN_Read(ButtonName btn)
{
    bool retval = BTN_RELEASED;

    if (btn == BTN0 && (DIO->DATA & (1 << BTN0)) == 0)
    {
        retval = BTN_PRESSED;
    }
    else
    {
        if (btn == BTN1 && (DIO->DATA & (1 << BTN1)) == 0)
        {
            retval = BTN_PRESSED;
        }
    }

    return retval;
}

bool BTN_IsIntCapable(ButtonName btn)
{
    switch (btn)
    {
    case BTN0:
#if RTE_BUTTON_HB_BTN0_INT_ENABLE == 1
        return true;
#else
        return false;
#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */

    case BTN1:
#if RTE_BUTTON_HB_BTN1_INT_ENABLE == 1
        return true;
#else
        return false;
#endif /* RTE_BUTTON_HB_BTN1_INT_ENABLE == 1 */
    default:
        break;
    }

    return false;
}

int32_t BTN_AttachInt(enum BTN_Edge edge, BTN_Callback cb, void *arg,
        ButtonName btn)
{
#if RTE_BUTTON_HB_BTN0_INT_ENABLE == 1
    if (btn == BTN0)
    {
        NVIC_DisableIRQ(BTN0_IRQn);
        EventCallback_Remove(&_btn[0].event);

        _btn[0].cb = cb;
        _btn[0].arg = arg;
        _btn[0].scheduled = false;

        if (cb != NULL)
        {
            BTN_InitializeInt(edge, BTN0, RTE_BUTTON_HB_BTN0_INT_SRC);

            EventCallback_Init(&_btn[0].event, RTE_BUTTON_HB_BTN0_EVENT_ID,
                    (EventCallback_Prototype) &BTN_EventHandler, &_btn[0]);
            EventCallback_Register(&_btn[0].event);

            NVIC_ClearPendingIRQ(BTN0_IRQn);
            NVIC_EnableIRQ(BTN0_IRQn);
        }

        return 0;
    }
#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */

#if RTE_BUTTON_HB_BTN1_INT_ENABLE == 1
    if (btn == BTN1)
    {
        NVIC_DisableIRQ(BTN1_IRQn);
        EventCallback_Remove(&_btn[1].event);

        _btn[1].cb = cb;
        _btn[1].arg = arg;
        _btn[1].scheduled = false;

        if (cb != NULL)
        {
            BTN_InitializeInt(edge, BTN1, RTE_BUTTON_HB_BTN1_INT_SRC);

            EventCallback_Init(&_btn[1].event, RTE_BUTTON_HB_BTN1_EVENT_ID,
                    (EventCallback_Prototype) &BTN_EventHandler, &_btn[1]);
            EventCallback_Register(&_btn[1].event);

            NVIC_ClearPendingIRQ(BTN1_IRQn);
            NVIC_EnableIRQ(BTN1_IRQn);
        }

        return 0;
    }
#endif /* RTE_BUTTON_HB_BTN1_INT_ENABLE == 1 */

    return -2;
}

int32_t BTN_AttachScheduled(enum BTN_Edge edge, BTN_Callback cb, void *arg,
        ButtonName btn)
{
#if RTE_BUTTON_HB_BTN0_INT_ENABLE == 1
    if (btn == BTN0)
    {
        NVIC_DisableIRQ(BTN0_IRQn);
        EventCallback_Remove(&_btn[0].event);

        _btn[0].cb = cb;
        _btn[0].arg = arg;
        _btn[0].scheduled = true;

        if (cb != NULL)
        {
            BTN_InitializeInt(edge, BTN0, RTE_BUTTON_HB_BTN0_INT_SRC);

            EventCallback_Init(&_btn[0].event, RTE_BUTTON_HB_BTN0_EVENT_ID,
                    (EventCallback_Prototype) &BTN_EventHandler, &_btn[0]);
            EventCallback_Register(&_btn[0].event);

            NVIC_ClearPendingIRQ(BTN0_IRQn);
            NVIC_EnableIRQ(BTN0_IRQn);
        }

        return 0;
    }
#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */

#if RTE_BUTTON_HB_BTN1_INT_ENABLE == 1
    if (btn == BTN1)
    {
        NVIC_DisableIRQ(BTN1_IRQn);
        EventCallback_Remove(&_btn[1].event);

        _btn[1].cb = cb;
        _btn[1].arg = arg;
        _btn[1].scheduled = true;

        if (cb != NULL)
        {
            BTN_InitializeInt(edge, BTN1, RTE_BUTTON_HB_BTN1_INT_SRC);

            EventCallback_Init(&_btn[1].event, RTE_BUTTON_HB_BTN1_EVENT_ID,
                    (EventCallback_Prototype) &BTN_EventHandler, &_btn[1]);
            EventCallback_Register(&_btn[1].event);

            NVIC_ClearPendingIRQ(BTN1_IRQn);
            NVIC_EnableIRQ(BTN1_IRQn);
        }

        return 0;
    }
#endif /* RTE_BUTTON_HB_BTN0_INT_ENABLE == 1 */

    return -2;
}

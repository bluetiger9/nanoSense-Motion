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
//! \file BDK_Task.c
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup SCHEDULING_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "BDK_Task.h"
#include "HAL.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

enum BDK_TaskRunState
{
    BDK_TASK_STATE_RESET = 0,
    BDK_TASK_STATE_INITIALIZED,
    BDK_TASK_STATE_RUNNING
};

struct BDK_TimerCmd
{
    BDK_TaskCallback cb;
    void *arg;
};

struct BDK_Task_Resources
{
    uint8_t run_state;
    uint16_t allocated_msg_idx;

    ke_state_t task_state;
    struct ke_msg_handler task_msg_table[RTE_APP_TASK_HANDLER_COUNT];
    struct ke_state_handler task_state_handle;
    struct ke_task_desc task_handle;
};

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static int BDK_DefaultMsgHandler(ke_msg_id_t const msg_id, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id);

static int BDK_TimerMsgHandler(ke_msg_id_t const msg_id, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

struct BDK_Task_Resources task_res = { 0 };

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void BDK_TaskInit(void)
{
    if (task_res.run_state == BDK_TASK_STATE_RESET)
    {
        memset(&task_res, 0, sizeof(task_res));

        task_res.task_state_handle.msg_table = task_res.task_msg_table;
        task_res.task_state_handle.msg_cnt = 0;

        task_res.task_handle.state_handler = NULL;
        task_res.task_handle.default_handler = &task_res.task_state_handle;
        task_res.task_handle.state = &task_res.task_state;
        task_res.task_handle.state_max = 1;
        task_res.task_handle.idx_max = 1;

        task_res.run_state = BDK_TASK_STATE_INITIALIZED;

        BDK_TaskAddMsgHandler(KE_MSG_DEFAULT_HANDLER, &BDK_DefaultMsgHandler);
        BDK_TaskAddMsgHandler(BDK_SCHEDULE_MSG, &BDK_TimerMsgHandler);
    }
}

void BDK_TaskAddMsgHandler(ke_msg_id_t id, ke_msg_func_t func)
{
    // Message handlers can be added only once task structures were initialized
    // but task is not running yet.
    ASSERT_DEBUG(task_res.run_state == BDK_TASK_STATE_INITIALIZED);

    // If this fails there are not enough handler slots for this application.
    // BDK_TASK_MAX_HANDLER_COUNT needs to be increased and BDK library rebuild.
    ASSERT_DEBUG(task_res.task_state_handle.msg_cnt < RTE_APP_TASK_HANDLER_COUNT);

    task_res.task_msg_table[task_res.task_state_handle.msg_cnt].id = id;
    task_res.task_msg_table[task_res.task_state_handle.msg_cnt].func = func;
    task_res.task_state_handle.msg_cnt += 1;
}

void BDK_TaskStart(void)
{
    if (task_res.run_state == BDK_TASK_STATE_INITIALIZED)
    {
        uint8_t status;

        status = ke_task_create(TASK_APP, &task_res.task_handle);
        ASSERT_DEBUG(status == KE_TASK_OK);

        task_res.run_state = BDK_TASK_STATE_RUNNING;
    }
}

ke_msg_id_t BDK_TaskAllocateMsgId(void)
{
    ke_msg_id_t id;

    id = BDK_TASK_FIRST_CUSTOM_MSG + task_res.allocated_msg_idx;
    task_res.allocated_msg_idx += 1;

    return id;
}

void BDK_TaskSchedule(BDK_TaskCallback cb, void *arg)
{
    struct BDK_TimerCmd *cmd;

    if (task_res.run_state >= BDK_TASK_STATE_INITIALIZED)
    {
        cmd = KE_MSG_ALLOC_DYN(BDK_SCHEDULE_MSG, TASK_APP, TASK_APP, BDK_TimerCmd, 0);

        cmd->cb = cb;
        cmd->arg = arg;

        ke_msg_send(cmd);
    }
}

static int BDK_DefaultMsgHandler(ke_msg_id_t const msg_id, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return KE_MSG_CONSUMED;
}

static int BDK_TimerMsgHandler(ke_msg_id_t const msg_id, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    const struct BDK_TimerCmd *cmd = param;

    if (cmd->cb != NULL)
    {
        cmd->cb(cmd->arg);
    }

    return KE_MSG_CONSUMED;
}

//! \}
//! \}

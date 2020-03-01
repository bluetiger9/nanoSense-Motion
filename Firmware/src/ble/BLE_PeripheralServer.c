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
//! \file BLE_PeripheralServer.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup BLE_GRP
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <BLE_PeripheralServer.h>
#include <HAL_error.h>
#include <BDK_Task.h>
#include <rsl10.h>
#include <rsl10_ke.h>
#include <rsl10_ble.h>
#include <rsl10_profiles.h>
#include <rsl10_protocol.h>
#include <rsl10_hw_cid101.h>

#include "app_trace.h"
#include "app_ble_hooks.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

enum BLE_State
{
    BLE_STATE_OFF,
    BLE_STATE_INIT,
    BLE_STATE_CREATE_DB,
    BLE_STATE_READY,
    BLE_STATE_ADVERTISING,
    BLE_STATE_CONNECTED,
    BLE_STATE_MAX
};

struct BLE_Resources
{
    enum BLE_State state;

    uint8_t local_name[BDK_BLE_LOCAL_NAME_MAX_LENGTH];
    uint8_t local_name_len;

    uint8_t manu_data[BDK_BLE_MANUF_DATA_MAX_LENGTH];
    uint8_t manu_data_len;

    struct bd_addr baddr;
    uint8_t baddr_type;
    uint16_t adv_int_min;
    uint16_t adv_int_max;

    uint16_t conhdl; /**< Connection handle */
    uint8_t conidx; /**< Connection index */

    BDK_BLE_SVC_AddFunc svc_add_func[BDK_BLE_SVC_MAX];
    BDK_BLE_SVC_EnableFunc svc_enable_func[BDK_BLE_SVC_MAX];
    uint8_t svc_add_index;
    uint8_t svc_count;
};

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

void Device_Param_Prepare(app_device_param_t *param);

static int GAPM_ProfileAddedInd(  ke_msg_id_t const msg_id, struct gapm_profile_added_ind const *param,    ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPM_CmpEvt(           ke_msg_id_t const msg_id, struct gapm_cmp_evt const *param,              ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPC_GetDevInfoReqInd( ke_msg_id_t const msg_id, struct gapc_get_dev_info_req_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPC_ConnectionReqInd( ke_msg_id_t const msg_id, struct gapc_connection_req_ind const *param,   ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPC_CmpEvt(           ke_msg_id_t const msg_id, struct gapc_cmp_evt const *param,              ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPC_DisconnectInd(    ke_msg_id_t const msg_id, struct gapc_disconnect_ind const *param,       ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPC_ParamUpdatedInd(  ke_msg_id_t const msg_id, struct gapc_param_updated_ind const *param,    ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int GAPC_ParamUpdateReqInd(ke_msg_id_t const msg_id, struct gapc_param_update_req_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

static bool BDK_BLE_ServiceAdd(void);
static void BDK_BLE_SendConnectionConfirmation(void);
static void BDK_BLE_SetServiceState(bool enable);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static struct BLE_Resources ble_env = {
    0
};

static struct gapm_set_dev_config_cmd *bdk_gapm_conf_cmd = NULL;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void BDK_BLE_Initialize(void)
{
    if (ble_env.state != BLE_STATE_OFF)
    {
        return;
    }

    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);
    __set_FAULTMASK(FAULTMASK_DISABLE_INTERRUPTS);

    memset(&ble_env, 0, sizeof(ble_env));
    ble_env.state = BLE_STATE_INIT;
    ble_env.adv_int_min = BDK_BLE_ADV_INT_DEFAULT;
    ble_env.adv_int_max = BDK_BLE_ADV_INT_DEFAULT;
    BDK_BLE_SetLocalName(BDK_BLE_DEFAULT_LOCAL_NAME);

    /* Add Bluetooth related message handlers to application task. */
    BDK_TaskInit();
    BDK_TaskAddMsgHandler(GAPM_CMP_EVT, (ke_msg_func_t)GAPM_CmpEvt);
    BDK_TaskAddMsgHandler(GAPM_PROFILE_ADDED_IND, (ke_msg_func_t)GAPM_ProfileAddedInd);
    BDK_TaskAddMsgHandler(GAPC_CONNECTION_REQ_IND, (ke_msg_func_t)GAPC_ConnectionReqInd);
    BDK_TaskAddMsgHandler(GAPC_CMP_EVT, (ke_msg_func_t)GAPC_CmpEvt);
    BDK_TaskAddMsgHandler(GAPC_DISCONNECT_IND, (ke_msg_func_t)GAPC_DisconnectInd);
    BDK_TaskAddMsgHandler(GAPC_GET_DEV_INFO_REQ_IND, (ke_msg_func_t)GAPC_GetDevInfoReqInd);
    BDK_TaskAddMsgHandler(GAPC_PARAM_UPDATED_IND, (ke_msg_func_t)GAPC_ParamUpdatedInd);
    BDK_TaskAddMsgHandler(GAPC_PARAM_UPDATE_REQ_IND, (ke_msg_func_t)GAPC_ParamUpdateReqInd);

    /* Initialize Bluetooth stack */
    BLE_InitNoTL(0);
    BLE_Reset();

    /* Enable the Bluetooth related interrupts needed */
    NVIC_EnableIRQ(BLE_EVENT_IRQn);
    NVIC_EnableIRQ(BLE_RX_IRQn);
    NVIC_EnableIRQ(BLE_CRYPT_IRQn);
    NVIC_EnableIRQ(BLE_ERROR_IRQn);
    NVIC_EnableIRQ(BLE_SW_IRQn);
    NVIC_EnableIRQ(BLE_GROSSTGTIM_IRQn);
    NVIC_EnableIRQ(BLE_FINETGTIM_IRQn);
    NVIC_EnableIRQ(BLE_CSCNT_IRQn);
    NVIC_EnableIRQ(BLE_SLP_IRQn);

#if BDK_BLE_BADDR_TYPE == BDK_BLE_BADDR_TYPE_PUBLIC
    /* Load device bluetooth address from non-volatile memory. */
    if (Device_Param_Read(PARAM_ID_PUBLIC_BLE_ADDRESS, ble_env.baddr.addr))
    {
        /* Do nothing else -> address was loaded from NVR3. */
    }
    else
    {
        /* Failed to load address -> use default one. */
        memcpy(ble_env.baddr.addr, &co_default_bdaddr, BDK_BLE_BADDR_LENGTH);
    }
    ble_env.baddr_type = GAPM_CFG_ADDR_PUBLIC;
#else
    /* Use predefined bluetooth address. */
    uint8_t default_addr[BDK_BLE_BADDR_LENGTH] = BDK_BLE_PRIVATE_BADDR;
    memcpy(ble_env.baddr.addr, default_addr, BDK_BLE_BADDR_LENGTH);
    ble_env.baddr_type = GAPM_CFG_ADDR_PRIVATE;
#endif /* BDK_BLE_BADDR_TYPE == BDK_BLE_BADDR_TYPE_PUBLIC */

    /* Initialize GAPM configuration command to initialize the stack */
    bdk_gapm_conf_cmd = malloc(sizeof(struct gapm_set_dev_config_cmd));
    ASSERT_DEBUG(bdk_gapm_conf_cmd != NULL);
    bdk_gapm_conf_cmd->operation = GAPM_SET_DEV_CONFIG;
    bdk_gapm_conf_cmd->role = GAP_ROLE_PERIPHERAL;
    memcpy(bdk_gapm_conf_cmd->addr.addr, ble_env.baddr.addr, BDK_BLE_BADDR_LENGTH);
    bdk_gapm_conf_cmd->addr_type = ble_env.baddr_type;
    bdk_gapm_conf_cmd->renew_dur = BDK_BLE_RENEW_DUR;
    memset(&bdk_gapm_conf_cmd->irk.key[0], 0, KEY_LEN);
    bdk_gapm_conf_cmd->pairing_mode = GAPM_PAIRING_DISABLE;
    bdk_gapm_conf_cmd->gap_start_hdl = 0;
    bdk_gapm_conf_cmd->gatt_start_hdl = 0;
    bdk_gapm_conf_cmd->max_mtu = BDK_BLE_MTU_MAX;
    bdk_gapm_conf_cmd->max_mps = BDK_BLE_MPS_MAX;
    bdk_gapm_conf_cmd->att_and_ext_cfg = BDK_BLE_ATT_CFG;
    bdk_gapm_conf_cmd->sugg_max_tx_octets = BDK_BLE_TX_OCT_MAX;
    bdk_gapm_conf_cmd->sugg_max_tx_time = BDK_BLE_TX_TIME_MAX;
    bdk_gapm_conf_cmd->tx_pref_rates = GAP_RATE_ANY;
    bdk_gapm_conf_cmd->rx_pref_rates = GAP_RATE_ANY;
    bdk_gapm_conf_cmd->max_nb_lecb = 0x0;
    bdk_gapm_conf_cmd->audio_cfg = 0;

    struct gapm_reset_cmd *cmd;

    /* Reset the stack */
    cmd = KE_MSG_ALLOC(GAPM_RESET_CMD, TASK_GAPM, TASK_APP, gapm_reset_cmd);
    cmd->operation = GAPM_RESET;

    /* Send the message */
    ke_msg_send(cmd);

    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
    __set_FAULTMASK(FAULTMASK_ENABLE_INTERRUPTS);
}

void BDK_BLE_SetLocalName(const char* name)
{
    uint32_t name_len;

    if (ble_env.state == BLE_STATE_OFF)
    {
        BDK_BLE_Initialize();
    }

    if (name != NULL)
    {
        name_len = strnlen(name, BDK_BLE_LOCAL_NAME_MAX_LENGTH + 1);
        if (name_len <= BDK_BLE_LOCAL_NAME_MAX_LENGTH)
        {
            memcpy(ble_env.local_name, name, name_len);
            ble_env.local_name_len = name_len;
        }
    }
}

void BDK_BLE_SetManufSpecificData(const uint8_t* data, uint32_t len)
{
    if (ble_env.state == BLE_STATE_OFF)
    {
        BDK_BLE_Initialize();
    }

    if (data != NULL && len <= BDK_BLE_MANUF_DATA_MAX_LENGTH)
    {
        memcpy(ble_env.manu_data, data, len);
        ble_env.manu_data_len = len;
    }
}

void BDK_BLE_SetAdvertisementInterval(uint16_t interval_min, uint16_t interval_max)
{
    if (ble_env.state == BLE_STATE_OFF)
    {
        BDK_BLE_Initialize();
    }

    ble_env.adv_int_min = interval_min;
    ble_env.adv_int_max = interval_max;
}

void BDK_BLE_AddService(void (*svc_add_func)(void), void (*svc_enable_func)(uint8_t))
{
    if (ble_env.state == BLE_STATE_OFF)
    {
        BDK_BLE_Initialize();
    }

    if (svc_add_func != NULL && svc_enable_func != NULL)
    {
        ble_env.svc_add_func[ble_env.svc_count] = svc_add_func;
        ble_env.svc_enable_func[ble_env.svc_count] = svc_enable_func;

        ble_env.svc_count += 1;
    }
}

void BDK_BLE_ProfileAddedInd(void)
{
    if (ble_env.state == BLE_STATE_CREATE_DB)
    {
        if (!BDK_BLE_ServiceAdd())
        {
            ble_env.state = BLE_STATE_READY;

            BDK_BLE_AdvertisingStart();
        }
    }
}


/* ----------------------------------------------------------------------------
 * Function      : Device_Param_Prepare(struct app_device_param *param)
 * ----------------------------------------------------------------------------
 * Description   : Configure the device related stack parameters
 * Inputs        : - param    - Pointer to the parameters to be configured
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Device_Param_Prepare(app_device_param_t *param)
{
    param->device_param_src_type = FLASH_PROVIDED_or_DFLT;

    BLE_DeviceParam_Set_ClockAccuracy(BDK_BLE_RADIO_CLOCK_ACCURACY);
    BLE_DeviceParam_Set_ADV_IFS(1300);
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPM_ProfileAddedInd(ke_msg_id_t const msg_id,
 *                                          struct gapm_profile_added_ind
 *                                          const *param,
 *                                          ke_task_id_t const dest_id,
 *                                          ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle the received result of adding a profile to the
 *                 attribute database
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapm_profile_added_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPM_ProfileAddedInd(ke_msg_id_t const msg_id, struct gapm_profile_added_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    BDK_BLE_ProfileAddedInd();

    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPM_CmpEvt(ke_msg_id_t const msg_id,
 *                                 struct gapm_cmp_evt
 *                                 const *param,
 *                                 ke_task_id_t const dest_id,
 *                                 ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle the reception of a GAPM complete event
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapm_cmp_evt
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPM_CmpEvt(ke_msg_id_t const msg_id, struct gapm_cmp_evt const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_set_dev_config_cmd *cmd;

    switch (param->operation)
    {
        /* A reset has occurred, configure the device and
         * start a kernel timer for the application */
        case (GAPM_RESET):
        {
            if (param->status == GAP_ERR_NO_ERROR)
            {
                /* Set the device configuration */
                cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD, TASK_GAPM, TASK_APP,
                                   gapm_set_dev_config_cmd);
                memcpy(cmd, bdk_gapm_conf_cmd,
                       sizeof(struct gapm_set_dev_config_cmd));
                free(bdk_gapm_conf_cmd);
                bdk_gapm_conf_cmd = NULL;

                /* Send message */
                ke_msg_send(cmd);
            }
        }
        break;

        /* Device configuration updated */
        case (GAPM_SET_DEV_CONFIG):
        {
            /* Start creating the GATT database */
            ble_env.state = BLE_STATE_CREATE_DB;

            /* Add the first required service in the database */
            if (!BDK_BLE_ServiceAdd())
            {
                /* If there are no more services to add, go to the ready
                 * state */
                ble_env.state = BLE_STATE_READY;

                /* Start advertising since there are no services to add
                 * to the attribute database */
                BDK_BLE_AdvertisingStart();
            }
        }
        break;

        /* Device started/stoped advertising */
        case GAPM_ADV_UNDIRECT:
            TRACE_PRINTF("operation=%d, status=%d\r\n", param->operation,
                    param->status);
            ASSERT_DEBUG(param->status == GAP_ERR_NO_ERROR
                            || param->status == GAP_ERR_CANCELED);
            break;

        default:
        {
            ASSERT_DEBUG(param->status == GAP_ERR_NO_ERROR);
            /* No action required for other operations */
        }
        break;
    }

    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_GetDevInfoReqInd(ke_msg_id_t const msg_id,
 *                                           struct gapc_get_dev_info_req_ind
 *                                           const *param,
 *                                           ke_task_id_t const dest_id,
 *                                           ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle message device info request received from GAP
 *                 controller
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_get_dev_info_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPC_GetDevInfoReqInd(ke_msg_id_t const msg_id, struct gapc_get_dev_info_req_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    /* Allocate message */
    struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM, src_id, dest_id, gapc_get_dev_info_cfm, ble_env.local_name_len);

    switch (param->req)
    {
        case GAPC_DEV_NAME:
        {
            cfm->req = GAPC_DEV_NAME;
            memcpy(&cfm->info.name.value[0], ble_env.local_name, ble_env.local_name_len);
            cfm->info.name.length = ble_env.local_name_len;
        }
        break;

        case GAPC_DEV_APPEARANCE:
        {
            /* Set the device appearance (no appearance) */
            cfm->info.appearance = 0;
            cfm->req = GAPC_DEV_APPEARANCE;
            cfm->info.appearance = GAPM_WRITE_DISABLE;
        }
        break;

        case GAPC_DEV_SLV_PREF_PARAMS:
        {
            /* Slave preferred connection interval (minimum) */
            cfm->info.slv_params.con_intv_min = BDK_BLE_PREF_SLV_MIN_CON_INTERVAL;

            /* Slave preferred connection interval (maximum) */
            cfm->info.slv_params.con_intv_max = BDK_BLE_PREF_SLV_MAX_CON_INTERVAL;

            /* Slave preferred connection latency */
            cfm->info.slv_params.slave_latency = BDK_BLE_PREF_SLV_LATENCY;

            /* Slave preferred link supervision timeout */
            cfm->info.slv_params.conn_timeout = BDK_BLE_PREF_SLV_SUP_TIMEOUT;

            cfm->req = GAPC_DEV_SLV_PREF_PARAMS;
        }
        break;

        default:
        {
            /* No action required for other requests */
        }
        break;
    }

    /* Send message */
    ke_msg_send(cfm);

    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_ConnectionReqInd(ke_msg_idd_t const msg_id,
 *                                           struct gapc_connection_req_ind
 *                                           const *param,
 *                                           ke_task_id_t const dest_id,
 *                                           ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle connection indication message received from GAP
 *                 controller
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_connection_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPC_ConnectionReqInd(ke_msg_id_t const msg_id, struct gapc_connection_req_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if (ble_env.state != BLE_STATE_CONNECTED)
    {
        ble_env.conidx = KE_IDX_GET(src_id);
        if (ble_env.conidx != GAP_INVALID_CONIDX)
        {
            ble_env.state = BLE_STATE_CONNECTED;
            ble_env.conhdl = param->conhdl;

            BDK_BLE_SendConnectionConfirmation();
            BDK_BLE_SetServiceState(true);

            App_PeerDeviceConnected();
        }
    }

    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_CmpEvt(ke_msg_id_t const msg_id,
 *                                 struct gapc_cmp_evt
 *                                 const *param,
 *                                 ke_task_id_t const dest_id,
 *                                 ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle received GAPC complete event
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_cmp_evt
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPC_CmpEvt(ke_msg_id_t const msg_id, struct gapc_cmp_evt const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    ASSERT_DEBUG(param->status == GAP_ERR_NO_ERROR);

    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_DisconnectInd(ke_msg_id_t const msg_id,
 *                                        struct gapc_disconnect_ind
 *                                        const *param,
 *                                        ke_task_id_t const dest_id,
 *                                        ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle disconnect indication message from GAP controller
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_disconnect_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPC_DisconnectInd(ke_msg_id_t const msg_id, struct gapc_disconnect_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if (ble_env.state != BLE_STATE_CONNECTED)
    {
        return (KE_MSG_CONSUMED);
    }

    /* Go to the ready state */
    ble_env.state = BLE_STATE_READY;
    ble_env.conidx = INVALID_DEV_IDX;

    /* Disable services for this connection */
    BDK_BLE_SetServiceState(false);

    App_PeerDeviceDisconnected();

    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_ParamUpdatedInd(ke_msg_id_t const msg_id,
 *                                          struct gapc_param_updated_ind
 *                                          const *param,
 *                                          ke_task_id_t const dest_id,
 *                                          ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle message parameter updated indication received from
 *                 GAP controller
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_param_updated_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static int GAPC_ParamUpdatedInd(ke_msg_id_t const msg_id, struct gapc_param_updated_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return KE_MSG_CONSUMED;
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_ParamUpdateReqInd(ke_msg_id_t const msg_id,
 *                         struct gapc_param_update_req_ind const *param,
 *                         ke_task_id_t const dest_id,
 *                         ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   :
 * ------------------------------------------------------------------------- */
static int GAPC_ParamUpdateReqInd(ke_msg_id_t const msg_id, struct gapc_param_update_req_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_param_update_cfm *cfm;

    if (ble_env.state != BLE_STATE_CONNECTED)
    {
        return KE_MSG_CONSUMED;
    }

    cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM, KE_BUILD_ID(TASK_GAPC, ble_env.conidx), KE_BUILD_ID(TASK_APP, 0), gapc_param_update_cfm);
    cfm->accept = 1;
    cfm->ce_len_max = 0xFFFF;
    cfm->ce_len_min = 0xFFFF;

    /* Send message */
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : bool Service_Add(void)
 * ----------------------------------------------------------------------------
 * Description   : Add the next service in the service list,
 *                 calling the appropriate add service function
 * Inputs        : None
 * Outputs       : return value - Indicates if any service has not yet been
 *                                added
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static bool BDK_BLE_ServiceAdd(void)
{
    /* Check if another should be added in the database */
    if (ble_env.svc_add_index < ble_env.svc_count && ble_env.svc_add_func[ble_env.svc_add_index] != NULL)
    {
        /* Call the function used to add the required service */
        ble_env.svc_add_func[ble_env.svc_add_index]();

        /* Select the next service to add */
        ble_env.svc_add_index += 1;
        return true;
    }

    return false;
}

void BDK_BLE_AdvertisingStart(void)
{
    /* Change state to advertising */
    if (ble_env.state == BLE_STATE_READY)
    {
        ble_env.state = BLE_STATE_ADVERTISING;

        /* Prepare the GAPM_START_ADVERTISE_CMD message */
        struct gapm_start_advertise_cmd *cmd;

        /* Prepare the start advertisement command message */
        cmd = KE_MSG_ALLOC(GAPM_START_ADVERTISE_CMD, TASK_GAPM, TASK_APP,
                           gapm_start_advertise_cmd);
        cmd->op.addr_src = GAPM_STATIC_ADDR;
        cmd->channel_map = ADV_ALL_CHNLS_EN;

        cmd->intv_min = ble_env.adv_int_min;
        cmd->intv_max = ble_env.adv_int_max;

        cmd->op.code = GAPM_ADV_UNDIRECT;
        cmd->op.state = 0;
        cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
        cmd->info.host.adv_filt_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;

        /* Set advertisement packet data (Complete Local Name). */
        cmd->info.host.adv_data[0] = 1 + ble_env.local_name_len;
        cmd->info.host.adv_data[1] = GAP_AD_TYPE_COMPLETE_NAME;
        memcpy(&cmd->info.host.adv_data[2], ble_env.local_name, ble_env.local_name_len);
        cmd->info.host.adv_data_len = 2 + ble_env.local_name_len;

        if (cmd->info.host.adv_data_len <= (GAP_ADV_DATA_LEN - 6))
        {
            uint8_t *ptr = &cmd->info.host.adv_data[cmd->info.host.adv_data_len];
            ptr[0] = 1 + 4;
            ptr[1] = GAP_AD_TYPE_SLAVE_CONN_INT_RANGE;
            ptr[2] = BDK_BLE_PREF_SLV_MIN_CON_INTERVAL & 0xFF;
            ptr[3] = BDK_BLE_PREF_SLV_MIN_CON_INTERVAL >> 8;
            ptr[4] = BDK_BLE_PREF_SLV_MAX_CON_INTERVAL & 0xFF;
            ptr[5] = BDK_BLE_PREF_SLV_MAX_CON_INTERVAL >> 8;

            cmd->info.host.adv_data_len += 6;
        }

        /* Set Scan Response packet data (Manufacturer Specific Data). */
        cmd->info.host.scan_rsp_data[0] = BDK_BLE_MANUFACTURER_ID_LENGTH + ble_env.manu_data_len;
        memcpy(&cmd->info.host.scan_rsp_data[1], BDK_BLE_MANUFACTURER_ID, BDK_BLE_MANUFACTURER_ID_LENGTH);
        memcpy(&cmd->info.host.scan_rsp_data[1 + BDK_BLE_MANUFACTURER_ID_LENGTH], ble_env.manu_data, ble_env.manu_data_len);
        cmd->info.host.scan_rsp_data_len = 1 + BDK_BLE_MANUFACTURER_ID_LENGTH + ble_env.manu_data_len;

        /* Send the message */
        ke_msg_send(cmd);
    }
}

void BDK_BLE_AdvertisingStop(void)
{
    if (ble_env.state == BLE_STATE_ADVERTISING)
    {
        ble_env.state = BLE_STATE_READY;

        struct gapm_cancel_cmd *cmd;

        cmd = KE_MSG_ALLOC(GAPM_CANCEL_CMD, TASK_GAPM, TASK_APP, gapm_cancel_cmd);

        cmd->operation = GAPM_CANCEL;

        ke_msg_send(cmd);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Send_Connection_Confirmation(uint8_t device_indx)
 * ----------------------------------------------------------------------------
 * Description   : Send connection confirmation to peer device
 * Inputs        : - device_indx - Peer device index in the app env structure
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
static void BDK_BLE_SendConnectionConfirmation(void)
{
    struct gapc_connection_cfm *cfm;

    /* Allocate connection confirmation message */
    cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                       KE_BUILD_ID(TASK_GAPC, ble_env.conidx),
                       KE_BUILD_ID(TASK_APP, 0), gapc_connection_cfm);

    cfm->ltk_present = false;

    cfm->svc_changed_ind_enable = 0;
    cfm->auth = GAP_AUTH_REQ_NO_MITM_BOND;

    /* Send the message */
    ke_msg_send(cfm);
}

/* ----------------------------------------------------------------------------
 * Function      : void BLE_SetServiceState(bool enable, uint8_t device_indx)
 * ----------------------------------------------------------------------------
 * Description   : Set Bluetooth application environment state to enabled
 * Inputs        : - enable      - Indicates that enable request should be sent
 *                                 for all services/profiles or their status
 *                                 should be set to disabled
 *                                 enabled or disabled
 *                 - device_indx - Peer device index in the app env structure
 * Outputs       : None
 * Assumptions   : Peer device must be connected. This function should
 *                  only be called after ConnectionConfirmation is sent.
 * ------------------------------------------------------------------------- */
void BDK_BLE_SetServiceState(bool enable)
{
    if (enable == true)
    {
        uint8_t i;

        for (i = 0; i < ble_env.svc_count; ++i)
        {
            ble_env.svc_enable_func[i](ble_env.conidx);
        }
    }

    if (ble_env.state < BLE_STATE_CONNECTED)
    {
        BDK_BLE_AdvertisingStart();
    }
//    if (device_indx != INVALID_DEV_IDX)
//    {
//        /* All standard services should be send enable request to the stack,
//         * for custom services, application should decide if it would want
//         * to do a specific action
//         * all services should be disabled once the link is lost
//         */
//        if (enable == true)
//        {
//            /* To enable standard Bluetooth services, an enable request should
//             * be sent to the stack (for related profile) and at receiving the
//             * response the enable flag can be set. For custom service it is
//             * application implementation dependent. Here, it starts service
//             * discovery, and if the service UUID and characteristics UUID are
//             * discovered, then it goes to an state that is equivalent to the
//             * enable flag of standard profiles
//             */
//            ble_env[device_indx].next_svc_enable = 0;
//            Service_Enable(ble_env[device_indx].conidx);
//        }
//        else
//        {
//            bass_support_env[device_indx].enable = false;
//            cs_env[device_indx].state = CS_INIT;
//        }
//    }
//
//    /* Keep advertising while all masters are not connected */
//    if (Connected_Peer_Num() < NUM_MASTERS)
//    {
//        Advertising_Start();
//    }
}

signed int BDK_BLE_GetConIdx(void)
{
    /* Check connection index to set device index */
    if (ble_env.state == BLE_STATE_CONNECTED)
    {
        return ble_env.conidx;
    }

    return INVALID_DEV_IDX;
}

bool BDK_BLE_IsConnected(void)
{
    return (ble_env.state == BLE_STATE_CONNECTED);
}

//! \}
//! \}

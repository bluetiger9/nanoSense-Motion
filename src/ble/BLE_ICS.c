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
//! \file BLE_ICS.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup BLE_GRP
//! \{
//! \addtogroup ICS
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <BDK_Task.h>
#include "BLE_ICS.h"
#include <HAL_error.h>
#include <app_trace.h>

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

/** \brief <i>Characteristic</i> declaration attribute UUID */
#define ATT_DECL_CHARACTERISTIC_128     { 0x03, 0x28, 0x00, 0x00, 0x00, 0x00, \
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                          0x00, 0x00, 0x00, 0x00 }

/** \brief <i>Client Characteristic Configuration</i> attribute UUID */
#define ATT_DESC_CLIENT_CHAR_CFG_128    { 0x02, 0x29, 0x00, 0x00, 0x00, 0x00, \
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                          0x00, 0x00, 0x00, 0x00 }

/** \brief <i>Characteristic User Description</i> attribute UUID */
#define ATT_DESC_CHAR_USER_DESC_128     { 0x01, 0x29, 0x00, 0x00, 0x00, 0x00, \
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                          0x00, 0x00, 0x00, 0x00 }

#define ATT_DECL_CHAR() \
    { ATT_DECL_CHARACTERISTIC_128, PERM(RD, ENABLE), 0, 0 }

#define ATT_DECL_CHAR_UUID_128(uuid, perm, max_length) \
    { uuid, perm, max_length, PERM(RI, ENABLE) | PERM(UUID_LEN, UUID_128) }

#define ATT_DECL_CHAR_CCC()                                                     \
    { ATT_DESC_CLIENT_CHAR_CFG_128, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), \
      0, PERM(RI, ENABLE) }

#define ATT_DECL_CHAR_USER_DESC(max_length)                      \
    { ATT_DESC_CHAR_USER_DESC_128, PERM(RD, ENABLE), max_length, \
      PERM(RI, ENABLE) }

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

//! \name Internal Functions
//! \{

static void BLE_ICS_ServiceAdd(void);

static void BLE_ICS_Enable(uint8_t conidx);

static int BLE_ICS_GATTM_AddSvcRsp(ke_msg_id_t const msg_id,
        struct gattm_add_svc_rsp const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id);

static int BLE_ICS_GATTC_ReadReqInd(ke_msg_id_t const msg_id,
        struct gattc_read_req_ind const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id);

static int BLE_ICS_GATTC_WriteReqInd(ke_msg_id_t const msg_id,
        struct gattc_write_req_ind const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id);

static int BLE_ICS_GATTC_AttInfoReqInd(ke_msg_id_t const msg_id,
        struct gattc_read_req_ind const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id);

static int BLE_ICS_GATTC_CmpEvt(ke_msg_id_t const msg_id,
        struct gattc_cmp_evt const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id);

//! \}

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static struct BLE_ICS_Resources cs_res = { 0 };

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void BLE_ICS_Initialize(BLE_ICS_RxIndHandler rx_ind_handler)
{
    if (cs_res.state == BLE_ICS_OFF)
    {
        BDK_BLE_Initialize();

        memset(&cs_res, 0, sizeof(cs_res));
        cs_res.state = BLE_ICS_CREATE_DB;
        cs_res.rx_write_handler = rx_ind_handler;
        cs_res.tx_cccd_value = ATT_CCC_START_NTF;

        BDK_TaskAddMsgHandler(GATTM_ADD_SVC_RSP,
                (ke_msg_func_t) &BLE_ICS_GATTM_AddSvcRsp);
        BDK_TaskAddMsgHandler(GATTC_READ_REQ_IND,
                (ke_msg_func_t) &BLE_ICS_GATTC_ReadReqInd);
        BDK_TaskAddMsgHandler(GATTC_WRITE_REQ_IND,
                (ke_msg_func_t) &BLE_ICS_GATTC_WriteReqInd);
        BDK_TaskAddMsgHandler(GATTC_ATT_INFO_REQ_IND,
                (ke_msg_func_t) &BLE_ICS_GATTC_AttInfoReqInd);
        BDK_TaskAddMsgHandler(GATTC_CMP_EVT,
                (ke_msg_func_t) &BLE_ICS_GATTC_CmpEvt);

        BDK_BLE_AddService(&BLE_ICS_ServiceAdd, &BLE_ICS_Enable);
    }
}

uint32_t BLE_ICS_Notify(uint8_t *data, uint8_t data_len)
{
    int conidx = BDK_BLE_GetConIdx();
    struct gattc_send_evt_cmd *cmd = NULL;

    if (cs_res.state < BLE_ICS_CONNECTED || conidx == INVALID_DEV_IDX)
    {
        return 1;
    }

    if (data_len == 0 || data_len > ICS_CHARACTERISTIC_VALUE_LENGTH)
    {
        return 2;
    }

    /* Copy data for any later read requests. */
    memcpy(cs_res.tx_value, data, data_len);
    cs_res.tx_value_length = data_len;

    /* Send notify command with data. */
    cmd = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD, KE_BUILD_ID(TASK_GATTC, conidx),
            TASK_APP, gattc_send_evt_cmd, data_len * sizeof(uint8_t));
    cmd->handle = cs_res.start_hdl + ICS_IDX_TX_VALUE_VAL + 1;
    cmd->operation = GATTC_NOTIFY;
    cmd->seq_num = 0;
    cmd->length = data_len;
    memcpy(cmd->value, data, data_len);

    ke_msg_send(cmd);

    return 0;
}

static void BLE_ICS_ServiceAdd(void)
{
    struct gattm_add_svc_req * req;
    const uint8_t service_uuid[ATT_UUID_128_LEN] = ICS_SERVICE_UUID;
    const struct gattm_att_desc att[ICS_IDX_NB] = {
            /* TX Characteristic */
            [ICS_IDX_TX_VALUE_CHAR] = ATT_DECL_CHAR(),

            [ICS_IDX_TX_VALUE_VAL] = ATT_DECL_CHAR_UUID_128(
                    ICS_TX_CHARACTERISTIC_UUID,
                    PERM(RD, ENABLE) | PERM(NTF, ENABLE),
                    ICS_CHARACTERISTIC_VALUE_LENGTH),

            [ICS_IDX_TX_VALUE_CCC] = ATT_DECL_CHAR_CCC(),

            [ICS_IDX_TX_VALUE_USR_DSCP] = ATT_DECL_CHAR_USER_DESC(
                    ICS_TX_CHARACTERISTIC_NAME_LEN),

            /* RX Characteristic */
            [ICS_IDX_RX_VALUE_CHAR] = ATT_DECL_CHAR(),

            [ICS_IDX_RX_VALUE_VAL] = ATT_DECL_CHAR_UUID_128(
                    ICS_RX_CHARACTERISTIC_UUID,
                    PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE) | PERM(WRITE_COMMAND, ENABLE),
                    ICS_CHARACTERISTIC_VALUE_LENGTH),

            [ICS_IDX_RX_VALUE_CCC] = ATT_DECL_CHAR_CCC(),

            [ICS_IDX_RX_VALUE_USR_DSCP] = ATT_DECL_CHAR_USER_DESC(
                    ICS_RX_CHARACTERISTIC_NAME_LEN)
    };

    if (cs_res.state == BLE_ICS_CREATE_DB)
    {
        req = KE_MSG_ALLOC_DYN(GATTM_ADD_SVC_REQ, TASK_GATTM, TASK_APP,
                gattm_add_svc_req, ICS_IDX_NB * sizeof(struct gattm_att_desc));
        req->svc_desc.start_hdl = 0;
        req->svc_desc.task_id = TASK_APP;
        req->svc_desc.perm = PERM(SVC_UUID_LEN, UUID_128);
        req->svc_desc.nb_att = ICS_IDX_NB;
        memcpy(req->svc_desc.uuid, service_uuid, ATT_UUID_128_LEN);
        memcpy(req->svc_desc.atts, att,
                ICS_IDX_NB * sizeof(struct gattm_att_desc));

        ke_msg_send(req);
    }
}

static void BLE_ICS_Enable(uint8_t conidx)
{
    if (cs_res.state >= BLE_ICS_READY)
    {
        if (conidx != INVALID_DEV_IDX)
        {
            cs_res.state = BLE_ICS_CONNECTED;
        }
        else
        {
            cs_res.state = BLE_ICS_READY;
        }
    }
}

static int BLE_ICS_GATTM_AddSvcRsp(ke_msg_id_t const msg_id,
        struct gattm_add_svc_rsp const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    ASSERT_DEBUG(param->status == GAP_ERR_NO_ERROR);

    if (cs_res.state == BLE_ICS_CREATE_DB)
    {
        cs_res.state = BLE_ICS_READY;

        /* Save attribute database pointer to first ICS characteristic. */
        cs_res.start_hdl = param->start_hdl;

        /* Indicate to BLE stack that ICS profile was added and it can continue
         * with initialization of other profiles.
         */
        BDK_BLE_ProfileAddedInd();
    }

    return KE_MSG_CONSUMED;
}

static int BLE_ICS_GATTC_ReadReqInd(ke_msg_id_t const msg_id,
        struct gattc_read_req_ind const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    uint8_t status = GAP_ERR_NO_ERROR;
    uint8_t *val_ptr = NULL;
    uint8_t val_len = 0;
    uint16_t att_num = 0;
    struct gattc_read_cfm *cfm;

    int conidx = BDK_BLE_GetConIdx();

    if (conidx == INVALID_DEV_IDX)
    {
        return KE_MSG_CONSUMED;
    }

    /* Get index of characteristic which was requested. */
    if (param->handle > cs_res.start_hdl)
    {
        att_num = param->handle - cs_res.start_hdl - 1;
    }
    else
    {
        status = ATT_ERR_INVALID_HANDLE;
    }

    if (status == GAP_ERR_NO_ERROR)
    {
        switch (att_num)
        {
        case ICS_IDX_TX_VALUE_VAL:
            val_len = cs_res.tx_value_length;
            val_ptr = cs_res.tx_value;
            break;

        case ICS_IDX_TX_VALUE_CCC:
            val_len = 2;
            val_ptr = (uint8_t*) &cs_res.tx_cccd_value;
            break;

        case ICS_IDX_TX_VALUE_USR_DSCP:
            val_len = ICS_TX_CHARACTERISTIC_NAME_LEN;
            val_ptr = (uint8_t*) ICS_TX_CHARACTERISTIC_NAME;
            break;

        case ICS_IDX_RX_VALUE_VAL:
            val_len = cs_res.rx_value_length;
            val_ptr = cs_res.rx_value;
            break;

        case ICS_IDX_RX_VALUE_CCC:
            val_len = 2;
            val_ptr = (uint8_t*) &cs_res.rx_cccd_value;
            break;

        case ICS_IDX_RX_VALUE_USR_DSCP:
            val_len = ICS_RX_CHARACTERISTIC_NAME_LEN;
            val_ptr = (uint8_t*) ICS_RX_CHARACTERISTIC_NAME;
            break;

        default:
            status = ATT_ERR_READ_NOT_PERMITTED;
            break;
        }
    }

    cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, KE_BUILD_ID(TASK_GATTC, conidx),
            TASK_APP, gattc_read_cfm, val_len)
    ;

    if (val_ptr != NULL)
    {
        memcpy(cfm->value, val_ptr, val_len);
    }
    cfm->handle = param->handle;
    cfm->length = val_len;
    cfm->status = status;

    ke_msg_send(cfm);

    return KE_MSG_CONSUMED;
}

static int BLE_ICS_GATTC_WriteReqInd(ke_msg_id_t const msg_id,
        struct gattc_write_req_ind const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    uint8_t status = GAP_ERR_NO_ERROR;
    uint16_t att_num = 0;
    int conidx = BDK_BLE_GetConIdx();
    struct gattc_write_cfm *cfm;

    /* Check if connection is valid. */
    if (conidx == INVALID_DEV_IDX)
    {
        return KE_MSG_CONSUMED;
    }

    /* Check that offset is valid */
    if (param->offset != 0)
    {
        status = ATT_ERR_INVALID_OFFSET;
    }

    /* Get index of characteristic which was requested. */
    if (param->handle > cs_res.start_hdl)
    {
        att_num = param->handle - cs_res.start_hdl - 1;
    }
    else
    {
        status = ATT_ERR_INVALID_HANDLE;
    }

    if (status == GAP_ERR_NO_ERROR)
    {
        switch (att_num)
        {
        case ICS_IDX_TX_VALUE_CCC:
            if (param->length == 2)
            {
                memcpy(&cs_res.tx_cccd_value, param->value, 2);
            }
            else
            {
                status = ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN;
            }
            break;

            /* New command was written. */
        case ICS_IDX_RX_VALUE_VAL:
            if (param->length <= ICS_CHARACTERISTIC_VALUE_LENGTH)
            {
                memcpy(&cs_res.rx_value, param->value, param->length);
                cs_res.rx_value_length = param->length;
            }
            else
            {
                status = ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN;
            }
            break;

        case ICS_IDX_RX_VALUE_CCC:
            if (param->length == 2)
            {
                memcpy(&cs_res.rx_cccd_value, param->value, 2);
            }
            else
            {
                status = ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN;
            }
            break;

        default:
            status = ATT_ERR_WRITE_NOT_PERMITTED;
            break;
        }
    }

    cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, KE_BUILD_ID(TASK_GATTC, conidx),
            TASK_APP, gattc_write_cfm);

    cfm->handle = param->handle;
    cfm->status = status;

    ke_msg_send(cfm);

    /* Write indication handler */
    if (att_num == ICS_IDX_RX_VALUE_VAL && cs_res.rx_write_handler != NULL
            && status == GAP_ERR_NO_ERROR)
    {
        struct BLE_ICS_RxIndData ind;
        memcpy(ind.data, cs_res.rx_value, cs_res.rx_value_length);
        ind.data_len = cs_res.rx_value_length;

        cs_res.rx_write_handler(&ind);
    }

    return KE_MSG_CONSUMED;
}

static int BLE_ICS_GATTC_AttInfoReqInd(ke_msg_id_t const msg_id,
        struct gattc_read_req_ind const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    int conidx = BDK_BLE_GetConIdx();
    uint16_t att_num = 0;
    uint8_t status = GAP_ERR_NO_ERROR;
    struct gattc_att_info_cfm *cfm;

    /* Check if connection is valid. */
    if (conidx == INVALID_DEV_IDX)
    {
        return KE_MSG_CONSUMED;
    }

    /* Get index of characteristic which was requested. */
    if (param->handle > cs_res.start_hdl)
    {
        att_num = param->handle - cs_res.start_hdl - 1;
    }
    else
    {
        status = ATT_ERR_INVALID_HANDLE;
    }

    if (att_num == ICS_IDX_RX_VALUE_VAL)
    {
        cfm = KE_MSG_ALLOC(GATTC_ATT_INFO_CFM, KE_BUILD_ID(TASK_GATTC, conidx),
                TASK_APP, gattc_att_info_cfm);
        cfm->handle = param->handle;
        cfm->length = ICS_CHARACTERISTIC_VALUE_LENGTH;
        cfm->status = status;

        ke_msg_send(cfm);
    }

    return KE_MSG_CONSUMED;
}

static int BLE_ICS_GATTC_CmpEvt(ke_msg_id_t const msg_id,
        struct gattc_cmp_evt const *param, ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    return KE_MSG_CONSUMED;
}

//! \}
//! \}
//! \}

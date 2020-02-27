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
//! \file BLE_BASS.c
//! \version v1.0.0
//!
//! \addtogroup BDK_GRP
//! \{
//! \addtogroup BLE_GRP
//! \{
//! \addtogroup BASS
//! \{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include "BDK_Task.h"
#include "BLE_BASS.h"

//-----------------------------------------------------------------------------
// DEFINES / CONSTANTS
//-----------------------------------------------------------------------------

/* Maximum battery level */
#define BAT_LVL_MAX                     100

struct BLE_BASS_Resources
{
    /** Whether there is any device connected. */
    bool enabled;

    /** Message id assigned by APP_TASK for measuring callback. */
    ke_msg_id_t measure_msg_id;

    /** Delay between voltage measurements in 10x ms. */
    uint16_t measure_sample_rate;

    /** How many samples to average to get final voltage. */
    uint8_t measure_avg;

    /** How many measurements are summed in acc_value. */
    uint8_t total_samples;

    /** Accumulator for voltage measurements. */
    uint32_t acc_value;

    /** Last measured battery level. */
    uint8_t batt_level;

    /** Measured voltage level representing fully charged battery.
     *
     * All voltage levels above this value will be treated as 100%.
     */
    uint16_t vbat_max;

    /** Measured voltage level representing discharged battery.
	 *
	 * All voltage levels below this value will be treated as 0%.
	 */
    uint16_t vbat_min;

    BLE_BASS_BattLevelInd level_change_ind;
};

//-----------------------------------------------------------------------------
// EXTERNAL / FORWARD DECLARATIONS
//-----------------------------------------------------------------------------

static void BLE_BASS_ServiceAdd(void);
static void BLE_BASS_Enable(uint8_t conidx);
static int BLE_BASS_EnableRsp(ke_msg_id_t const msg_id,struct bass_enable_rsp const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
//static int BLE_BASS_LevelNtfCfgInd(ke_msg_id_t const msg_id, struct bass_batt_level_ntf_cfg_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int BLE_BASS_MeasureBattLevel(ke_msg_id_t const msg_id,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void BLE_BASS_UpdateBatLevel(void);

//-----------------------------------------------------------------------------
// INTERNAL / STATIC VARIABLES
//-----------------------------------------------------------------------------

static struct BLE_BASS_Resources bass_res = { 0 };

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

void BLE_BASS_Initialize(uint16_t sample_rate, uint8_t avg_count)
{
    bass_res.enabled = false;
    bass_res.measure_sample_rate = sample_rate / 10;
    bass_res.measure_avg = avg_count;
    bass_res.measure_msg_id = BDK_TaskAllocateMsgId();
    bass_res.vbat_min = VBAT_2p0V_MEASURED;
    bass_res.vbat_max = VBAT_3p0V_MEASURED;
    bass_res.level_change_ind = NULL;

    BDK_BLE_Initialize();

    BDK_TaskAddMsgHandler(BASS_ENABLE_RSP, (ke_msg_func_t)&BLE_BASS_EnableRsp);
    BDK_TaskAddMsgHandler(bass_res.measure_msg_id, (ke_msg_func_t)&BLE_BASS_MeasureBattLevel);
    BDK_BLE_AddService(&BLE_BASS_ServiceAdd, &BLE_BASS_Enable);

    /* Configure ADC */
    Sys_ADC_Set_Config(ADC_VBAT_DIV2_NORMAL | ADC_NORMAL | ADC_PRESCALE_6400);
    Sys_ADC_InputSelectConfig(0, ADC_NEG_INPUT_GND | ADC_POS_INPUT_VBAT_DIV2);
}

void BLE_BASS_SetVoltageRange(uint16_t batt_empty, uint16_t batt_full)
{
	bass_res.vbat_min = batt_empty;
	bass_res.vbat_max = batt_full;
}

void BLE_BASS_SetBattLevelInd(BLE_BASS_BattLevelInd cb)
{
	bass_res.level_change_ind = cb;
}

static void BLE_BASS_ServiceAdd(void)
{
    struct gapm_profile_task_add_cmd *req;
    struct bass_db_cfg *cfg;

    req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD, TASK_GAPM, TASK_APP, gapm_profile_task_add_cmd, sizeof(struct bass_db_cfg));

    /* Fill message */
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, DISABLE);
    req->prf_task_id = TASK_ID_BASS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    /* Set parameters */
    cfg = (struct bass_db_cfg *)req->param;
    cfg->bas_nb = 1;
    cfg->features[0] = BAS_BATT_LVL_NTF_SUP;
    cfg->batt_level_pres_format[0].unit = ATT_UNIT_PERCENTAGE;
    cfg->batt_level_pres_format[0].description = 0;
    cfg->batt_level_pres_format[0].format = ATT_FORMAT_UINT8;
    cfg->batt_level_pres_format[0].exponent = 0;
    cfg->batt_level_pres_format[0].name_space = 1;

    /* Send the message */
    ke_msg_send(req);

    /* Start battery level measurement */
    ke_timer_set(bass_res.measure_msg_id, TASK_APP, bass_res.measure_sample_rate);
}

static void BLE_BASS_Enable(uint8_t conidx)
{
    if (conidx != INVALID_DEV_IDX)
    {
        struct bass_enable_req *req;
        struct bass_env_tag *bass_env;
        bass_env = PRF_ENV_GET(BASS, bass);

        /* Allocate the message to enable battery service server */
        req = KE_MSG_ALLOC(BASS_ENABLE_REQ,
                           prf_src_task_get(&(bass_env->prf_env), conidx),
                           TASK_APP, bass_enable_req);

        /* Fill in the parameter structure, initializing the old battery levels
         * to 0 as there are no previous measurements */
        req->conidx = conidx;

        /* Enable notification support */
        req->ntf_cfg = BAS_BATT_LVL_NTF_SUP;

        /* Send the message */
        ke_msg_send(req);
    }
    else
    {
        bass_res.enabled = false;
    }
}

static int BLE_BASS_EnableRsp(ke_msg_id_t const msg_id,struct bass_enable_rsp const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        bass_res.enabled = true;
        BLE_BASS_UpdateBatLevel();
    }
    else
    {
        bass_res.enabled = false;
    }

    return (KE_MSG_CONSUMED);
}

static int BLE_BASS_MeasureBattLevel(ke_msg_id_t const msg_id,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
	uint16_t voltage;
    uint16_t level;

    ke_timer_set(bass_res.measure_msg_id, TASK_APP, bass_res.measure_sample_rate);

    voltage = ADC->DATA_TRIM_CH[0];
    level = ((voltage - bass_res.vbat_min) * BAT_LVL_MAX
                     / (bass_res.vbat_max - bass_res.vbat_min));
	level = ((level > BAT_LVL_MAX) ? BAT_LVL_MAX : level);

    bass_res.acc_value += level;
    bass_res.total_samples += 1;

    if (bass_res.total_samples >= bass_res.measure_avg)
    {
        uint8_t bat_level = bass_res.acc_value / bass_res.measure_avg;

        if (bat_level != bass_res.batt_level)
        {
            bass_res.batt_level = bat_level;

            BLE_BASS_UpdateBatLevel();

            if (bass_res.level_change_ind != NULL)
            {
            	bass_res.level_change_ind(voltage, bat_level);
            }
        }

        bass_res.total_samples = 0;
        bass_res.acc_value = 0;
    }

    return KE_MSG_CONSUMED;
}

static void BLE_BASS_UpdateBatLevel(void)
{
    int conidx = BDK_BLE_GetConIdx();

    if (conidx != INVALID_DEV_IDX)
    {
        struct bass_env_tag *bass_env = NULL;
        struct bass_batt_level_upd_req *req = NULL;
        bass_env = PRF_ENV_GET(BASS, bass);

        req = KE_MSG_ALLOC(BASS_BATT_LEVEL_UPD_REQ, prf_src_task_get(&(bass_env->prf_env), conidx), TASK_APP, bass_batt_level_upd_req);

        /* Fill in the parameter structure */
        req->bas_instance = 0;
        req->batt_level = bass_res.batt_level;

        ke_msg_send(req);
    }
}

//! \}
//! \}
//! \}

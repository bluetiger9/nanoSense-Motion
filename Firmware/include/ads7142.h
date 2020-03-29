#ifndef INCLUDE_ADS7142_H_
#define INCLUDE_ADS7142_H

#include <stdint.h>
#include <stdbool.h>

#define ADS7142_OK 				  0
#define ADS7142_COMM_ERROR		  1

#define PIN_READY				  3

/* Default I2C address (when ADDR is connected to ground) */
#define ADS7142_I2C_ADDR          0x18

/* Command op-codes */
#define OP_GENERAL_CALL              0x00
#define OP_DEVICE_RESET		         0x06
#define OP_SINGLE_READ               0x10
#define OP_SINGLE_WRITE              0x08
#define OP_SET_BIT                   0x18
#define OP_CLEAR_BIT                 0x20
#define OP_CONTINOUS_READ            0x30
#define OP_CONTINOUS_WRITE           0x28

/* Register Addresses */
#define REG_WKEY                  0x17
#define REG_DEVICE_RESET          0x14
#define REG_OFFSET_CAL            0x15
#define REG_OPMODE_SEL            0x1C

#define REG_OPMODE_I2CMODE_STATUS     0x00
#define REG_CHANNEL_INPUT_CFG         0x24
#define REG_AUTO_SEQ_CHEN             0x20
#define REG_START_SEQUENCE            0x1E

#define REG_ABORT_SEQUENCE            0x1F
#define REG_SEQUENCE_STATUS           0x04
#define REG_OSC_SEL                   0x18
#define REG_NCLK_SEL                  0x19

#define REG_DATA_BUFFER_OPMODE        0x2C
#define REG_DOUT_FORMAT_CFG           0x28
#define REG_DATA_BUFFER_STATUS        0x01
#define REG_ACC_EN                    0x30


#define ACC_CH0_LSB               0x08
#define ACC_CH0_MSB               0x09
#define ACC_CH1_LSB               0x0A
#define ACC_CH1_MSB               0x0B

#define ACCUMULATOR_STATUS        0x02
#define ALERT_DWC_EN              0x37
#define ALERT_CHEN                0x34

#define DWC_HTH_CH0_LSB           0x38
#define DWC_HTH_CH0_MSB           0x39
#define DWC_LTH_CH0_LSB           0x3A
#define DWC_LTH_CH0_MSB           0x3B

#define DWC_HYS_CH0               0x40

#define DWC_HTH_CH1_LSB           0x3C
#define DWC_HTH_CH1_MSB           0x3D
#define DWC_LTH_CH1_LSB           0x3E
#define DWC_LTH_CH1_MSB           0x3F

#define DWC_HYS_CH1               0x41

#define PRE_ALERT_MAX_EVENT_COUNT 0x36

#define ALERT_TRIG_CHID           0x03
#define ALERT_LOW_FLAGS           0x0C
#define ALERT_HIGH_FLAGS          0x0E

/* Register Value */
#define VAL_WKEY_ENABLE_RESET 		0x0A
#define VAL_WKEY_DISABLE_RESET		0x00
#define VAL_DEVICE_RESET			0x01

// API

typedef int32_t ret_t;

ret_t ads7142_init();

ret_t ads7142_read(uint32_t channels[]);

ret_t ads7142_set_alert_thresholds(uint8_t channel, uint32_t low, uint32_t high);

ret_t ads7142_enable_alerts(bool channel0, bool channel1);

ret_t ads7142_autonomous_mode_configure();

ret_t ads7142_autonomous_mode_start();

ret_t ads7142_check();

int32_t ads7142_get_hal_error();

#endif /* INCLUDE_ADS7142_H_ */

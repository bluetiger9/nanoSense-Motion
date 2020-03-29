#include "ads7142.h"
#include "HAL_I2C.h"
#include <BDK.h>

static int32_t hal_error;

static ret_t ads7142_read_reg(uint8_t dev_id, uint8_t addr, uint8_t *value)
{
    uint8_t data[] = { OP_SINGLE_READ, addr };

    if ((hal_error = HAL_I2C_Write(dev_id, data, sizeof(data), false)) != HAL_OK)
    {
        return ADS7142_COMM_ERROR;
    }

    if ((hal_error = HAL_I2C_Read(dev_id, value, 1, false)) != HAL_OK)
    {
        return ADS7142_COMM_ERROR;
    }

    return ADS7142_OK;
}

static ret_t ads7142_write_reg(uint8_t dev_id, uint8_t addr, uint8_t value)
{
    uint8_t data[] = { OP_SINGLE_WRITE, addr, value };

    if ((hal_error = HAL_I2C_Write(dev_id, data, sizeof(data), false)) != HAL_OK)
    {
        return ADS7142_COMM_ERROR;
    }

    return ADS7142_OK;
}

static ret_t ads7142_set_bit(uint8_t dev_id, uint8_t addr, uint8_t value)
{
    uint8_t data[] = { OP_SET_BIT, addr, value };

    if ((hal_error = HAL_I2C_Write(dev_id, data, sizeof(data), false) != HAL_OK))
    {
        return ADS7142_COMM_ERROR;
    }

    return ADS7142_OK;
}

static uint16_t ads7142_value_decode(uint8_t *data) {
	return ((data[0] << 8) + data[1]) >> 4;
}

static void ads7142_waitReady() {
	// wait on the busy/ready pin
	while (DIO_DATA->ALIAS[PIN_READY] != 0);
}

static ret_t ads7142_device_reset() {
	// send reset command
    uint8_t data[] = { OP_DEVICE_RESET };
	if ((hal_error = HAL_I2C_Write(ADS7142_I2C_ADDR, data, sizeof(data), false) != HAL_OK)) {
		return ADS7142_COMM_ERROR;
	}

	// wait for the ready signal
	ads7142_waitReady();

	return ADS7142_OK;
}

ret_t ads7142_init() {
	// wait ready
	ads7142_waitReady();

	// reset
	ret_t ret;
	if ((ret = ads7142_device_reset())) {
		return ret;
	}

	// calibrate
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_OFFSET_CAL, 0x01))) {
		return ret;
	}

	// input configure: 2 channel single-ended
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_CHANNEL_INPUT_CFG, 0x03))) {
		return ret;
	}

	// op mode: auto seq enable
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_OPMODE_SEL, 0x04))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			REG_OSC_SEL, 0x01 /* = Device uses Low Power Oscillator */))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			REG_NCLK_SEL, 18 /* nCLK = 18 */))) {
		return ret;
	}

	// auto seq channels: 0 & 1
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_AUTO_SEQ_CHEN, 0x03))) {
		return ret;
	}

	ads7142_waitReady();

	return ADS7142_OK;
}

ret_t ads7142_read(uint32_t channels[]) {
	ret_t ret;
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_START_SEQUENCE, 0x01))) {
		return ret;
	}

	uint8_t data[4];
	if ((hal_error = HAL_I2C_Read(ADS7142_I2C_ADDR, data, sizeof(data), false)) != HAL_OK)
	{
		return ADS7142_COMM_ERROR;
	}

	channels[0] = ads7142_value_decode(data + 0);
	channels[1] = ads7142_value_decode(data + 2);

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_ABORT_SEQUENCE, 0x01))) {
		return ret;
	}

	ads7142_waitReady();

	return ret;
}

ret_t ads7142_set_alert_thresholds(uint8_t channel, uint32_t low, uint32_t high) {
	uint8_t lowLsb = low & 0xFF;
	uint8_t lowMsb = low >> 8;
	uint8_t highLsb = high & 0xFF;
	uint8_t highMsb = high >> 8;

	uint8_t regLowLsb;
	uint8_t regLowMsb;
	uint8_t regHighLsb;
	uint8_t regHighMsb;

	if (channel == 0) {
		regLowLsb = DWC_LTH_CH0_LSB;
		regLowMsb = DWC_LTH_CH0_MSB;
		regHighLsb = DWC_HTH_CH0_LSB;
		regHighMsb = DWC_HTH_CH0_MSB;

	} else {
		regLowLsb = DWC_LTH_CH1_LSB;
		regLowMsb = DWC_LTH_CH1_MSB;
		regHighLsb = DWC_HTH_CH1_LSB;
		regHighMsb = DWC_HTH_CH1_MSB;
	}

	ret_t ret;
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, regLowLsb, lowLsb))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, regLowMsb, lowMsb))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, regHighLsb, highLsb))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, regHighMsb, highMsb))) {
		return ret;
	}

	return ret;
}

ret_t ads7142_enable_alerts(bool channel0, bool channel1) {
	uint8_t value = (channel0 ? 1 : 0) + (channel1 ? 2 : 0);

	ret_t ret;

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, ALERT_CHEN, value))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, ALERT_DWC_EN, 0x01 /* enable window comparator */))) {
		return ret;
	}
}

ret_t ads7142_autonomous_mode_configure() {
	ret_t ret;

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			REG_OPMODE_SEL, 0x06 /* = Autonomous Monitoring Mode */))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			REG_OSC_SEL, 0x01 /* = Device uses Low Power Oscillator */))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			REG_NCLK_SEL, 18 /* nCLK = 18 */))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			REG_DATA_BUFFER_OPMODE, 0x04 /* = Pre Alert Data Mode */))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR,
			PRE_ALERT_MAX_EVENT_COUNT, 0x30 /* = 4 (3+1) */))) {
		return ret;
	}

	return ret;

}

ret_t ads7142_autonomous_mode_start() {
	ret_t ret;
	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, REG_START_SEQUENCE, 0x01))) {
		return ret;
	}

	uint8_t data[2];
	if ((hal_error = HAL_I2C_Read(ADS7142_I2C_ADDR, data, sizeof(data), false)) != HAL_OK)
	{
		return ADS7142_COMM_ERROR;
	}


	uint8_t highFlags;
	uint8_t lowFlags;
	if ((ret = ads7142_read_reg(ADS7142_I2C_ADDR, ALERT_LOW_FLAGS, &lowFlags))) {
		return ret;
	}

	if ((ret = ads7142_read_reg(ADS7142_I2C_ADDR, ALERT_HIGH_FLAGS, &highFlags))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, ALERT_LOW_FLAGS, 0x03 /* reset */))) {
		return ret;
	}

	if ((ret = ads7142_write_reg(ADS7142_I2C_ADDR, ALERT_HIGH_FLAGS, 0x03 /* reset */))) {
		return ret;
	}

	// wait for PIN_ADS7142_ALERT to get low
    while (DIO_DATA->ALIAS[PIN_ADS7142_ALERT] == 0);

	return ret;
}

ret_t ads7142_read_flags() {
	uint8_t ret;
	uint8_t highFlags;
	uint8_t lowFlags;
	if ((ret = ads7142_read_reg(ADS7142_I2C_ADDR, ALERT_LOW_FLAGS, &lowFlags))) {
		return ret;
	}

	if ((ret = ads7142_read_reg(ADS7142_I2C_ADDR, ALERT_HIGH_FLAGS, &highFlags))) {
		return ret;
	}

	uint8_t data[48];
	if ((hal_error = HAL_I2C_Read(ADS7142_I2C_ADDR, data, sizeof(data), false)) != HAL_OK)
	{
		return ADS7142_COMM_ERROR;
	}

	uint16_t values[24];
	for (int i = 0; i < 48; i += 2) {
		values[i / 2] = ads7142_value_decode(data + i);
	}

	return ret;
}

int32_t ads7142_get_hal_error() {
	return hal_error;
}

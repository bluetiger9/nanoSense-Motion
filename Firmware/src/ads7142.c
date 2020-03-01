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

static void waitReady() {
	while (DIO_DATA->ALIAS[PIN_READY] != 0);
    //while (DIO->DATA[PIN_READY] == 1);
}

ret_t ads7142_reset() {
	ret_t ret;

	waitReady();

	uint8_t data[1];
	data[0] = OP_DEVICE_RESET;
	for (int i = 1; i < 127; ++i) {
		if ((hal_error = HAL_I2C_Read(i, data, 1, true)) == HAL_OK) {
			return i;
		}
	}

    data[0] = OP_DEVICE_RESET;
	if ((hal_error = HAL_I2C_Write(
			ADS7142_I2C_ADDR, data, 1, false) != HAL_OK)) {
		return ADS7142_COMM_ERROR;
	}

    data[0] = OP_GENERAL_CALL;
    if ((hal_error = HAL_I2C_Write(
    		ADS7142_I2C_ADDR, data, 1, false) != HAL_OK))
    {
        return ADS7142_COMM_ERROR;
    }

    data[0] = OP_DEVICE_RESET;
    if ((hal_error = HAL_I2C_Write(
    		ADS7142_I2C_ADDR, data, 1, false) != HAL_OK))
    {
        return ADS7142_COMM_ERROR;
    }

//	if ((ret = ads7142_write_reg(
//			ADS7142_I2C_ADDR, REG_WKEY, VAL_WKEY_ENABLE_RESET)) != ADS7142_OK) {
//		return ADS7142_COMM_ERROR;
//	}
//
//	if ((ret = ads7142_set_bit(
//			ADS7142_I2C_ADDR, REG_DEVICE_RESET, VAL_DEVICE_RESET)) != ADS7142_OK) {
//		return ADS7142_COMM_ERROR;
//	}

	waitReady();

	return ADS7142_OK;
}

ret_t ads7142_check() {
	uint8_t value;
	ret_t ret;
//	if ((ret = ads7142_read_reg(ADS7142_I2C_ADDR, REG_WKEY, &value)) != ADS7142_OK) {
//		return ret;
//	}

	return ADS7142_OK;
}

int32_t ads7142_get_hal_error() {
	return hal_error;
}

static ret_t ads7142_reg_write(uint8_t reg, uint8_t val)
{

  int ret = 0;
//  uint8_t n = 0;    // counter to make several readings.
//  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//  printf("w reg ADS7142\n");
//
//
//  cmd = i2c_cmd_link_create();
//  i2c_master_start(cmd);
//  i2c_master_write_byte(cmd, ADS7142_I2C_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN); // read slave address
//  i2c_master_write_byte(cmd, SINGLE_WRITE, ACK_CHECK_EN); // read slave address
//  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);                            // slave register
//  i2c_master_write_byte(cmd, val, ACK_CHECK_EN);                                  // slave register
//  i2c_master_stop(cmd);
//  ret = i2c_master_cmd_begin(I2C_PORT_A, cmd, 1000 / portTICK_RATE_MS);
//
//
//  i2c_cmd_link_delete(cmd);


  return(ret);

}
static ret_t ads7142_reg_read(uint8_t reg){
  int ret = 0;
//  uint8_t val = 0;
//  static uint32_t count = 0;
//  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//  // INITIALIZATION OF THE DEVICE PART 1//
//  printf("r reg ADS7142\n");
//
//  gpio_set_level(SQUARE_SIGNAL_PIN, 1); // TO MEASURE TASK TIME //
//
//  cmd = i2c_cmd_link_create();
//  i2c_master_start(cmd);
//
//  i2c_master_write_byte(cmd, ADS7142_I2C_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);      // read slave address
//  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);                                    // slave register
//  i2c_master_write_byte(cmd, SINGLE_READ, ACK_CHECK_EN);                            // slave register
//
//  i2c_master_start(cmd);
//  i2c_master_write_byte(cmd, ADS7142_I2C_ADDR << 1 | READ_BIT, ACK_CHECK_EN);       // read slave address
//  i2c_master_read_byte(cmd, &val, ACK_CHECK_EN);                                    // slave register
//
//  i2c_master_stop(cmd);
//
//  ret = i2c_master_cmd_begin(I2C_PORT_A, cmd, 1000 / portTICK_RATE_MS);
//  //vTaskDelay(10);   // this should give the control to another task while the i2c thingie is executed
//  i2c_cmd_link_delete(cmd);
//
//  gpio_set_level(SQUARE_SIGNAL_PIN, 0); // TO MEASURE TASK TIME //
//
//  printf("%d\n",count);    // i2c crashing--> always ath the same place?
//  count += 1;

  return(ret);

}

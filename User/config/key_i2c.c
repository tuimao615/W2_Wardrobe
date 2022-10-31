#include "key_i2c.h"
#include "config.h"


void Key_I2C_Config(void)
{
	RCC_GPIOB_Enable();
	
	GPIO_Out_OD(KEY_I2C_GPIO_PORT, KEY_I2C_SCL);
	GPIO_Out_OD(KEY_I2C_GPIO_PORT, KEY_I2C_SDA);
	
	SCL_H;
	SDA_H;
}

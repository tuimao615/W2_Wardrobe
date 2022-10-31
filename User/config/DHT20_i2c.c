#include "DHT20_i2c.h"
#include "config.h"

void DHT20_I2C_Config(void)
{
	RCC_GPIOA_Enable();
	
	GPIO_Out_PP(DHT20_I2C_GPIO_PORT, DHT20_I2C_SCL);//GPIO_Out_PP //GPIO_Out_OD
	GPIO_Out_PP(DHT20_I2C_GPIO_PORT, DHT20_I2C_SDA);//GPIO_Out_PP //GPIO_Out_OD
	
	SCL_H;
	SDA_H;
}

void SDA_Output(void)
{
	GPIO_Out_PP(DHT20_I2C_GPIO_PORT, DHT20_I2C_SDA);
}
 
void SDA_Input(void)
{
	GPIO_In_FLOATING(DHT20_I2C_GPIO_PORT, DHT20_I2C_SDA);
}
#ifndef __DHT20_I2C_H
#define __DHT20_I2C_H

#include "sys.h"


#define DHT20_I2C_GPIO_PORT     		GPIOB

#define DHT20_I2C_SCL								GPIO_Pin_6
#define DHT20_I2C_SDA								GPIO_Pin_7


#define SCL_H         GPIO_SetBits(DHT20_I2C_GPIO_PORT, DHT20_I2C_SCL)
#define SDA_H         GPIO_SetBits(DHT20_I2C_GPIO_PORT, DHT20_I2C_SDA)

#define SCL_L         GPIO_ResetBits(DHT20_I2C_GPIO_PORT, DHT20_I2C_SCL)
#define SDA_L         GPIO_ResetBits(DHT20_I2C_GPIO_PORT, DHT20_I2C_SDA)

#define SDA_read()    GPIO_ReadInputDataBit(DHT20_I2C_GPIO_PORT, DHT20_I2C_SDA)


void DHT20_I2C_Config(void);

void SDA_Output(void);
void SDA_Input(void);

#endif

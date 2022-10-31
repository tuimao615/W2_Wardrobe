#ifndef __KEY_I2C_H
#define __KEY_I2C_H

#include "sys.h"


#define KEY_I2C_GPIO_PORT     		GPIOB

#define KEY_I2C_SCL								GPIO_Pin_6
#define KEY_I2C_SDA								GPIO_Pin_7


#define SCL_H         GPIO_SetBits(KEY_I2C_GPIO_PORT, KEY_I2C_SCL)//KEY_I2C_GPIO_PORT->BSRR = KEY_I2C_SCL
#define SDA_H         GPIO_SetBits(KEY_I2C_GPIO_PORT, KEY_I2C_SDA)//KEY_I2C_GPIO_PORT->BSRR = KEY_I2C_SDA

#define SCL_L         GPIO_ResetBits(KEY_I2C_GPIO_PORT, KEY_I2C_SCL)//KEY_I2C_GPIO_PORT->BRR  = KEY_I2C_SCL_PIN
#define SDA_L         GPIO_ResetBits(KEY_I2C_GPIO_PORT, KEY_I2C_SDA)//KEY_I2C_GPIO_PORT->BRR  = KEY_I2C_SDA_PIN

//ÓÃGPIO_ReadInputDataBit£¬²»ÓÃGPIO_ReadOutputDataBit
#define SDA_read()    GPIO_ReadInputDataBit(KEY_I2C_GPIO_PORT, KEY_I2C_SDA)//KEY_I2C_GPIO_PORT->IDR  & KEY_I2C_SDA

//#define SDA_IN() 				{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
//#define SDA_OUT() 			{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}

void Key_I2C_Config(void);

#endif

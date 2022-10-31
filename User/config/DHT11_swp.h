#ifndef __DHT11_SWP_H
#define __DHT11_SWP_H

#include "sys.h"


#define DHT11_SWP_GPIO_PORT     		GPIOA
#define DHT11_SWP_PIN								GPIO_Pin_11


#define DHT11_H         GPIO_SetBits(DHT11_SWP_GPIO_PORT, DHT11_SWP_PIN)
#define DHT11_L         GPIO_ResetBits(DHT11_SWP_GPIO_PORT, DHT11_SWP_PIN)

#define DHT11_read()    GPIO_ReadInputDataBit(DHT11_SWP_GPIO_PORT, DHT11_SWP_PIN)


void DHT11_SWP_Config(void);

void DHT11_SWP_Output(void);
void DHT11_SWP_Input(void);

#endif

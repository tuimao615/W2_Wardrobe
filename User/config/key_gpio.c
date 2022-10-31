#include "key_gpio.h"
#include "config.h"

#define Key_GPIO_Reset_Pressed()    	(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_RESET_PIN) == 0)


void Key_GPIO_Config(void)
{
	RCC_GPIOB_Enable();
	GPIO_In_PU(KEY_GPIO_PORT, KEY_RESET_PIN);
}

MmiKey Key_GPIO_Get_Key(void)
{
	if(Key_GPIO_Reset_Pressed())
		return KEY_RESET;
	
	return KEY_NONE;
}

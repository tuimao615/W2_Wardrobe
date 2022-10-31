#ifndef __KEY_GPIO_H
#define __KEY_GPIO_H

#include "sys.h"
#include "type_def.h"


#define KEY_RCC_AHB_GPIO			RCC_AHBPeriph_GPIOB
#define KEY_GPIO_PORT     		GPIOB

#define	KEY_RESET_PIN					GPIO_Pin_5

void Key_GPIO_Config(void);
MmiKey Key_GPIO_Get_Key(void);

#endif

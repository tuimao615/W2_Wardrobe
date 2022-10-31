#include "seg_gpio.h"
#include "config.h"


void SEG_Config(void)
{
	RCC_GPIOA_Enable();
	RCC_GPIOB_Enable();
	
	GPIO_Out_PP(GPIOA, SEG_HT1621_CS | SEG_HT1621_RD | SEG_HT1621_WR);
	GPIO_Out_PP(GPIOB, SEG_HT1621_DATA);
}

void SEG_BL_Config()
{
	RCC_GPIOB_Enable();
	GPIO_Out_PP(SEG_BL_GPIO, SEG_BL_GPIO_PIN);
}

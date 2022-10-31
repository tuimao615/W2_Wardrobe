#ifndef __SLED_SPI_H
#define __SLED_SPI_H

#include "sys.h"
#include "type_def.h"

#define SLED_USE_SPI1
//#define SLED_USE_SPI2

#ifdef SLED_USE_SPI1
	#define SLED_SPI_RCC_AHB_GPIO			RCC_AHBPeriph_GPIOA
	#define SLED_SPI_GPIO_PORT     		GPIOA
#endif

#define SLED_BL_RCC_AHB_GPIO			 	RCC_AHBPeriph_GPIOB
#define SLED_BL_GPIO				       	GPIOB
#define SLED_BL_GPIO_PIN    			 	GPIO_Pin_1

#define SEG_BL_SET()								GPIO_WriteBit(SEG_BL_GPIO,SEG_BL_GPIO_PIN,Bit_SET)
#define SEG_BL_RESET()							GPIO_WriteBit(SEG_BL_GPIO,SEG_BL_GPIO_PIN,Bit_RESET)


void SLED_BL_Config(void);
void SLED_SPI_Init(void);

//void SLED_SPI_show_num(u8 pos, u8 num);
//void SLED_SPI_turn(LedLight light, u8 on);
		 
#endif

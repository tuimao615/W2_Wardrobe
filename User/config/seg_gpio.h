#ifndef __SEG_GPIO_H
#define __SEG_GPIO_H

#include "sys.h"


#define SEG_HT1621_CS					GPIO_Pin_5
#define SEG_HT1621_RD					GPIO_Pin_6
#define SEG_HT1621_WR					GPIO_Pin_7
#define SEG_HT1621_DATA				GPIO_Pin_0

#define CS_H		GPIO_SetBits(GPIOA, SEG_HT1621_CS)
#define RD_H		GPIO_SetBits(GPIOA, SEG_HT1621_RD)
#define WR_H		GPIO_SetBits(GPIOA, SEG_HT1621_WR)
#define DATA_1	GPIO_SetBits(GPIOB, SEG_HT1621_DATA)

#define CS_L		GPIO_ResetBits(GPIOA, SEG_HT1621_CS)
#define RD_L		GPIO_ResetBits(GPIOA, SEG_HT1621_RD)
#define WR_L		GPIO_ResetBits(GPIOA, SEG_HT1621_WR)
#define DATA_0	GPIO_ResetBits(GPIOB, SEG_HT1621_DATA)


#define SEG_BL_GPIO				    GPIOB
#define SEG_BL_GPIO_PIN    		GPIO_Pin_1

#define SEG_BL_ON()		GPIO_WriteBit(SEG_BL_GPIO,SEG_BL_GPIO_PIN,Bit_SET)
#define SEG_BL_OFF()	GPIO_WriteBit(SEG_BL_GPIO,SEG_BL_GPIO_PIN,Bit_RESET)


void SEG_Config(void);
void SEG_BL_Config(void);

#endif

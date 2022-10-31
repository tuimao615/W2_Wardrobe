#ifndef __DEVICE_GPIO_H
#define __DEVICE_GPIO_H

#include "sys.h"

//#define LIGHT_GPIO        GPIOB
//#define LIGHT_GPIO_PIN    GPIO_Pin_8

#define HEAT_GPIO       	GPIOB					//加热丝1
#define HEATONE_GPIO_PIN   	GPIO_Pin_9				
#define HEATTWO_GPIO_PIN   	GPIO_Pin_1				//加热丝2

#define FAN_GPIO        	GPIOA					//风扇
#define FAN_GPIO_PIN 			GPIO_Pin_5

#define FLZ_GPIO					GPIOA			//负离子
#define FLZ_GPIO_PIN			GPIO_Pin_4

#define CY_GPIO						GPIOA			//臭氧
#define CY_GPIO_PIN				GPIO_Pin_6

#define XX_GPIO						GPIOA			//香薰
#define XX_GPIO_PIN				GPIO_Pin_11

#define UV_GPIO		    		GPIOB				//紫外线检测
#define UV_GPIO_PIN     	GPIO_Pin_14

#define UV_LED_GPIO				GPIOB				//紫外灯
#define UV_LED_GPIO_PIN		GPIO_Pin_15

#define BUZ_GPIO					GPIOA			//蜂鸣器
#define BUZ_GPIO_PIN			GPIO_Pin_12


#define PIR_GPIO				GPIOB				//红外感应
#define PIR_GPIO_PIN			GPIO_Pin_8


//#define LIGHT_ON()				GPIO_WriteBit(LIGHT_GPIO,LIGHT_GPIO_PIN,Bit_SET)
//#define LIGHT_OFF()				GPIO_WriteBit(LIGHT_GPIO,LIGHT_GPIO_PIN,Bit_RESET)

#define HEATONE_ON()     	GPIO_WriteBit(HEAT_GPIO,HEATONE_GPIO_PIN,Bit_SET)
#define HEATONE_OFF()     	GPIO_WriteBit(HEAT_GPIO,HEATONE_GPIO_PIN,Bit_RESET)

#define HEATTWO_ON()     	GPIO_WriteBit(HEAT_GPIO,HEATTWO_GPIO_PIN,Bit_SET)
#define HEATTWO_OFF()     	GPIO_WriteBit(HEAT_GPIO,HEATTWO_GPIO_PIN,Bit_RESET)

#define FAN_ON()     		GPIO_WriteBit(FAN_GPIO,FAN_GPIO_PIN,Bit_SET)
#define FAN_OFF()     	GPIO_WriteBit(FAN_GPIO,FAN_GPIO_PIN,Bit_RESET)

#define FLZ_ON()     		GPIO_WriteBit(FLZ_GPIO,FLZ_GPIO_PIN,Bit_SET)
#define FLZ_OFF()   	  GPIO_WriteBit(FLZ_GPIO,FLZ_GPIO_PIN,Bit_RESET)

#define CY_ON()     		GPIO_WriteBit(CY_GPIO,CY_GPIO_PIN,Bit_SET)
#define CY_OFF()   			GPIO_WriteBit(CY_GPIO,CY_GPIO_PIN,Bit_RESET)

#define XX_ON()     		GPIO_WriteBit(XX_GPIO,XX_GPIO_PIN,Bit_SET)
#define XX_OFF()   	  	GPIO_WriteBit(XX_GPIO,XX_GPIO_PIN,Bit_RESET)

#define UV_ON()     		GPIO_WriteBit(UV_GPIO,UV_GPIO_PIN,Bit_SET)
#define UV_OFF()   	  	GPIO_WriteBit(UV_GPIO,UV_GPIO_PIN,Bit_RESET)

#define UV_LED_ON()     GPIO_WriteBit(UV_LED_GPIO,UV_LED_GPIO_PIN,Bit_RESET)
#define UV_LED_OFF()   	GPIO_WriteBit(UV_LED_GPIO,UV_LED_GPIO_PIN,Bit_SET)

#define BUZ_ON()     		GPIO_WriteBit(BUZ_GPIO,BUZ_GPIO_PIN,Bit_SET)
#define BUZ_OFF()   	  GPIO_WriteBit(BUZ_GPIO,BUZ_GPIO_PIN,Bit_RESET)


void Device_GPIO_Config(void);

#endif

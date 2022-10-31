#ifndef __JW01_USART_H
#define __JW01_USART_H

#include "sys.h"
#include "config.h"


#define JW01_USE_USART2
//#define JW01_USE_USART3


#if defined (JW01_USE_USART3)
	
	#define JW01_USART							USART3
	#define JW01_USART_IRQn					USART3_IRQn
	#define JW01_USART_IRQ_HANDLER 	void USART3_IRQHandler(void)
 
	#define JW01_USART_GPIO    									GPIOB
	#define JW01_USART_TX_PIN		       					GPIO_Pin_10
	#define JW01_USART_RX_PIN      		 					GPIO_Pin_11

	#define JW01_USART_TX_GPIO_PinSource		    GPIO_PinSource10
	#define JW01_USART_RX_GPIO_PinSource		    GPIO_PinSource11	
	
	#define JW01_POWER_GPIO    									GPIOB
	#define JW01_POWER_PIN											GPIO_Pin_13
	
#elif defined (JW01_USE_USART2)

	#define JW01_USART							USART2
	#define JW01_USART_IRQn					USART2_IRQn
	#define JW01_USART_IRQ_HANDLER 	void USART2_IRQHandler(void)
	
	#define JW01_USART_GPIO    									GPIOA
	#define JW01_USART_TX_PIN		       					GPIO_Pin_2
	#define JW01_USART_RX_PIN      		 					GPIO_Pin_3

	#define JW01_USART_TX_GPIO_PinSource		    GPIO_PinSource2
	#define JW01_USART_RX_GPIO_PinSource		    GPIO_PinSource3	
	
	#define JW01_POWER_GPIO    									GPIOA
	#define JW01_POWER_PIN											GPIO_Pin_7

#endif 


void JW01_USART_Config(void);
void JW01_HW_Poweron(void);
void JW01_USART_Enable_Rx(void);
	
#endif

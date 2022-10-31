#ifndef __BT_USART_H
#define __BT_USART_H

#include "stdio.h"
#include "stm32f0xx.h"

#define BT_USE_USART1
//#define BT_USE_USART2

#if !defined (BT_USE_USART1) && !defined (BT_USE_USART2)
	#error "Please select which USART port using by bluetooth"
#endif


#if defined (BT_USE_USART1)

	#define	WIFI_RCC_AHB_GPIO								RCC_AHBPeriph_GPIOA
	#define BT_USART_GPIO    								GPIOA  
	#define BT_USART_TX_GPIO_PinSource			GPIO_PinSource9
	#define BT_USART_RX_GPIO_PinSource		  GPIO_PinSource10

	#define BT_USART_TX_PIN									GPIO_Pin_9
	#define BT_USART_RX_PIN	       		 			GPIO_Pin_10

	#define BT_USART							USART1
	#define BT_USART_IRQ					USART1_IRQn
	#define BT_USART_IRQ_HANDLER 	void USART1_IRQHandler(void)

#elif defined (BT_USE_USART2)

	#define	WIFI_RCC_AHB_GPIO								RCC_AHBPeriph_GPIOA
	#define BT_USART_GPIO    								GPIOA   
	#define BT_USART_TX_GPIO_PinSource			GPIO_PinSource2
	#define BT_USART_RX_GPIO_PinSource			GPIO_PinSource3	

	#define BT_USART_TX_PIN		       				GPIO_Pin_2
	#define BT_USART_RX_PIN	       		 			GPIO_Pin_3

	#define BT_USART							USART2
	#define BT_USART_IRQ					USART2_IRQn
	#define BT_USART_IRQ_HANDLER 	void USART2_IRQHandler(void)

#endif


void BT_USART_Config(void);

#endif

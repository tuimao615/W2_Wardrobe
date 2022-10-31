#ifndef __WIFI_USART_H
#define __WIFI_USART_H

//#include "stdio.h"
#include "sys.h"
#include "config.h"


#define WIFI_USE_USART1
//#define WIFI_USE_USART2

#if !defined (WIFI_USE_USART1) && !defined (WIFI_USE_USART2)
	#error "Please select which USART port using by Wi-Fi"
#endif



#if defined (WIFI_USE_USART1)
	
	#define WIFI_USART							USART1
	#define WIFI_USART_IRQn					USART1_IRQn
	#define WIFI_USART_IRQ_HANDLER 	void USART1_IRQHandler(void)
 
	#define WIFI_USART_GPIO    									GPIOA
	#define WIFI_USART_TX_PIN		       					GPIO_Pin_9
	#define WIFI_USART_RX_PIN      		 					GPIO_Pin_10

	#define WIFI_USART_TX_GPIO_PinSource		    GPIO_PinSource9
	#define WIFI_USART_RX_GPIO_PinSource		    GPIO_PinSource10	
	
	#define WIFI_POWER_GPIO    									GPIOC
	#define WIFI_POWER_PIN											GPIO_Pin_13

#elif defined (WIFI_USE_USART2)

	#define WIFI_USART							USART2
	#define WIFI_USART_IRQn					USART2_IRQn
	#define WIFI_USART_IRQ_HANDLER 	void USART2_IRQHandler(void)
	
	#define WIFI_USART_GPIO    									GPIOA
	#define WIFI_USART_TX_PIN		       					GPIO_Pin_2
	#define WIFI_USART_RX_PIN      		 					GPIO_Pin_3

	#define WIFI_USART_TX_GPIO_PinSource		    GPIO_PinSource2
	#define WIFI_USART_RX_GPIO_PinSource		    GPIO_PinSource3	
	
	#define WIFI_POWER_GPIO    									GPIOC
	#define WIFI_POWER_PIN											GPIO_Pin_13

#endif 


void WIFI_USART_Config(void);
void WIFI_USART_Enable_Rx(void);
	
void WIFI_HW_Poweron(void);

#if defined (USE_HF_LPC330)
	void WIFI_HW_Reset(void);
	void WIFI_HW_SmartBleLink(void);
	void WIFI_HW_Reload_FactorySet(void);
#endif
	
#endif

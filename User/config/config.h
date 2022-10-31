#ifndef __CONFIG_H
#define __CONFIG_H

#include "sys.h"


#define USE_TUYA_MODULE


#ifdef USE_TUYA_MODULE
	#define USE_TUYA_WBR1
#else
	#define USE_HF_LPC330
#endif

#if !defined (USE_HF_LPC330) && !defined (USE_TUYA_WBR1)
	#error "Please select using which Wi-Fi module"
#endif


void RCC_GPIOA_Enable(void);
void RCC_GPIOB_Enable(void);
void RCC_GPIOC_Enable(void);

void GPIO_In_PU(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_In_PD(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_In_FLOATING(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Out_PP(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Out_OD(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_AF_PP(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_AF_OD(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void USART_Setup(USART_TypeDef* USARTx, uint32_t BaudRate);
void USART_Enable(USART_TypeDef* USARTx);
	
void NVIC_Enable(uint8_t channel, uint8_t priority, uint8_t sub_priority);

#endif

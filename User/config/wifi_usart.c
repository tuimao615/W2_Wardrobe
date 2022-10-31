#include "wifi_usart.h"
#include "config.h"

#include "delay.h"


void WIFI_USART_Config(void)
{    
	//ʹ��ʱ��
#if defined (WIFI_USE_USART1)
	RCC_GPIOA_Enable();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#elif defined (WIFI_USE_USART2)
	RCC_GPIOA_Enable();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	  
#endif
		
	//����GPIO
	GPIO_AF_PP(WIFI_USART_GPIO, WIFI_USART_TX_PIN);
	GPIO_In_FLOATING(WIFI_USART_GPIO, WIFI_USART_RX_PIN);
	
	RCC_GPIOC_Enable();
	GPIO_Out_PP(WIFI_POWER_GPIO, WIFI_POWER_PIN);
	
	//����USART
	USART_Setup(WIFI_USART, 115200);

	//ʹ��USART
	USART_Enable(WIFI_USART);
		
	//�����ж�
	NVIC_Enable(WIFI_USART_IRQn, 1, 0);
}


void WIFI_USART_Enable_Rx(void)
{
	USART_ITConfig(WIFI_USART, USART_IT_RXNE, ENABLE);
}

void WIFI_HW_Poweron(void)
{ 
	GPIO_ResetBits(WIFI_POWER_GPIO, WIFI_POWER_PIN);
	delay_ms(500);
	GPIO_SetBits(WIFI_POWER_GPIO, WIFI_POWER_PIN);
}

#if defined (USE_HF_LPC330)

void WIFI_HW_Reload_FactorySet(void)
{
	int i;
	for(i=0; i<5; i++)
		delay_ms(1000);
	GPIO_ResetBits(WIFI_USART_GPIO, WIFI_RELOAD_PIN);
	for(i=0; i<5; i++)
		delay_ms(1000);
	GPIO_SetBits(WIFI_USART_GPIO, WIFI_RELOAD_PIN);
}

void WIFI_HW_SmartBleLink(void)
{
	GPIO_ResetBits(WIFI_USART_GPIO, WIFI_RELOAD_PIN);
	delay_ms(500);
	GPIO_SetBits(WIFI_USART_GPIO, WIFI_RELOAD_PIN);
	
	delay_ms(1000);
	
  GPIO_ResetBits(WIFI_USART_GPIO, WIFI_RELOAD_PIN);
	delay_ms(500);
	GPIO_SetBits(WIFI_USART_GPIO, WIFI_RELOAD_PIN);
}

void WIFI_HW_Reset(void)
{
	GPIO_ResetBits(WIFI_USART_GPIO, WIFI_RESET_PIN);
	delay_ms(100);
  GPIO_SetBits(WIFI_USART_GPIO, WIFI_RESET_PIN);
}

#endif

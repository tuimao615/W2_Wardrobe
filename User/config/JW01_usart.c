#include "jw01_usart.h"

#include "delay.h"


void JW01_USART_Config(void)
{    
	//ʹ��ʱ��
#if defined (JW01_USE_USART3)
	RCC_GPIOB_Enable();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	  
#elif defined (JW01_USE_USART2)
	RCC_GPIOA_Enable();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	  
#endif
		
	//����GPIO
	GPIO_AF_PP(JW01_USART_GPIO, JW01_USART_TX_PIN);
	GPIO_In_FLOATING(JW01_USART_GPIO, JW01_USART_RX_PIN);//����3 RX �� GPIO_In_FLOATING��
	
	RCC_GPIOC_Enable();
	GPIO_Out_PP(JW01_POWER_GPIO, JW01_POWER_PIN);
	
	//����USART
	USART_Setup(JW01_USART, 9600);//JW01���ڲ�����9600

	//ʹ��USART
	USART_Enable(JW01_USART);
		
	//�����ж�
	NVIC_Enable(JW01_USART_IRQn, 1, 1);
}

void JW01_HW_Poweron(void)
{
	GPIO_ResetBits(JW01_POWER_GPIO, JW01_POWER_PIN);
	delay_ms(500);
	GPIO_SetBits(JW01_POWER_GPIO, JW01_POWER_PIN);
}

void JW01_USART_Enable_Rx(void)
{
	USART_ITConfig(JW01_USART, USART_IT_RXNE, ENABLE);
}

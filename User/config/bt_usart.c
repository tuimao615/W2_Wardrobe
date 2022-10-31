#include "bt_usart.h"


void BT_USART_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		//使能时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	#if defined (BT_USE_USART1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	#elif defined (BT_USE_USART2)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	  
	#endif

		//设置GPIO
    GPIO_PinAFConfig(BT_USART_GPIO, BT_USART_TX_GPIO_PinSource, GPIO_AF_1);
    GPIO_PinAFConfig(BT_USART_GPIO, BT_USART_RX_GPIO_PinSource, GPIO_AF_1);
	
		GPIO_InitStructure.GPIO_Pin = BT_USART_TX_PIN | BT_USART_RX_PIN;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(BT_USART_GPIO, &GPIO_InitStructure);		   
	
    //设置USART
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(BT_USART, &USART_InitStructure);

		//使能USART
		USART_ClearFlag(BT_USART,USART_FLAG_TC);	
    USART_ITConfig(BT_USART, USART_IT_RXNE, ENABLE);
    USART_ITConfig(BT_USART, USART_IT_TXE, ENABLE);		
		USART_Cmd(BT_USART, ENABLE);
		
		//设置中断
		NVIC_InitStructure.NVIC_IRQChannel = BT_USART_IRQ;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
}

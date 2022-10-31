#include "sled_spi.h"



//Segment

void SLED_SPI_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	//Clock
	RCC_AHBPeriphClockCmd( SLED_SPI_RCC_AHB_GPIO, ENABLE );
#ifdef SLED_USE_SPI1
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1, ENABLE );
#endif
 		
	//GPIO����
#ifdef SLED_USE_SPI1
	GPIO_PinAFConfig(SLED_SPI_GPIO_PORT,GPIO_PinSource5,GPIO_AF_0);        
	GPIO_PinAFConfig(SLED_SPI_GPIO_PORT,GPIO_PinSource6,GPIO_AF_0);       
	GPIO_PinAFConfig(SLED_SPI_GPIO_PORT,GPIO_PinSource7,GPIO_AF_0);
#endif
	
	//GPIO
#ifdef SLED_USE_SPI1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  						
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SLED_SPI_GPIO_PORT, &GPIO_InitStructure);
	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);					//��λSPI1        
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);				//ֹͣ��λSPI1    

	SPI_I2S_DeInit(SPI1);    //���Ĵ�������Ϊȱʡֵ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  								//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
  
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);    		//��Ҫ����Ӧ������λ����Ϊ 8 λ
	
	SPI_Cmd(SPI1, ENABLE); 												//ʹ��SPI����	 
}


//Backlight

void SLED_BL_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd( SLED_BL_RCC_AHB_GPIO, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = SLED_BL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SLED_BL_GPIO, &GPIO_InitStructure);
}


//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SLED_SPI_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 
} 

//SPIx ��дһ���ֽ�
uint8_t SLED_SPI_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_SendData8(SPI1, TxData); 									//ͨ������SPIx����һ������
	
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_ReceiveData8(SPI1); 									//����ͨ��SPIx������յ�����					    
}


/* API */

void SLED_SPI_Init(void)
{
	SLED_SPI_Config();
	SLED_SPI_SetSpeed(SPI_BaudRatePrescaler_2);
}

void SLED_SPI_show_num(u8 pos, u8 num)
{
	SLED_SPI_ReadWriteByte(1);
	SLED_SPI_ReadWriteByte(pos);
	
	SLED_SPI_ReadWriteByte(00);
	SLED_SPI_ReadWriteByte(00);
	SLED_SPI_ReadWriteByte(00);
}

void SLED_SPI_turn(LedLight light, u8 on)
{
	u8 command;
	switch(light)
	{
		case LED_DOT: command=1; break;
		case LED_TEMPERATURE: command=1; break;
		case LED_HUMIDITY: command=1; break;
		
		case LED_BT: command=1; break;
		case LED_WIFI: command=1; break;
		case LED_CLOCK: command=1; break;
		case LED_ALARM: command=1; break;
		
		case LED_MODE_ZIDONG: command=1; break;
		case LED_MODE_HONGGAN: command=2; break;
		case LED_MODE_SHAJUN: command=3; break;
		case LED_MODE_XIANGXUN: command=4; break;
		case LED_MODE_JINGHUA: command=5; break;
		default:
			return;
	}
	
	
	SLED_SPI_ReadWriteByte(command);
	if(on)
		SLED_SPI_ReadWriteByte(1);
	else
		SLED_SPI_ReadWriteByte(0);
}

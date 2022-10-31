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
 		
	//GPIO复用
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
	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);					//复位SPI1        
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);				//停止复位SPI1    

	SPI_I2S_DeInit(SPI1);    //将寄存器重设为缺省值
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//串行同步时钟的空闲状态为高电平
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  								//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
  
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);    		//重要，把应答数据位设置为 8 位
	
	SPI_Cmd(SPI1, ENABLE); 												//使能SPI外设	 
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


//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SLED_SPI_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); 
} 

//SPIx 读写一个字节
uint8_t SLED_SPI_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_SendData8(SPI1, TxData); 									//通过外设SPIx发送一个数据
	
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_ReceiveData8(SPI1); 									//返回通过SPIx最近接收的数据					    
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

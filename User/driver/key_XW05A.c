#include "key_XW05A.h"
#include "key_i2c.h"

#include "delay.h"

static void I2C_delay(void)
{
	delay_us(1000);
}

//产生I2C起始信号
static FunctionalState I2C_Start(void)
{
	SCL_H;
	
	SDA_H;
	I2C_delay();
	if(!SDA_read()) return DISABLE;	/* SDA线为低电平则总线忙,退出 */
	
	SDA_L;
	I2C_delay();
	if(SDA_read()) return DISABLE;	/* SDA线为高电平则总线出错,退出 */
	
	SDA_L;
	delay_us(10);
	return ENABLE;
}	  

//产生I2C停止信号
static void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	
	SDA_L;
	I2C_delay();
	
	SCL_H;
	I2C_delay();
	
	SDA_H;
	I2C_delay();				   	
}

//产生ACK应答
//static void I2C_Ack(void)
//{
//	SCL_L;
//	I2C_delay();
//	SDA_L;
//	I2C_delay();
//	SCL_H;
//	I2C_delay();
//	SCL_L;
//	I2C_delay();
//}

//不产生ACK应答		    
static void I2C_NAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}			


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static u8 I2C_Wait_Ack(void)
{
	SCL_L;
	I2C_delay();
	
	SDA_H;			
	I2C_delay();
	
	SCL_H;
	I2C_delay();
	
	if(SDA_read())
	{
		SCL_L;
		return DISABLE;
	}
	
	SCL_L;
	
	return ENABLE;
} 

//发送一个字节	  
static void I2C_Send_Byte(u8 SendByte)
{      
	uint8_t i=8;
	while(i--)
	{
		SCL_L;
		I2C_delay();
		
		if(SendByte&0x80)
			SDA_H;  
		else 
			SDA_L;  
		I2C_delay();
		
		SCL_H;
		I2C_delay();
		
		SendByte<<=1;
	}
	SCL_L; 
} 	 

//读取一个字节
static u8 I2C_Read_Byte(void)
{
	uint8_t RecvByte=0;

	SDA_H;
	
	uint8_t i=8;
	while(i--)
	{
		RecvByte<<=1; 
		
		SCL_L;
		I2C_delay();
		
		SCL_H;
		I2C_delay();
		
		if(SDA_read())
		{
			RecvByte|=0x01;
		}
	}
	
	SCL_L;
	
	return RecvByte;
}


unsigned char Key_I2C_Get_Keys(void)
{
	unsigned char keys = 0;
	
	if(!I2C_Start())
		return 0;
	
	I2C_Send_Byte(0x83);
	if(!I2C_Wait_Ack())
		return 0;
	
	keys = I2C_Read_Byte();
	I2C_NAck();
	
	I2C_Stop();
	
	return ~keys;
}

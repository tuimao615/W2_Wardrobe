#include "sensor_DHT11.h"
#include "DHT11_swp.h"

//#include "delay.h"

#define TRY_NUMBER 20


//数据定义:
//----以下变量均为全局变量--------
//----温度高8位== Temp_H------
//----温度低8位== Temp_L------
//----湿度高8位== Humi_H-----
//----湿度低8位== Humi_L-----
//----校验 8位 == checkdata-----
static unsigned char Temp_H ,Temp_L,Humi_H,Humi_L,checkdata;
static unsigned char Temp_H_temp,Temp_L_temp,Humi_H_temp,Humi_L_temp,checkdata_temp;


void Sensor_DHT11_Init(void)
{
	DHT11_SWP_Config();
}


unsigned char DHT11_get_temperature(void)
{
	return Temp_H;
}

unsigned char DHT11_get_humidity(void)
{
	return Humi_H;
}

static void delay_us(unsigned int us)
{
	uint16_t	i,j;
	for(i=0;i<us;i++)
		for(j=0;j<6;j++) 
			__nop();
}

static void delay_ms(unsigned int ms)
{
	uint16_t	i;
	for(i=0;i<ms;i++)
		delay_us(833);
}


static unsigned char DHT11_read_byte(void)
{
	unsigned char i, count;
	unsigned char data = 0;

	for(i = 0; i < 8; i++)
	{
		data <<= 1;
		
		count = 0;
		while((!DHT11_read()))
		{
			delay_us(10);
			if(count++ >= 5)
			{
				return 0;
			}
		}

		delay_us(30);
 
		if(DHT11_read()){
			data |= 0x1;
			
			count = 0;
			while(DHT11_read())
			{
				delay_us(10);
				if(count++ >= 4)
				{
					return 0;
				}
			}
		}
		else{
		} 
	}
	return data;
}

static uint8_t DHT11_start_sampling(void)
{
	unsigned char count;
	
	DHT11_SWP_Output();
	
	//主机拉低18ms   
	DHT11_L;
	delay_ms(20);
	
	//总线由上拉电阻拉高 主机延时20us
	DHT11_H;
	delay_us(40);

	//主机设为输入 判断从机响应信号 
	//DHT11_H; 
	DHT11_SWP_Input();
 
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行      
	if(!DHT11_read()) //T!
	{
		//判断从机是否发出 80us 的低电平响应信号是否结束    
		count = 0;
		while((!DHT11_read()))
		{
			delay_us(10);
			if(count++ >= 10)
			{
				return 0;
			}
		}
		// printf("DHT11 answers.\r\n");
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		count = 0;
		while((DHT11_read()))
		{
			delay_us(10);
			if(count++ >= 10)
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
 
void DHT11_update(void)
{
	unsigned char temp;
	if(DHT11_start_sampling())
	{
		//数据接收状态         
		Humi_H_temp = DHT11_read_byte();
		Humi_L_temp = DHT11_read_byte();
		Temp_H_temp = DHT11_read_byte();
		Temp_L_temp = DHT11_read_byte();
		checkdata_temp = DHT11_read_byte();
		
		/* Data transmission finishes, pull the bus high */
		DHT11_SWP_Output();
		DHT11_H;

		//数据校验 		
		temp=(Temp_H_temp+Temp_L_temp+Humi_H_temp+Humi_L_temp);
		if(temp == checkdata_temp)
		{
			Humi_H=Humi_H_temp;
			Humi_L=Humi_L_temp;
			Temp_H=Temp_H_temp;
			Temp_L=Temp_L_temp;
			checkdata=checkdata_temp;
		
			//printf("DHT11 tempature %d.%d  humidity %d.%d \r\n",Temp_H,Temp_L,Humi_H,Temp_L);
			//update
		
		}else{
			//printf("checksum failure \r\n");
		} 
	}
	else{
		//printf("DHT11 didn't answer. Sampling failed. \r\n");
	}
}

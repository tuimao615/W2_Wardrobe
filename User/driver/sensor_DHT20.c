/*******************************************/
/*@版权所有：广州奥松电子有限公司          */
/*@作者：温湿度传感器事业部                */
/*@版本：V1.2                              */
/*******************************************/
#include "sensor_DHT20.h" 
#include "DHT20_i2c.h"

#include "delay.h"
#include "usart.h"
#include "stdio.h"


static int Humidity, Temperature;


void DHT20_update(void)
{
  uint32_t CT_data[2];
	//AHT20_Read_CTdata(CT_data);       //不经过CRC校验，直接读取AHT20的温度和湿度数据    推荐每隔大于1S读一次
  if(AHT20_Read_CTdata_crc(CT_data))  //crc校验后，读取AHT20的温度和湿度数据 
	{
		Humidity = CT_data[0]*100/1024/1024;  //计算得到湿度值c1（放大了10倍）
		Temperature = CT_data[1]*200/1024/1024-50;//计算得到温度值t1（放大了10倍）
	}
	else
	{
		Humidity = 0;
		Temperature = 0;
	}
 }	

int DHT20_get_temperature(void)
{
	return Temperature;
}

int DHT20_get_humidity(void)
{
	return Humidity;
}


//static void delay_us(unsigned int us)
//{
//	uint16_t	i,j;
//	for(i=0;i<us;i++)
//		for(j=0;j<6;j++) 
//			__nop();
//}

//static void delay_ms(unsigned int ms)
//{
//	uint16_t	i;
//	for(i=0;i<ms;i++)
//		delay_us(833);
//}

static void I2C_Start(void)		 //I2C主机发送START信号
{
	SDA_Output();
	SDA_H;
	delay_us(8);
	SCL_H;
	delay_us(8);
	SDA_L;
	delay_us(8);
	SCL_L;
	delay_us(8);   
}

static void I2C_Stop(void)	  //一条协议结束
{
	SDA_Output();
	SDA_L;
	delay_us(8);
	SCL_H;	
	delay_us(8);
	SDA_H;
	delay_us(8);
}

static uint8_t Receive_ACK(void)   //看AHT20是否有回复ACK
{
	uint16_t CNT;
	CNT = 0;
	
	SCL_L;	
	SDA_Input();
	delay_us(8);	
	SCL_H;	
	delay_us(8);	
	while((SDA_read())  && CNT < 100) 
		CNT++;
	if(CNT == 100)
	{
		return 0;
	}
 	SCL_L;	
	delay_us(8);	
	return 1;
}

static void Send_ACK(void)		  //主机回复ACK信号
{
	SCL_L;	
	delay_us(8);	
	SDA_Output();
	SDA_L;
	delay_us(8);	
	SCL_H;	
	delay_us(8);
	SCL_L;	
	delay_us(8);
	SDA_Input();
	delay_us(8);
}

static void Send_NOT_ACK(void)	//主机不回复ACK
{
	SCL_L;	
	delay_us(8);
	SDA_Output();
	SDA_H;
	delay_us(8);
	SCL_H;	
	delay_us(8);		
	SCL_L;	
	delay_us(8);
	SDA_L;
	delay_us(8);
}


static void AHT20_WR_Byte(uint8_t Byte) //往AHT20写一个字节
{
	uint8_t Data,N,i;	
	Data=Byte;
	i = 0x80;
	SDA_Output();
	for(N=0;N<8;N++)
	{
		SCL_L; 
		delay_us(4);	
		if(i&Data)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}	
			
    SCL_H;
		delay_us(4);
		Data <<= 1;
		 
	}
	SCL_L;
	delay_us(8);   
	SDA_Input();
	delay_us(8);	
}	


static uint8_t AHT20_RD_Byte(void)//从AHT20读取一个字节
{
	uint8_t Byte,i,a;
	Byte = 0;
	SCL_L;
	SDA_Input();
	delay_us(8);	
	for(i=0;i<8;i++)
	{
    SCL_H;		
		delay_us(5);
		a=0;
		if(SDA_read()) a=1;
		Byte = (Byte<<1)|a;
		SCL_L;
		delay_us(5);
	}
  //SDA_Input();
	delay_us(8);	
	return Byte;
}



uint8_t AHT20_Read_Status(void)//读取AHT20的状态寄存器
{
	uint8_t Byte_first;	
	
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_NOT_ACK();
	I2C_Stop();
	
	return Byte_first;
}

uint8_t AHT20_Read_Cal_Enable(void)  //查询cal enable位有没有使能
{
	uint8_t val = 0;//ret = 0,
  val = AHT20_Read_Status();
	 if((val & 0x68)==0x08)
		 return 1;
   else  return 0;
 }

void AHT20_SendAC(void) //向AHT20发送AC命令
{
	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xac);//0xAC采集命令
	Receive_ACK();
	AHT20_WR_Byte(0x33);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	I2C_Stop();
}

//CRC校验类型：CRC8/MAXIM
//多项式：X8+X5+X4+1
//Poly：0011 0001  0x31
//高位放到后面就变成 1000 1100 0x8c
//C现实代码：
uint8_t Calc_CRC8(uint8_t *message,uint8_t Num)
{
	uint8_t i;
	uint8_t byte;
	uint8_t crc=0xFF;
  for(byte=0; byte<Num; byte++)
  {
    crc^=(message[byte]);
    for(i=8;i>0;--i)
    {
      if(crc&0x80) crc=(crc<<1)^0x31;
      else crc=(crc<<1);
    }
  }
  return crc;
}

void AHT20_Read_CTdata(uint32_t *ct) //没有CRC校验，直接读取AHT20的温度和湿度数据
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	uint32_t RetuData;
	uint16_t cnt;
	
	AHT20_SendAC();//向AHT10发送AC命令
	delay_ms(80);//延时80ms左右	
	
  cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
	{
		delay_us(1508);
		if(cnt++>=100)
		{
		 break;
		}
	}
	
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	Byte_1th = AHT20_RD_Byte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
	Send_ACK();
	Byte_2th = AHT20_RD_Byte();//湿度
	Send_ACK();
	Byte_3th = AHT20_RD_Byte();//湿度
	Send_ACK();
	Byte_4th = AHT20_RD_Byte();//湿度/温度
	Send_ACK();
	Byte_5th = AHT20_RD_Byte();//温度
	Send_ACK();
	Byte_6th = AHT20_RD_Byte();//温度
	Send_NOT_ACK();
	I2C_Stop();

	RetuData = 0;
	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//湿度
	
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //温度
}


uint8_t AHT20_Read_CTdata_crc(uint32_t *ct) //CRC校验后，读取AHT20的温度和湿度数据
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	volatile uint8_t  Byte_7th=0;
	
	uint32_t RetuData;
	uint16_t cnt;
	// uint8_t  CRCDATA=0;
	 uint8_t  CTDATA[6]={0};//用于CRC传递数组
	
	AHT20_SendAC();//向AHT10发送AC命令
	delay_ms(80);//延时80ms左右	
	 
  cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
	{
		delay_us(1508);
		if(cnt++>=100)
		{
		 break;
		}
	}
	
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	CTDATA[0]=Byte_1th = AHT20_RD_Byte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
	Send_ACK();
	CTDATA[1]=Byte_2th = AHT20_RD_Byte();//湿度
	Send_ACK();
	CTDATA[2]=Byte_3th = AHT20_RD_Byte();//湿度
	Send_ACK();
	CTDATA[3]=Byte_4th = AHT20_RD_Byte();//湿度/温度
	Send_ACK();
	CTDATA[4]=Byte_5th = AHT20_RD_Byte();//温度
	Send_ACK();
	CTDATA[5]=Byte_6th = AHT20_RD_Byte();//温度
	Send_ACK();
	Byte_7th = AHT20_RD_Byte();//CRC数据
	Send_NOT_ACK();                           //注意: 最后是发送NAK
	I2C_Stop();
	
	if(Calc_CRC8(CTDATA,6)==Byte_7th)
	{
		RetuData = 0;
		RetuData = (RetuData|Byte_2th)<<8;
		RetuData = (RetuData|Byte_3th)<<8;
		RetuData = (RetuData|Byte_4th);
		RetuData =RetuData >>4;
		ct[0] = RetuData;//湿度
		
		RetuData = 0;
		RetuData = (RetuData|Byte_4th)<<8;
		RetuData = (RetuData|Byte_5th)<<8;
		RetuData = (RetuData|Byte_6th);
		RetuData = RetuData&0xfffff;
		ct[1] =RetuData; //温度
		
		return 1;
	}
	else
	{
		ct[0]=0x00;
		ct[1]=0x00;//校验错误返回值，客户可以根据自己需要更改
		
		return 0;
	}//CRC数据
}



void JH_Reset_REG(uint8_t addr)
{
	uint8_t Byte_first,Byte_second,Byte_third,Byte_fourth;

	I2C_Start();
	AHT20_WR_Byte(0x70);//原来是0x70
	Receive_ACK();
	AHT20_WR_Byte(addr);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	I2C_Stop();

	delay_ms(5);//延时5ms左右
	
	I2C_Start();
	AHT20_WR_Byte(0x71);//
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_ACK();
	Byte_second = AHT20_RD_Byte();
	Send_ACK();
	Byte_third = AHT20_RD_Byte();
	Send_NOT_ACK();
	I2C_Stop();
	
  delay_ms(10);//延时10ms左右
	
	I2C_Start();
	AHT20_WR_Byte(0x70);///
	Receive_ACK();
	AHT20_WR_Byte(0xB0|addr);////寄存器命令
	Receive_ACK();
	AHT20_WR_Byte(Byte_second);
	Receive_ACK();
	AHT20_WR_Byte(Byte_third);
	Receive_ACK();
	I2C_Stop();
	
	Byte_second=0x00;
	Byte_third =0x00;
}

void AHT20_Start_Init(void)
{
	JH_Reset_REG(0x1b);
	JH_Reset_REG(0x1c);
	JH_Reset_REG(0x1e);
}

void Sensor_DHT20_Init(void)   //初始化AHT20
{	
	DHT20_I2C_Config();
	
	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xa8);//0xA8进入NOR工作模式
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	I2C_Stop();

	delay_ms(10);//延时10ms左右

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xbe);//0xBE初始化命令，AHT20的初始化命令是0xBE,   AHT10的初始化命令是0xE1
	Receive_ACK();
	AHT20_WR_Byte(0x08);//相关寄存器bit[3]置1，为校准输出
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	I2C_Stop();
	
	delay_ms(10);//延时10ms左右
	
	/***********************************************************************************/
	/**///①刚上电，产品芯片内部就绪需要时间，延时100~500ms,建议500ms
	/***********************************************************************************/
	//delay_ms(500);
	
	/***********************************************************************************/
	/**///②上电第一次发0x71读取状态字，判断状态字是否为0x18,如果不是0x18,进行寄存器初始化
	/***********************************************************************************/
	if((AHT20_Read_Status()&0x18)!=0x18)
	{
		AHT20_Start_Init(); //重新初始化寄存器
		delay_ms(10);
	}
}

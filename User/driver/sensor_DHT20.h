#ifndef _SENSOR_DHT20_H_
#define _SENSOR_DHT20_H_

#include "sys.h"

void Sensor_DHT20_Init(void);   //初始化AHT20

void DHT20_update(void);
int DHT20_get_temperature(void);
int DHT20_get_humidity(void);

//void I2C_Start(void);		 //I2C主机发送START信号
//void AHT20_WR_Byte(uint8_t Byte); //往AHT20写一个字节
//uint8_t AHT20_RD_Byte(void);//从AHT20读取一个字节
//uint8_t Receive_ACK(void);   //看AHT20是否有回复ACK
//void Send_ACK(void)	;	  //主机回复ACK信号
//void Send_NOT_ACK(void);	//主机不回复ACK
//void Stop_I2C(void);	  //一条协议结束

//uint8_t AHT20_Read_Status(void);//读取AHT20的状态寄存器
//uint8_t AHT20_Read_Cal_Enable(void);  //查询cal enable位有没有使能
//void AHT20_SendAC(void); //向AHT20发送AC命令
//uint8_t Calc_CRC8(uint8_t *message,uint8_t Num);

void AHT20_Read_CTdata(uint32_t *ct); //没有CRC校验，直接读取AHT20的温度和湿度数据
uint8_t AHT20_Read_CTdata_crc(uint32_t *ct); //CRC校验后，读取AHT20的温度和湿度数据

//void JH_Reset_REG(uint8_t addr);///重置寄存器
//void AHT20_Start_Init(void);///上电初始化进入正常测量状态

#endif

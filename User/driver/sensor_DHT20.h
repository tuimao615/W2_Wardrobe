#ifndef _SENSOR_DHT20_H_
#define _SENSOR_DHT20_H_

#include "sys.h"

void Sensor_DHT20_Init(void);   //��ʼ��AHT20

void DHT20_update(void);
int DHT20_get_temperature(void);
int DHT20_get_humidity(void);

//void I2C_Start(void);		 //I2C��������START�ź�
//void AHT20_WR_Byte(uint8_t Byte); //��AHT20дһ���ֽ�
//uint8_t AHT20_RD_Byte(void);//��AHT20��ȡһ���ֽ�
//uint8_t Receive_ACK(void);   //��AHT20�Ƿ��лظ�ACK
//void Send_ACK(void)	;	  //�����ظ�ACK�ź�
//void Send_NOT_ACK(void);	//�������ظ�ACK
//void Stop_I2C(void);	  //һ��Э�����

//uint8_t AHT20_Read_Status(void);//��ȡAHT20��״̬�Ĵ���
//uint8_t AHT20_Read_Cal_Enable(void);  //��ѯcal enableλ��û��ʹ��
//void AHT20_SendAC(void); //��AHT20����AC����
//uint8_t Calc_CRC8(uint8_t *message,uint8_t Num);

void AHT20_Read_CTdata(uint32_t *ct); //û��CRCУ�飬ֱ�Ӷ�ȡAHT20���¶Ⱥ�ʪ������
uint8_t AHT20_Read_CTdata_crc(uint32_t *ct); //CRCУ��󣬶�ȡAHT20���¶Ⱥ�ʪ������

//void JH_Reset_REG(uint8_t addr);///���üĴ���
//void AHT20_Start_Init(void);///�ϵ��ʼ��������������״̬

#endif

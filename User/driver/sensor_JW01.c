#include "sensor_JW01.h"
#include "jw01_usart.h"


typedef struct 
{
  unsigned short   volatile Wd_Indx;
  unsigned short   volatile Rd_Indx;
  unsigned short   Mask;
  unsigned char *pbuf;
}UartBuf;


#define RX_BUFFER_SIZE   128
#define TX_BUFFER_SIZE   128

static unsigned char rx_buffer[RX_BUFFER_SIZE];
static unsigned char tx_buffer[TX_BUFFER_SIZE];

//���� ����ṹ��ʵ������������
static UartBuf UartRxbuf;
static UartBuf UartTxbuf;


void Sensor_JW01_Init(void)
{
	JW01_USART_Config();

	UartTxbuf.Wd_Indx = 0;
	UartTxbuf.Rd_Indx = 0;
	UartTxbuf.Mask = TX_BUFFER_SIZE - 1;
	UartTxbuf.pbuf = &tx_buffer[0];

	UartRxbuf.Wd_Indx = 0;
	UartRxbuf.Rd_Indx = 0;
	UartRxbuf.Mask = RX_BUFFER_SIZE - 1;
	UartRxbuf.pbuf = &rx_buffer[0];	
}

//��ȡ���������е�һ���ֽ�
static unsigned char UartBuf_RD(UartBuf *Ringbuf)
{
  unsigned char temp;
  temp = Ringbuf->pbuf[Ringbuf->Rd_Indx & Ringbuf->Mask];//���ݳ����������Ҫ�����Ǿ������ݻ��εĹؼ�
  Ringbuf->Rd_Indx++;//��ȡ���һ�Σ���ָ���1��Ϊ��һ�� ��ȡ�� ׼��
  return temp;
}

//��һ���ֽ�д��һ�����νṹ����
static void UartBuf_WD(UartBuf *Ringbuf,uint8_t DataIn)
{
  Ringbuf->pbuf[Ringbuf->Wd_Indx & Ringbuf->Mask] = DataIn;//���ݳ����������Ҫ�����Ǿ������ݻ��εĹؼ�
  Ringbuf->Wd_Indx++;//д��һ�Σ�дָ���1��Ϊ��һ��д����׼��
}

//�����������Ŀ����ֽڳ��ȣ���дָ��д��һȦ��׷���˶�ָ��
//��ô֤������д���ˣ���ʱӦ�����ӻ��������ȣ�����������Χ���ݴ���ʱ��
static unsigned short  UartBuf_Cnt(UartBuf *Ringbuf)
{
  return (Ringbuf->Wd_Indx - Ringbuf->Rd_Indx) & Ringbuf->Mask;//���ݳ����������Ҫ�����Ǿ������ݻ��εĹؼ�
}

void JW01_USART_Put_Char(unsigned char DataToSend)
{
	UartBuf_WD(&UartTxbuf,DataToSend);//�����������ݷ��ڻ��λ���������
	USART_ITConfig(JW01_USART, USART_IT_TXE, ENABLE);  //���������жϿ�ʼžžž���ͻ����е�����
}

JW01_USART_IRQ_HANDLER
{
  if(USART_GetITStatus(JW01_USART, USART_IT_TXE) == SET)
  {   
		if(UartBuf_Cnt(&UartTxbuf)!= 0)
		{
			USART_SendData(JW01_USART, UartBuf_RD(&UartTxbuf));
		}
		if(UartBuf_Cnt(&UartTxbuf)==0)  
			USART_ITConfig(JW01_USART, USART_IT_TXE, DISABLE);
  } 
	else if(USART_GetITStatus(JW01_USART, USART_IT_RXNE) == SET)
  {
    USART_ClearITPendingBit(JW01_USART, USART_IT_RXNE);	
    
		unsigned char byte = (unsigned char) USART_ReceiveData(JW01_USART);
		UartBuf_WD(&UartRxbuf,byte); 		
  }
}


static unsigned char TVOC_H, TVOC_L, CH2O_H, CH2O_L, CO2_H, CO2_L, checkdata;
static unsigned char TVOC_H_temp, TVOC_L_temp, CH2O_H_temp, CH2O_L_temp, CO2_H_temp, CO2_L_temp, checkdata_temp;

uint16_t JW01_get_TVOC(void)
{
	return TVOC_H*256 + TVOC_L;
}

uint16_t JW01_get_CH2O(void)
{
	return CH2O_H*256 + CH2O_L;
}

uint16_t JW01_get_CO2(void)
{
	return CO2_H*256 + CO2_L;
}

static uint8_t JW01_start_sampling(void)
{
	while(UartBuf_Cnt(&UartRxbuf))
	{
		if(UartBuf_RD(&UartRxbuf) == 0x2C)
			if(UartBuf_Cnt(&UartRxbuf) && UartBuf_RD(&UartRxbuf) == 0xE4)
				return 1;
	}
	return 0;
}

static unsigned char JW01_read_byte(void)
{
	if(UartBuf_Cnt(&UartRxbuf))
		return UartBuf_RD(&UartRxbuf);
	return 0;
}

void JW01_update(void)
{
	unsigned char temp;
	if(JW01_start_sampling())
	{
		TVOC_H_temp = JW01_read_byte();
		TVOC_L_temp = JW01_read_byte();
		CH2O_H_temp = JW01_read_byte();
		CH2O_L_temp = JW01_read_byte();
		CO2_H_temp = JW01_read_byte();
		CO2_L_temp = JW01_read_byte();
		checkdata_temp = JW01_read_byte();
		
		temp=(0x2C+0xE4 +TVOC_H_temp+TVOC_L_temp+CH2O_H_temp+CH2O_L_temp+CO2_H_temp+CO2_L_temp);
		if(temp == checkdata_temp && CO2_L_temp != 0)
		{
			TVOC_H = TVOC_H_temp;
			TVOC_L = TVOC_L_temp;
			CH2O_H = CH2O_H_temp;
			CH2O_L = CH2O_L_temp;
			CO2_H = CO2_H_temp;
			CO2_L = CO2_L_temp;
			checkdata = checkdata_temp;
		}
	}
}
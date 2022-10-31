#include "wifi_usart.h"
#include "mcu_api.h"


typedef struct 
{
  unsigned short   volatile Wd_Indx;
  unsigned short   volatile Rd_Indx;
  unsigned short   Mask;
  unsigned char *pbuf;
}UartBuf;

//���� ����ṹ��ʵ������������
static UartBuf UartRxbuf;
static UartBuf UartTxbuf;

#define RX_BUFFER_SIZE   128
#define TX_BUFFER_SIZE   128

static unsigned char rx_buffer[RX_BUFFER_SIZE];
static unsigned char tx_buffer[TX_BUFFER_SIZE];



void WIFI_Init(void)
{
	WIFI_USART_Config();

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


void WIFI_USART_Put_Char(unsigned char DataToSend)
{
	UartBuf_WD(&UartTxbuf,DataToSend);//�����������ݷ��ڻ��λ���������
	USART_ITConfig(WIFI_USART, USART_IT_TXE, ENABLE);  //���������жϿ�ʼžžž���ͻ����е�����
}

WIFI_USART_IRQ_HANDLER
{
  if(USART_GetITStatus(WIFI_USART, USART_IT_TXE) == SET)
  {   
		if(UartBuf_Cnt(&UartTxbuf)!= 0)
		{
			USART_SendData(WIFI_USART,UartBuf_RD(&UartTxbuf));
		}
		if(UartBuf_Cnt(&UartTxbuf)==0)  
			USART_ITConfig(WIFI_USART, USART_IT_TXE, DISABLE);
  }
  
	else if(USART_GetITStatus(WIFI_USART, USART_IT_RXNE) == SET)
  {
    USART_ClearITPendingBit(WIFI_USART, USART_IT_RXNE);	
     
		unsigned char byte = (unsigned char) USART_ReceiveData(WIFI_USART);
		uart_receive_input(byte);  
		//UartBuf_WD(&UartRxbuf,byte); 		
  }
}

#include "bt_buffer.h"

#include "bt_usart.h"
#include "delay.h"


 /************************** Buffer ********************************************/
 
//u8 uart3_rev_buf[UART_BUFFER_SIZE];

typedef struct 
{
  unsigned short   volatile Wd_Indx;
  unsigned short   volatile Rd_Indx;
  unsigned short   Mask;
  unsigned char *pbuf;
}UartBuf;


//���� ����ṹ��ʵ������������
static UartBuf UartTxbuf;//���η��ͽṹ��
static UartBuf UartRxbuf;//���ν��սṹ��

static unsigned char rx_buffer[RX_BUFFER_SIZE];
static unsigned char tx_buffer[TX_BUFFER_SIZE];


void BT_USART_Init(void)
{
	BT_USART_Config();
	
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


/************************** USART <-> Buffer ********************************************/


static u16 countTx=0;
static u16 countRx=0;

static volatile unsigned char Udatatmp;//���ڽ�����ʱ�����ֽ�
static volatile unsigned char Uart1datatmp;//���ڽ�����ʱ�����ֽ�

BT_USART_IRQ_HANDLER
{
	
  if(USART_GetITStatus(BT_USART, USART_IT_TXE) == SET)
  {   
		if(UartBuf_Cnt(&UartTxbuf)!= 0)
		{
			countTx++;
			USART_SendData(BT_USART,UartBuf_RD(&UartTxbuf)); //�������ݻ��淢��
		}
		if(UartBuf_Cnt(&UartTxbuf)==0)  
			USART_ITConfig(BT_USART, USART_IT_TXE, DISABLE);//���绺����ˣ��͹رմ��ڷ����ж�
  }
  else if(USART_GetITStatus(BT_USART, USART_IT_RXNE) == SET)
  {
    USART_ClearITPendingBit(BT_USART, USART_IT_RXNE);//��������жϱ�־
        
    //���ֻ��λ������鴮�ڽ��շ�ʽ�������ڽ���������ݣ��ܷ��㡣�����ݵ�Ҫ����:
    //���ͷ�����Ҫ�������ݰ�ͷ���Ա������������޵�ַ������
		countRx++;
    Udatatmp = (unsigned char) USART_ReceiveData(BT_USART);          //��ʱ���ݸ�ֵ
        
    UartBuf_WD(&UartRxbuf,Udatatmp);               //д���ڽ��ջ�������      
  }
}

void BT_USART_Put_Char(unsigned char DataToSend)
{
	UartBuf_WD(&UartTxbuf,DataToSend);//�����������ݷ��ڻ��λ���������
	USART_ITConfig(BT_USART, USART_IT_TXE, ENABLE);  //���������жϿ�ʼžžž���ͻ����е�����
}

/*
u8 WIFI_USART_Get_Char(void)
{
	while (!(WIFI_USART->ISR & USART_FLAG_RXNE));
	return(USART_ReceiveData(WIFI_USART));
}
*/

/************************** AT Command ********************************************/

static char Cmdreturn[CMD_RETURN_LENGTH];//ָ��ķ��ؽ������

void BT_Uart_SendCmd(const char *p)
{
  char i;
    
  for(i=0;i<CMD_RETURN_LENGTH;i++)
		Cmdreturn[i] = 0;//�ͷ�ָ����ջ���
  
	delay_ms(100);//д��һ��ָ���ʱ500ms�ٶȽ��ջ���

  for(i=0;i<strlen(p);i++)
		BT_USART_Put_Char(*(p+i));  
	
  delay_ms(100);//д��һ��ָ���ʱ500ms�ٶȽ��ջ���
    
  i=0;
  while(UartBuf_Cnt(&UartRxbuf) != 0)     //�����ڻ��岻Ϊ��ʱ�������ڻ��帳ֵ��ָ��������
		Cmdreturn[i++] = UartBuf_RD(&UartRxbuf);
}

//���AT����ִ�к�Ľ�����Ƿ���Ԥ��һ��
// 1 һ��
// 0 ��һ��
char BT_Check_CmdResult(const char *p)
{
	char i,n;
	n = strlen(p);
  for(i=0;i<strlen(p);i++)
		if(Cmdreturn[i] != *(p+i)) break;
  if(i != strlen(p)) return false;
	
	n = strlen(Cmdreturn);
	for(;i<strlen(Cmdreturn);i++)
		if(Cmdreturn[i] != '\n' && Cmdreturn[i] != '\r') break;
	if(i != strlen(Cmdreturn)) return false;
	
  return true;
}


/************************** Passthrough ********************************************/

int BT_SendData(unsigned char* buf, int buflen)
{
	int i;
	for(i=0;i<buflen;i++)
		BT_USART_Put_Char(*(buf+i));  
	return i;
}

int BT_GetData(unsigned char* buf, int count)
{
  int i=0;
	
	//Network delay
	int delay = 0;
	while(UartBuf_Cnt(&UartRxbuf) == 0 && delay < 10)
	{
		delay_ms(100);
		delay++;
	}
	
	//Receive time
	delay_ms(100);
	
  while(UartBuf_Cnt(&UartRxbuf) != 0 && i < count)
		buf[i++] = UartBuf_RD(&UartRxbuf);
	
	return i;
}

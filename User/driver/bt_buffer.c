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


//环形 数组结构体实例化两个变量
static UartBuf UartTxbuf;//环形发送结构体
static UartBuf UartRxbuf;//环形接收结构体

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


//读取环形数据中的一个字节
static unsigned char UartBuf_RD(UartBuf *Ringbuf)
{
  unsigned char temp;
  temp = Ringbuf->pbuf[Ringbuf->Rd_Indx & Ringbuf->Mask];//数据长度掩码很重要，这是决定数据环形的关键
  Ringbuf->Rd_Indx++;//读取完成一次，读指针加1，为下一次 读取做 准备
  return temp;
}

//将一个字节写入一个环形结构体中
static void UartBuf_WD(UartBuf *Ringbuf,uint8_t DataIn)
{
  Ringbuf->pbuf[Ringbuf->Wd_Indx & Ringbuf->Mask] = DataIn;//数据长度掩码很重要，这是决定数据环形的关键
  Ringbuf->Wd_Indx++;//写完一次，写指针加1，为下一次写入做准备
}

//环形数据区的可用字节长度，当写指针写完一圈，追上了读指针
//那么证明数据写满了，此时应该增加缓冲区长度，或者缩短外围数据处理时间
static unsigned short  UartBuf_Cnt(UartBuf *Ringbuf)
{
  return (Ringbuf->Wd_Indx - Ringbuf->Rd_Indx) & Ringbuf->Mask;//数据长度掩码很重要，这是决定数据环形的关键
}


/************************** USART <-> Buffer ********************************************/


static u16 countTx=0;
static u16 countRx=0;

static volatile unsigned char Udatatmp;//串口接收临时数据字节
static volatile unsigned char Uart1datatmp;//串口接收临时数据字节

BT_USART_IRQ_HANDLER
{
	
  if(USART_GetITStatus(BT_USART, USART_IT_TXE) == SET)
  {   
		if(UartBuf_Cnt(&UartTxbuf)!= 0)
		{
			countTx++;
			USART_SendData(BT_USART,UartBuf_RD(&UartTxbuf)); //环形数据缓存发送
		}
		if(UartBuf_Cnt(&UartTxbuf)==0)  
			USART_ITConfig(BT_USART, USART_IT_TXE, DISABLE);//假如缓冲空了，就关闭串口发送中断
  }
  else if(USART_GetITStatus(BT_USART, USART_IT_RXNE) == SET)
  {
    USART_ClearITPendingBit(BT_USART, USART_IT_RXNE);//清除接收中断标志
        
    //此种环形缓冲数组串口接收方式，适用于解包各种数据，很方便。对数据的要求是:
    //发送方必须要求有数据包头，以便解决串口数据无地址的问题
		countRx++;
    Udatatmp = (unsigned char) USART_ReceiveData(BT_USART);          //临时数据赋值
        
    UartBuf_WD(&UartRxbuf,Udatatmp);               //写串口接收缓冲数组      
  }
}

void BT_USART_Put_Char(unsigned char DataToSend)
{
	UartBuf_WD(&UartTxbuf,DataToSend);//将待发送数据放在环形缓冲数组中
	USART_ITConfig(BT_USART, USART_IT_TXE, ENABLE);  //启动发送中断开始啪啪啪发送缓冲中的数据
}

/*
u8 WIFI_USART_Get_Char(void)
{
	while (!(WIFI_USART->ISR & USART_FLAG_RXNE));
	return(USART_ReceiveData(WIFI_USART));
}
*/

/************************** AT Command ********************************************/

static char Cmdreturn[CMD_RETURN_LENGTH];//指令的返回结果缓存

void BT_Uart_SendCmd(const char *p)
{
  char i;
    
  for(i=0;i<CMD_RETURN_LENGTH;i++)
		Cmdreturn[i] = 0;//释放指令接收缓存
  
	delay_ms(100);//写完一条指令，延时500ms再度接收缓存

  for(i=0;i<strlen(p);i++)
		BT_USART_Put_Char(*(p+i));  
	
  delay_ms(100);//写完一条指令，延时500ms再度接收缓存
    
  i=0;
  while(UartBuf_Cnt(&UartRxbuf) != 0)     //当串口缓冲不为空时，将串口缓冲赋值给指令结果缓冲
		Cmdreturn[i++] = UartBuf_RD(&UartRxbuf);
}

//检查AT命令执行后的结果，是否与预期一致
// 1 一致
// 0 不一致
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

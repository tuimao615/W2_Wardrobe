#ifndef __BT_BUFFER_H
#define __BT_BUFFER_H

#include "stm32f0xx.h"


#define BT_CONFIG_OK  1
#define BT_CONFIG_ERR 0

#define true  1
#define false 0

//#define UART_BUFFER_SIZE 1024

#define RX_BUFFER_SIZE   128
#define TX_BUFFER_SIZE   128

#define CMD_RETURN_LENGTH 128


void BT_USART_Init(void);

void BT_Uart_SendCmd(const char *p);
char BT_Check_CmdResult(const char *p);

int BT_SendData(unsigned char* buf, int buflen);
int BT_GetData(unsigned char* buf, int count);


#endif

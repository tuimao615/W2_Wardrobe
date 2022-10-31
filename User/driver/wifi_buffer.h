#ifndef __WIFI_BUFFER_H
#define __WIFI_BUFFER_H

#include "sys.h"


#define WIFI_CONFIG_OK  1
#define WIFI_CONFIG_ERR 0

#define true  1
#define false 0

//#define UART_BUFFER_SIZE 1024

#define RX_BUFFER_SIZE   128
#define TX_BUFFER_SIZE   128

#define CMD_RETURN_LENGTH 128


void WIFI_Init(void);

void WIFI_Uart_SendCmd(const char *p);
char WIFI_Check_CmdResult(const char *p);

int WIFI_SenData(unsigned char* buf, int buflen);
int WIFI_GetData(unsigned char* buf, int count);


#endif

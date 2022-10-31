#ifndef _WIFI_COMMAND_H
#define _WIFI_COMMAND_H

//#include "stm32f0xx.h"

void WIFI_SW_SmartBleLink(void);
void WIFI_Setup_Network(void);

int WifiAtCmdSendWithRsp(char *pCmd,char *pRsp);
int IsWifiTcpLink(void);
int IsWifiLink(void);

#endif

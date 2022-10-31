#include "wifi_LPC330.h"
#include "wifi_buffer.h"
#include "wifi_usart.h"
#include "config.h"

#include "delay.h"

#ifdef USE_HF_LPC330

#if 0//def DEBUG_UART
#define PrintfLogInfo(fmt, args...) do{ \
if(log_level <= LOG_LEVEL_1) \
{ \
    printf(fmt, ##args); \
} \
}while(0);
#define PrintfLogNotice(fmt, args...) do{ \
if(log_level <= LOG_LEVEL_2) \
{ \
    printf(fmt, ##args); \
} \
}while(0);
#define PrintfLogError(fmt, args...) do{ \
if(log_level <= LOG_LEVEL_3) \
{ \
    printf(fmt, ##args); \
} \
}while(0);

#else

#define PrintfLogInfo(fmt, args...)
#define PrintfLogNotice(fmt, args...)
#define PrintfLogError(fmt, args...) 

#endif


const char AT_EMPTY_ACK[] = {"AT+\n"};
const char AT_ANSWER_OK[] =  {"+ok"};

const char AT_ECHO_ACK[] = {"AT+E\n"};

const char AT_WMODE_ACK[] = "AT+WMODE\n";
const char AT_WMODE_AP_ANSWER[] = {"+ok=AP"};
const char AT_WMODE_STA_ANSWER[] = {"+ok=STA"};

const char AT_SMARTBT_START[] = "AT+SMARTBTSTART\n";

//const char AT_NAME_ASK[] =     {"AT+NAME?"}; //查询蓝牙设备名
//const char AT_NAME_ANSWER[] =  {"OK+NAME:Crazepony"}; 
//const char AT_NAME_SET[] =     {"AT+NAMECrazepony"}; //设置蓝牙设备名

//const char AT_CODE_ASK[] =     {"AT+PIN?"};
//const char AT_CODE_ANSWER[] =  {"OK+PIN:1234"};  
//const char AT_CODE_SET[] =     {"AT+PIN1234"}; //蓝牙配对密码默认为1234

//const char AT_RENEW_ASK[] =    {"AT+RENEW"}; //恢复出厂设置
//const char AT_RENEW_ANSWER[] = {"OK+RENEW"};    

//const char AT_BAUD_ASK[] =     {"AT+BAUD?"};
//const char AT_BAUD_ANSWER[] =  {"OK+BAUD:115200"};
//const char AT_BAUD_SET[] =     {"AT+BAUD4"};



int wifiConfigState =0;//wifi相关配置是否完成



int WIFI_Config_State(void)
{
    return wifiConfigState;
}

void WIFI_Exec_ATCmd(const char *pCmd)
{
    WIFI_Uart_SendCmd(pCmd);
    if(WIFI_Check_CmdResult("+ok") != true)
    {
        PrintfLogInfo("f:%s\r\n",Cmdreturn);    
    }
    else
    {
        PrintfLogInfo("s:%s\r\n",Cmdreturn); 
    }
    return;
}


//进入AT命令模式
char WIFI_Enter_AT_Mode(char num)
{
	int i;
	
	if(num < 2)
		num = 2;
	
	for(i=0;i<num;i++)
	{
		WIFI_Uart_SendCmd("+");
		WIFI_Uart_SendCmd("+");
		WIFI_Uart_SendCmd("+");
		
		delay_ms(500); //两次尝试之间适当延时，一定时间后模块进入正常模式

		if(WIFI_Check_CmdResult("a") != true)
		{
			PrintfLogInfo("WIFI: Enter AT mode (-2) %s\r\n",Cmdreturn);
			continue ;
		}
		
		WIFI_Uart_SendCmd("a");
		if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
		{
			PrintfLogInfo("WIFI: Enter AT mode (-1) %s\r\n",Cmdreturn);
			continue ;
		}
		
		//开机默认开启
		WIFI_Uart_SendCmd(AT_EMPTY_ACK); //正确的空指令
		if(WIFI_Check_CmdResult(AT_ANSWER_OK) == false) //本条回显还有
		{
			WIFI_Uart_SendCmd(AT_ECHO_ACK); //关闭回显
		}
		//WIFI_Uart_SendCmd(AT_EMPTY_ACK); //正确的空指令
		
		PrintfLogInfo("WIFI: Enter AT mode (OK) %s\r\n",Cmdreturn);
		
		return true;
	}	
	
	return false;
}


//进入蓝牙配网模式
void WIFI_SW_SmartBleLink(void)
{
	PrintfLogNotice("WIFI: Start StamrtBleLink mode\r\n");
	
	//必须重启？
	WIFI_HW_Reset();
	
	//适当延时，一定时间后模块进入正常模式
	delay_ms(1000);
	
	//从透传模式切换到AT命令模式
	if(WIFI_Enter_AT_Mode(3) == true)
	{
		wifiConfigState = WIFI_CONFIG_ERR;
		
		//启动蓝牙配网功能
		WIFI_Exec_ATCmd(AT_SMARTBT_START);
	}
}

void WIFI_Setup_Network()
{
	PrintfLogNotice("WIFI: Setup network\r\n");

	char isAtMode = false;
	u8 sendBuffer[64];
	
	//如果在AT命令模式下，确保回显打开
	WIFI_Uart_SendCmd(AT_EMPTY_ACK);
	WIFI_Uart_SendCmd(AT_EMPTY_ACK); //正确的空指令
	if(WIFI_Check_CmdResult(AT_ANSWER_OK) == true)
	{
		isAtMode = true;
	}
//	else if(WIFI_Check_CmdResult(AT_ECHO_ANSWER_OK2) == true)
//	{
//		isAtMode = true;
//		WIFI_Uart_SendCmd(AT_ECHO_ACK);
//		WIFI_Uart_SendCmd("AT+\n");
//		if(WIFI_Check_CmdResult(AT_ANSWER_OK) == true)
//		{
//			;
//		}
//	}

	//如果在透传模式下，切换到AT命令模式
	if(isAtMode == false)
	{
		isAtMode = WIFI_Enter_AT_Mode(3);
	}
	
	if(isAtMode == true)
	{
		wifiConfigState = WIFI_CONFIG_ERR;
		
		//查询 WIFI 操作模式
		WIFI_Uart_SendCmd(AT_WMODE_ACK);
		
		//Timeout
		//WIFI_Exec_ATCmd("AT+TCPTO=300\n");
		//WIFI_Exec_ATCmd("AT+TCPTO\n");
		
		//Connect
		WIFI_Exec_ATCmd("AT+TCPDIS=on\n");
		WIFI_Exec_ATCmd("AT+TCPDIS\n");
		WIFI_Exec_ATCmd("AT+TCPLK\n");


		//Ping
		//WIFI_Exec_ATCmd("AT+PING=192.168.31.146\r\n");//网络”Ping”指令
		//delay_ms(2000);	
		
		//设置/查询 SOCKA 网络协议参数，修改后即生效。
		WIFI_Exec_ATCmd("AT+NETP=TCP,CLIENT,81,192.168.3.29\n");
		//WIFI_Exec_ATCmd("AT+NETP=TCP,CLIENT,81,192.168.31.146\n");
		//sprintf( sendBuffer, "AT+NETP=TCP,CLIENT,%d,%s\r\n",configParam.port,configParam.ip);
		//sprintf( sendBuffer, "AT+NETP=TCP,CLIENT,%d,%s\r\n",configParam.port,configParam.ip);
		//WIFI_Exec_ATCmd(sendBuffer);//设置/查询 SOCKA 网络协议参数，修改后即生效。
		delay_ms(500);
		WIFI_Exec_ATCmd("AT+NETP\n");
		

		//进入透传模式
		WIFI_Exec_ATCmd("AT+ENTM\n");
		
		wifiConfigState = WIFI_CONFIG_OK;		
	}	
}

void WIFI_EnterCmdMode()
{
    while(1)
    {
        WIFI_Uart_SendCmd("+");
        delay_ms(200);

        WIFI_Uart_SendCmd("+");   
        delay_ms(200);
			
        WIFI_Uart_SendCmd("+");
        delay_ms(2000);
			
        if(WIFI_Check_CmdResult("a") != true)
        {
            //HM-11模块已经是115200波特率，无需配置
            PrintfLogInfo("WIFI_Uart_SendCmd -1= %s\r\n",Cmdreturn); //打印
            continue ;
        }

        WIFI_Uart_SendCmd("a");
        if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
        {
            //HM-11模块已经是115200波特率，无需配置
            PrintfLogInfo("WIFI_Uart_SendCmd 0= %s\r\n",Cmdreturn); //打印
            continue ;
        }
        else
        {
            PrintfLogInfo("WIFI_Uart_SendCmd 00= %s\r\n",Cmdreturn); //打印
            //进入命令模式
            //模块从透传模式切换到命令模式时，默认回显功能打开，第一次输入 AT+E 后关闭回显功能，再次输入后打开回显功能，AT+E=on/off 可保存回显功能。
            break;
        }
        
        PrintfLogInfo("WIFI_Uart_SendCmd 0= %s\r\n",Cmdreturn); //打印
    }
		
   // WIFI_Uart_SendCmd("AT+E\n");
    //WIFI_Exec_ATCmd(AT_SMARTBT_START);//启动蓝牙方式 SmartBleLink 配网功能
    //while(1);
		
    WIFI_Uart_SendCmd("AT+WMODE\n");//设置/查询 WIFI 操作模式，复位后设置生效。
    if(WIFI_Check_CmdResult(AT_WMODE_AP_ANSWER) != true)
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 1= %s\r\n",Cmdreturn); //打印          //HM-11模块已经是115200波特率，无需配置
        //continue ;
    }
    else
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 11= %s\r\n",Cmdreturn); //打印          //HM-11模块已经是115200波特率，无需配置
    }
		
#if 0       
    WIFI_Uart_SendCmd("AT+VER\n");//OK
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 2= %s\r\n",Cmdreturn); //打印           //HM-11模块已经是115200波特率，无需配置
       // continue ;
    }
             
    WIFI_Uart_SendCmd("AT+LANN\n");//OK
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //打印          //HM-11模块已经是115200波特率，无需配置
       // continue ;
    }


    WIFI_Uart_SendCmd("AT+WAP\n");
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //打印          //HM-11模块已经是115200波特率，无需配置
       // continue ;
    }
#endif
		
#if 0
    WIFI_Uart_SendCmd(AT_SMARTBT_START);
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //打印           //HM-11模块已经是115200波特率，无需配置
        // continue ;
    }
#endif
		
    WIFI_Uart_SendCmd("AT+WSSSID\n");//设置/查询关联 AP 的 SSID，复位后设置生效。
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 4= %s\r\n",Cmdreturn); //打印         //HM-11模块已经是115200波特率，无需配置
        // continue ;
    }
    else
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 44= %s\r\n",Cmdreturn); //打印         //HM-11模块已经是115200波特率，无需配置
    }
		
    #if 0
    WIFI_Uart_SendCmd("AT+SMARTAPCONFIG\n");
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //打印          //HM-11模块已经是115200波特率，无需配置
        // continue ;
    }
    #endif
		
    #if 0
    WIFI_Uart_SendCmd((char*)&Cmdreturn);
    //WIFI_Uart_SendCmd((char*)&Udatatmp);
    
 
   // Uart1SendaBTCmd("ATAAT");
    //printf("Uart1SendaBTCmd = %s\r\n",CmdUart1return); //打印              
    
    if(CmdUart1Judgement(AT_ANSWER_OK2) != true)
    {
        //HM-11模块已经是115200波特率，无需配置
        continue ;
    }
    #endif
		
    //WIFI_Exec_ATCmd("AT+E\n");//打开/关闭回显功能。

    WIFI_Exec_ATCmd("AT+CMDPW\n");//设置/查询透传模式下发送 AT 命令的前导字符，复位后设置生效。
    //WIFI_Exec_ATCmd("AT+ENTM\n");//进入透传模式。
    WIFI_Exec_ATCmd("AT+TMODE\n");//设置/查询模组的数据传输模式，复位后设置生效。
    WIFI_Exec_ATCmd("AT+MID\n");//查询模块 ID
    WIFI_Exec_ATCmd("AT+VER\n");//查询软件版本号。
    WIFI_Exec_ATCmd("AT+BVER\n");//查询 Bootloader 版本号。
    WIFI_Exec_ATCmd("AT+HWVER\n");//查询芯片硬件版本号。
    //WIFI_Exec_ATCmd("AT+RELD\n");//恢复出厂设置
    //WIFI_Exec_ATCmd("AT+FCLR\n");//擦除出厂配置设置。
    //WIFI_Exec_ATCmd("AT+Z\n");//重启模块。
    WIFI_Exec_ATCmd("AT+UART\n");//设置或查询串口操作，复位后设置生效。
    WIFI_Exec_ATCmd("AT+NETP\n");//设置/查询 SOCKA 网络协议参数，修改后即生效。
    WIFI_Exec_ATCmd("AT+MAXSK\n");//设置/查询模块 SOCKA 工作在 TCP Server 时 TCP Client 接入数目。
    WIFI_Exec_ATCmd("AT+TCPLK\n");//查询 SOCKA TCP 链接是否已建链。
    WIFI_Exec_ATCmd("AT+TCPTO\n");//设置/查询 SOCKA TCP 超时时间，复位后设置生效。

    WIFI_Exec_ATCmd("AT+TCPDIS\n");//建立/断开 SOCKA TCP 链接。
   // WIFI_Exec_ATCmd("AT+SEND\n");//在命令模式下发送数据给 SOCKA
   // WIFI_Exec_ATCmd("AT+RECV\n");//在命令模式下接收 SOCKA 的数据
    WIFI_Exec_ATCmd("AT+UDPLCPT\n");//设置/查询 Socket A， Socket B 用作 UDP 通讯时的本地端口
    WIFI_Exec_ATCmd("AT+WANN\n");//设置/查询 STA 的网络参数 ，复位后设置生效。
    WIFI_Exec_ATCmd("AT+WSMAC\n");//设置/查询模块的 STA MAC 地址参数，复位后设置生效。
    WIFI_Exec_ATCmd("AT+WSLK\n");//查询 STA 的无线 Link 状态。
    //WIFI_Exec_ATCmd("AT+SMTLK\n");//启动 SmartLink 功能
    //WIFI_Exec_ATCmd("AT+NDBGL\n");//打开/关闭调试信息输出。

    WIFI_Exec_ATCmd("AT+SMEM\n");//查询模组 RAM 动态分配情况。
    //WIFI_Exec_ATCmd("AT+WIFI\n");//打开/关闭 Wi-Fi 命令
    //WIFI_Exec_ATCmd("AT+SMARTAPCONFIG\n");//配置 SoftAP 方式 SmartAPLink 配网功能

    //WIFI_Exec_ATCmd(AT_SMARTBT_START);//启动蓝牙方式 SmartBleLink 配网功能
    WIFI_Exec_ATCmd("AT+PING=192.168.3.1\n");//网络”Ping”指令

    WIFI_Exec_ATCmd("AT+NETP=TCP,CLIENT,7788,192.168.3.101\r\n");//设置/查询 SOCKA 网络协议参数，修改后即生效。
    delay_ms(2000);

    WIFI_Exec_ATCmd("AT+NETP\n");//设置/查询 SOCKA 网络协议参数，修改后即生效。
//WIFI_Exec_ATCmd("AT+E\n");//打开/关闭回显功能。
    delay_ms(2000);


   WIFI_Exec_ATCmd("AT+ENTM\n"); //进入透传模式
	 
	 wifiConfigState = WIFI_CONFIG_OK;
}



int WifiAtCmdSendWithRsp(char *pCmd,char *pRsp)
{
    if(WIFI_CONFIG_OK != wifiConfigState)
    {
        PrintfLogError("fail:wifiConfigState = 0x%x\r\n",wifiConfigState);
        return 0; 
    }
    WIFI_Uart_SendCmd(pCmd);
    if(WIFI_Check_CmdResult(pRsp) != true)
    {
        PrintfLogInfo("f:%s\r\n",Cmdreturn);    
        return 0;
    }
    else
    {
        PrintfLogInfo("s:%s\r\n",Cmdreturn); 
        return 1;
    }

}

int IsWifiTcpLink(void)
{
    //查询 SOCKA TCP 链接是否已建链。
    return WifiAtCmdSendWithRsp("AT+TCPLK\n","AT+TCPLK\n+ok=on");
}

int IsWifiLink(void)
{
    //查询 STA 的无线 Link 状态。
    if(1== WifiAtCmdSendWithRsp("AT+WSLK\n","AT+WSLK\n+ok=Disconnected"))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

#endif

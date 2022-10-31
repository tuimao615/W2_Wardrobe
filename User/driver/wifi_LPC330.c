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

//const char AT_NAME_ASK[] =     {"AT+NAME?"}; //��ѯ�����豸��
//const char AT_NAME_ANSWER[] =  {"OK+NAME:Crazepony"}; 
//const char AT_NAME_SET[] =     {"AT+NAMECrazepony"}; //���������豸��

//const char AT_CODE_ASK[] =     {"AT+PIN?"};
//const char AT_CODE_ANSWER[] =  {"OK+PIN:1234"};  
//const char AT_CODE_SET[] =     {"AT+PIN1234"}; //�����������Ĭ��Ϊ1234

//const char AT_RENEW_ASK[] =    {"AT+RENEW"}; //�ָ���������
//const char AT_RENEW_ANSWER[] = {"OK+RENEW"};    

//const char AT_BAUD_ASK[] =     {"AT+BAUD?"};
//const char AT_BAUD_ANSWER[] =  {"OK+BAUD:115200"};
//const char AT_BAUD_SET[] =     {"AT+BAUD4"};



int wifiConfigState =0;//wifi��������Ƿ����



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


//����AT����ģʽ
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
		
		delay_ms(500); //���γ���֮���ʵ���ʱ��һ��ʱ���ģ���������ģʽ

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
		
		//����Ĭ�Ͽ���
		WIFI_Uart_SendCmd(AT_EMPTY_ACK); //��ȷ�Ŀ�ָ��
		if(WIFI_Check_CmdResult(AT_ANSWER_OK) == false) //�������Ի���
		{
			WIFI_Uart_SendCmd(AT_ECHO_ACK); //�رջ���
		}
		//WIFI_Uart_SendCmd(AT_EMPTY_ACK); //��ȷ�Ŀ�ָ��
		
		PrintfLogInfo("WIFI: Enter AT mode (OK) %s\r\n",Cmdreturn);
		
		return true;
	}	
	
	return false;
}


//������������ģʽ
void WIFI_SW_SmartBleLink(void)
{
	PrintfLogNotice("WIFI: Start StamrtBleLink mode\r\n");
	
	//����������
	WIFI_HW_Reset();
	
	//�ʵ���ʱ��һ��ʱ���ģ���������ģʽ
	delay_ms(1000);
	
	//��͸��ģʽ�л���AT����ģʽ
	if(WIFI_Enter_AT_Mode(3) == true)
	{
		wifiConfigState = WIFI_CONFIG_ERR;
		
		//����������������
		WIFI_Exec_ATCmd(AT_SMARTBT_START);
	}
}

void WIFI_Setup_Network()
{
	PrintfLogNotice("WIFI: Setup network\r\n");

	char isAtMode = false;
	u8 sendBuffer[64];
	
	//�����AT����ģʽ�£�ȷ�����Դ�
	WIFI_Uart_SendCmd(AT_EMPTY_ACK);
	WIFI_Uart_SendCmd(AT_EMPTY_ACK); //��ȷ�Ŀ�ָ��
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

	//�����͸��ģʽ�£��л���AT����ģʽ
	if(isAtMode == false)
	{
		isAtMode = WIFI_Enter_AT_Mode(3);
	}
	
	if(isAtMode == true)
	{
		wifiConfigState = WIFI_CONFIG_ERR;
		
		//��ѯ WIFI ����ģʽ
		WIFI_Uart_SendCmd(AT_WMODE_ACK);
		
		//Timeout
		//WIFI_Exec_ATCmd("AT+TCPTO=300\n");
		//WIFI_Exec_ATCmd("AT+TCPTO\n");
		
		//Connect
		WIFI_Exec_ATCmd("AT+TCPDIS=on\n");
		WIFI_Exec_ATCmd("AT+TCPDIS\n");
		WIFI_Exec_ATCmd("AT+TCPLK\n");


		//Ping
		//WIFI_Exec_ATCmd("AT+PING=192.168.31.146\r\n");//���硱Ping��ָ��
		//delay_ms(2000);	
		
		//����/��ѯ SOCKA ����Э��������޸ĺ���Ч��
		WIFI_Exec_ATCmd("AT+NETP=TCP,CLIENT,81,192.168.3.29\n");
		//WIFI_Exec_ATCmd("AT+NETP=TCP,CLIENT,81,192.168.31.146\n");
		//sprintf( sendBuffer, "AT+NETP=TCP,CLIENT,%d,%s\r\n",configParam.port,configParam.ip);
		//sprintf( sendBuffer, "AT+NETP=TCP,CLIENT,%d,%s\r\n",configParam.port,configParam.ip);
		//WIFI_Exec_ATCmd(sendBuffer);//����/��ѯ SOCKA ����Э��������޸ĺ���Ч��
		delay_ms(500);
		WIFI_Exec_ATCmd("AT+NETP\n");
		

		//����͸��ģʽ
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
            //HM-11ģ���Ѿ���115200�����ʣ���������
            PrintfLogInfo("WIFI_Uart_SendCmd -1= %s\r\n",Cmdreturn); //��ӡ
            continue ;
        }

        WIFI_Uart_SendCmd("a");
        if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
        {
            //HM-11ģ���Ѿ���115200�����ʣ���������
            PrintfLogInfo("WIFI_Uart_SendCmd 0= %s\r\n",Cmdreturn); //��ӡ
            continue ;
        }
        else
        {
            PrintfLogInfo("WIFI_Uart_SendCmd 00= %s\r\n",Cmdreturn); //��ӡ
            //��������ģʽ
            //ģ���͸��ģʽ�л�������ģʽʱ��Ĭ�ϻ��Թ��ܴ򿪣���һ������ AT+E ��رջ��Թ��ܣ��ٴ������򿪻��Թ��ܣ�AT+E=on/off �ɱ�����Թ��ܡ�
            break;
        }
        
        PrintfLogInfo("WIFI_Uart_SendCmd 0= %s\r\n",Cmdreturn); //��ӡ
    }
		
   // WIFI_Uart_SendCmd("AT+E\n");
    //WIFI_Exec_ATCmd(AT_SMARTBT_START);//����������ʽ SmartBleLink ��������
    //while(1);
		
    WIFI_Uart_SendCmd("AT+WMODE\n");//����/��ѯ WIFI ����ģʽ����λ��������Ч��
    if(WIFI_Check_CmdResult(AT_WMODE_AP_ANSWER) != true)
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 1= %s\r\n",Cmdreturn); //��ӡ          //HM-11ģ���Ѿ���115200�����ʣ���������
        //continue ;
    }
    else
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 11= %s\r\n",Cmdreturn); //��ӡ          //HM-11ģ���Ѿ���115200�����ʣ���������
    }
		
#if 0       
    WIFI_Uart_SendCmd("AT+VER\n");//OK
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 2= %s\r\n",Cmdreturn); //��ӡ           //HM-11ģ���Ѿ���115200�����ʣ���������
       // continue ;
    }
             
    WIFI_Uart_SendCmd("AT+LANN\n");//OK
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //��ӡ          //HM-11ģ���Ѿ���115200�����ʣ���������
       // continue ;
    }


    WIFI_Uart_SendCmd("AT+WAP\n");
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //��ӡ          //HM-11ģ���Ѿ���115200�����ʣ���������
       // continue ;
    }
#endif
		
#if 0
    WIFI_Uart_SendCmd(AT_SMARTBT_START);
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //��ӡ           //HM-11ģ���Ѿ���115200�����ʣ���������
        // continue ;
    }
#endif
		
    WIFI_Uart_SendCmd("AT+WSSSID\n");//����/��ѯ���� AP �� SSID����λ��������Ч��
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 4= %s\r\n",Cmdreturn); //��ӡ         //HM-11ģ���Ѿ���115200�����ʣ���������
        // continue ;
    }
    else
    {
        PrintfLogInfo("WIFI_Uart_SendCmd 44= %s\r\n",Cmdreturn); //��ӡ         //HM-11ģ���Ѿ���115200�����ʣ���������
    }
		
    #if 0
    WIFI_Uart_SendCmd("AT+SMARTAPCONFIG\n");
    if(WIFI_Check_CmdResult(AT_ANSWER_OK) != true)
    {
        printf("WIFI_Uart_SendCmd 3= %s\r\n",Cmdreturn); //��ӡ          //HM-11ģ���Ѿ���115200�����ʣ���������
        // continue ;
    }
    #endif
		
    #if 0
    WIFI_Uart_SendCmd((char*)&Cmdreturn);
    //WIFI_Uart_SendCmd((char*)&Udatatmp);
    
 
   // Uart1SendaBTCmd("ATAAT");
    //printf("Uart1SendaBTCmd = %s\r\n",CmdUart1return); //��ӡ              
    
    if(CmdUart1Judgement(AT_ANSWER_OK2) != true)
    {
        //HM-11ģ���Ѿ���115200�����ʣ���������
        continue ;
    }
    #endif
		
    //WIFI_Exec_ATCmd("AT+E\n");//��/�رջ��Թ��ܡ�

    WIFI_Exec_ATCmd("AT+CMDPW\n");//����/��ѯ͸��ģʽ�·��� AT �����ǰ���ַ�����λ��������Ч��
    //WIFI_Exec_ATCmd("AT+ENTM\n");//����͸��ģʽ��
    WIFI_Exec_ATCmd("AT+TMODE\n");//����/��ѯģ������ݴ���ģʽ����λ��������Ч��
    WIFI_Exec_ATCmd("AT+MID\n");//��ѯģ�� ID
    WIFI_Exec_ATCmd("AT+VER\n");//��ѯ����汾�š�
    WIFI_Exec_ATCmd("AT+BVER\n");//��ѯ Bootloader �汾�š�
    WIFI_Exec_ATCmd("AT+HWVER\n");//��ѯоƬӲ���汾�š�
    //WIFI_Exec_ATCmd("AT+RELD\n");//�ָ���������
    //WIFI_Exec_ATCmd("AT+FCLR\n");//���������������á�
    //WIFI_Exec_ATCmd("AT+Z\n");//����ģ�顣
    WIFI_Exec_ATCmd("AT+UART\n");//���û��ѯ���ڲ�������λ��������Ч��
    WIFI_Exec_ATCmd("AT+NETP\n");//����/��ѯ SOCKA ����Э��������޸ĺ���Ч��
    WIFI_Exec_ATCmd("AT+MAXSK\n");//����/��ѯģ�� SOCKA ������ TCP Server ʱ TCP Client ������Ŀ��
    WIFI_Exec_ATCmd("AT+TCPLK\n");//��ѯ SOCKA TCP �����Ƿ��ѽ�����
    WIFI_Exec_ATCmd("AT+TCPTO\n");//����/��ѯ SOCKA TCP ��ʱʱ�䣬��λ��������Ч��

    WIFI_Exec_ATCmd("AT+TCPDIS\n");//����/�Ͽ� SOCKA TCP ���ӡ�
   // WIFI_Exec_ATCmd("AT+SEND\n");//������ģʽ�·������ݸ� SOCKA
   // WIFI_Exec_ATCmd("AT+RECV\n");//������ģʽ�½��� SOCKA ������
    WIFI_Exec_ATCmd("AT+UDPLCPT\n");//����/��ѯ Socket A�� Socket B ���� UDP ͨѶʱ�ı��ض˿�
    WIFI_Exec_ATCmd("AT+WANN\n");//����/��ѯ STA ��������� ����λ��������Ч��
    WIFI_Exec_ATCmd("AT+WSMAC\n");//����/��ѯģ��� STA MAC ��ַ��������λ��������Ч��
    WIFI_Exec_ATCmd("AT+WSLK\n");//��ѯ STA ������ Link ״̬��
    //WIFI_Exec_ATCmd("AT+SMTLK\n");//���� SmartLink ����
    //WIFI_Exec_ATCmd("AT+NDBGL\n");//��/�رյ�����Ϣ�����

    WIFI_Exec_ATCmd("AT+SMEM\n");//��ѯģ�� RAM ��̬���������
    //WIFI_Exec_ATCmd("AT+WIFI\n");//��/�ر� Wi-Fi ����
    //WIFI_Exec_ATCmd("AT+SMARTAPCONFIG\n");//���� SoftAP ��ʽ SmartAPLink ��������

    //WIFI_Exec_ATCmd(AT_SMARTBT_START);//����������ʽ SmartBleLink ��������
    WIFI_Exec_ATCmd("AT+PING=192.168.3.1\n");//���硱Ping��ָ��

    WIFI_Exec_ATCmd("AT+NETP=TCP,CLIENT,7788,192.168.3.101\r\n");//����/��ѯ SOCKA ����Э��������޸ĺ���Ч��
    delay_ms(2000);

    WIFI_Exec_ATCmd("AT+NETP\n");//����/��ѯ SOCKA ����Э��������޸ĺ���Ч��
//WIFI_Exec_ATCmd("AT+E\n");//��/�رջ��Թ��ܡ�
    delay_ms(2000);


   WIFI_Exec_ATCmd("AT+ENTM\n"); //����͸��ģʽ
	 
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
    //��ѯ SOCKA TCP �����Ƿ��ѽ�����
    return WifiAtCmdSendWithRsp("AT+TCPLK\n","AT+TCPLK\n+ok=on");
}

int IsWifiLink(void)
{
    //��ѯ STA ������ Link ״̬��
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

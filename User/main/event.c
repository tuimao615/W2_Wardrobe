#include "event.h"

/*volatile*/ u16 current_msg = 0;

u16 get_current_events(void)
{
	return current_msg;
}


void add_event(MmiEvent event)
{ 
	uint16_t msg = 0;
	
	switch(event)
	{
		//WIFI (High priority)
		case EVENT_WIFI_RELOAD_FACTORYSET: 	msg = MSG_WIFI_RELOAD_FACTORYSET; break;
		case EVENT_WIFI_SMART_BLE_LINK: 		msg = MSG_WIFI_SMART_BLE_LINK; break;
		
		//Alarm (Medium priority)
		case EVENT_ALARM_HOT: 			msg = MSG_ALARM_HOT; break;
		case EVENT_ALARM_POLLUTION: msg = MSG_ALARM_POLLUTION; break;
		
		//Funtion (Low priority)
		case EVENT_MODE_JINGHUA: 	msg = MSG_MODE_JINGHUA; break;	
		case EVENT_MODE_XIANGXUN: msg = MSG_MODE_XIANGXUN; break;	
		case EVENT_MODE_SHAJUN:  	msg = MSG_MODE_SHAJUN; break;	
		case EVENT_MODE_HONGGAN:  msg = MSG_MODE_HONGGAN;	break;
		case EVENT_MODE_ZIDONG:  	msg = MSG_MODE_ZIDONG; break;

		default:
			break;
	}
	
	current_msg |= msg;
}


void remove_event(MmiEvent event)
{
	
}

	
	//��Ӧ����Key�д���
	//WIFI_HW_Reload_FactorySet(); 

	//��Ӧ����Key�д���
	//WIFI_HW_SmartBleLink(); //Ӳ����
	//WIFI_SW_SmartBleLink(); //������
	//WIFI_Setup_Network(); //��������

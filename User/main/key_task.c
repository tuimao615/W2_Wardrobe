#include "key.h"		
#include "mode.h"

//Tuya
#include "wifi.h"
#include "device_gpio.h"

u8 Key_Task(void)
{
	static int reset_count = 0;
	MmiEvent event = EVENT_NONE;	
	MmiKey mmikey = Key_Scan();
	
	//Reset
	if(mmikey == KEY_NONE)							//长按重置wifi
	{
		BUZ_OFF();
		if(reset_count > 80)						//长按八秒以上
		{
			event = EVENT_WIFI_RELOAD_FACTORYSET;
		}
		else if(reset_count > 30)					//长按三秒以上
		{
			event = EVENT_WIFI_SMART_BLE_LINK;
		}
		reset_count = 0;
		return 0;
	}
	
#ifdef USE_TUYA_MODULE
	//void all_data_update(void);
	//if((mmikey >= KEY_MODE_JINGHUA && mmikey <= KEY_MODE_ZIDONG)
		//all_data_update();
#endif
	
	switch(mmikey)
	{
		case KEY_MODE_JINGHUA: 	enter_jinhua_mode();  break;
		case KEY_MODE_XIANGXUN:	enter_xiangxun_mode(); break;
		case KEY_MODE_SHAJUN:	enter_shajun_mode(); break;
		case KEY_MODE_HONGGAN:	enter_honggan_mode(); break;
		case KEY_MODE_ZIDONG: 	enter_zidong_mode(); break;
		
		case KEY_ALT_CONFIG: 		mcu_set_wifi_mode(0); break;

		case KEY_RESET: 
			reset_count++;	BUZ_ON(); mcu_set_wifi_mode(0); break;
		
		default:
			break;
	}
	
	return (mmikey >= KEY_MODE_JINGHUA && mmikey < KEY_MAX);
}

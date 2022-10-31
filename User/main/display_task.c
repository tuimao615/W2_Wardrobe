#include "display.h"
#include "seg_HT1621.h"

#include "sensor.h"

#include "config.h"
#include "wifi.h"


void backlight_breathing(void);

void Display_Task(u8 update_mode)
{
	static u16 icon_count = 0;
	static u16 time_count = 0;
	static u16 mode_count = 0;

	//backlight_breathing();
	Lcd_set_brightness(100);

	
//	switch(i)
//	{
//		case 1: Lcd_show_all();break;
//		
//		case 3: Lcd_clear_all(); break;
//		
//		case 4: count = 0; break;

//	}
//	return;
		
	//Icon zone
#ifdef USE_TUYA_MODULE
	unsigned char wifi_state = mcu_get_wifi_work_state();
	
	if(SMART_CONFIG_STATE != wifi_state && SMART_AND_AP_STATE != wifi_state)
		show_BT(0);
	
	if(WIFI_CONNECTED == wifi_state || WIFI_CONN_CLOUD == wifi_state)
		show_Wifi(1);
	else if(WIFI_NOT_CONNECTED != wifi_state)
		show_Wifi(0);
	
	switch(icon_count++/4)
	{	
		case 0:
			if(SMART_CONFIG_STATE == wifi_state || SMART_AND_AP_STATE == wifi_state)
				show_BT(0);
			if(WIFI_NOT_CONNECTED == wifi_state)
				show_Wifi(0);
			break;
		case 1:
			if(SMART_CONFIG_STATE == wifi_state || SMART_AND_AP_STATE == wifi_state)
				show_BT(1);
			if(WIFI_NOT_CONNECTED == wifi_state)
				show_Wifi(1);
			break;
		default:
			icon_count = 0;
			break;
	}
	
#endif
	
	show_clock(0);
	show_alarm(0);

	
	//Number zone
	switch(time_count++/10)
	{	
		case 0: 
		case 1:
			show_time(16, 30);
			break;
		case 2: 
			int tempature = get_current_temperature();
			show_temperature(tempature);
			break;
		case 3:
			int humidity = get_current_humidity();
			show_humidity(humidity);
			break;
		case 4:
			u16 TVOC = get_TVOC();
			if(TVOC != 0)
			{
				show_3_number(TVOC);
				break;
			}
		case 5:
			u16 CH2O = get_CH2O();
			if(CH2O != 0)
			{
				show_3_number(CH2O);
				break;
			}
		case 6:
			u16 CO2 = get_CO2();
			if(CO2 != 0)
			{
				show_4_number(CO2);
				break;
			}			
		default: time_count = 0; break;
	}
	
	
	//Mode zone
	if(update_mode)
		mode_count = 0;
	if(mode_count++%4 == 0)
	{
		switch(mode_count/4)
		{	
			case 0: show_current_mode(0); break;
			case 1: show_current_mode(1); break;
			default: mode_count = 0; break;
		}
	}
}

void backlight_breathing(void)
{
	static u8 bright_percent = 100;
	
	static u8 bright_up = 0;
	if(bright_up)
	{
		if(bright_percent < 100)
			bright_percent++;
		else
			bright_up = 0;
	}
	else
	{
		if(bright_percent > 1)
			bright_percent--;
		else
			bright_up = 1;
	}

	Lcd_set_brightness(bright_percent);
}

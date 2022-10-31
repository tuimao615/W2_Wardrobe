#include "display.h"

#include "seg_HT1621.h"
#include "mode.h"


void Display_Init(void)
{
	Lcd_Init();
	Lcd_set_brightness(50);	
}

void show_time(u8 hour, u8 min)
{	
	Lcd_show_num1(hour/10);
	Lcd_show_num2(hour%10);
	Lcd_Dot_turn(1);
	Lcd_show_num3(min/10);
	Lcd_show_num4(min%10);
	
	Lcd_Temperature_turn(0);
	Lcd_Humidity_turn(0);
	
	Lcd_update();
}

void hide_time(void)
{
	Lcd_show_num1(10);
	Lcd_show_num2(10);
	Lcd_Dot_turn(0);
	Lcd_show_num3(10);
	Lcd_show_num4(10);
	
	Lcd_Temperature_turn(0);
	Lcd_Humidity_turn(0);
	
	Lcd_update();
}

void show_temperature(int num)
{
	Lcd_show_num1(10);
	Lcd_show_num2(10);
	Lcd_Dot_turn(0);
	Lcd_show_num3(num/10%10);
	Lcd_show_num4(num%10);

	Lcd_Temperature_turn(1);
	Lcd_Humidity_turn(0);
	
	Lcd_update();
}

void show_humidity(int num)
{
	Lcd_show_num1(10);
	Lcd_show_num2(10);
	Lcd_Dot_turn(0);
	Lcd_show_num3(num/10%10);
	Lcd_show_num4(num%10);
	
	Lcd_Temperature_turn(0);
	Lcd_Humidity_turn(1);
	
	Lcd_update();
}

void show_3_number(u16 num)
{
	Lcd_show_num1(10);
	Lcd_show_num2(num/100%10);
	Lcd_Dot_turn(0);
	Lcd_show_num3(num/10%10);
	Lcd_show_num4(num%10);
	
	Lcd_Temperature_turn(0);
	Lcd_Humidity_turn(0);
	
	Lcd_update();
}

void show_4_number(u16 num)
{
	Lcd_show_num1(num/1000%10);
	Lcd_show_num2(num/100%10);
	Lcd_Dot_turn(0);
	Lcd_show_num3(num/10%10);
	Lcd_show_num4(num/1%10);
	
	Lcd_Temperature_turn(0);
	Lcd_Humidity_turn(0);
	
	Lcd_update();
}


void show_BT(u8 on)
{
	Lcd_BT_turn(on);
	Lcd_update();
}

void show_Wifi(u8 on)
{
	Lcd_Wifi_turn(on);
	Lcd_update();
}

void show_clock(u8 on)
{
	Lcd_Clock_turn(on);
	Lcd_update();
}

void show_alarm(u8 on)
{
	Lcd_Alarm_turn(on);
	Lcd_update();
}

void show_mode(WorkMode mode)
{
	Lcd_Zidong_turn(WORK_MODE_ZIDONG == mode);
	Lcd_Jinghua_turn(WORK_MODE_JINGHUA == mode);
	Lcd_Xiangxun_turn(WORK_MODE_XIANGXUN == mode);
	Lcd_Shajun_turn(WORK_MODE_SHAJUN == mode);
	Lcd_Honggan_turn(WORK_MODE_HONGGAN == mode);
	Lcd_update();
}

void show_current_mode(u8 enable)
{
	WorkMode mode = get_work_mode();
	if(enable)
	{
		Lcd_Zidong_turn(enable);
		Lcd_Jinghua_turn(enable);
		Lcd_Xiangxun_turn(enable);
		Lcd_Shajun_turn(enable);
		Lcd_Honggan_turn(enable);
	}
	else
	{
		Lcd_Zidong_turn(WORK_MODE_ZIDONG != mode);
		Lcd_Jinghua_turn(WORK_MODE_JINGHUA != mode);
		Lcd_Xiangxun_turn(WORK_MODE_XIANGXUN != mode);
		Lcd_Shajun_turn(WORK_MODE_SHAJUN != mode);
		Lcd_Honggan_turn(WORK_MODE_HONGGAN != mode);
	}
	Lcd_update();
}

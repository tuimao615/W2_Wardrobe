#ifndef __SEG_HT1621_H
#define __SEG_HT1621_H

#include "sys.h"
#include "type_def.h"


void Lcd_Init(void);

void Lcd_Backlight_On(void);
void Lcd_Backlight_Off(void);
void Lcd_set_brightness(u8 percent);
	
void Lcd_show_all(void);
void Lcd_clear_all(void);
void Lcd_update(void);

void Lcd_show_num1(u8 num);
void Lcd_show_num2(u8 num);
void Lcd_show_num3(u8 num);
void Lcd_show_num4(u8 num);

void Lcd_Dot_turn(u8 on);
void Lcd_Temperature_turn(u8 on); 
void Lcd_Humidity_turn(u8 on);

void Lcd_BT_turn(u8 on);
void Lcd_Wifi_turn(u8 on);
void Lcd_Clock_turn(u8 on);
void Lcd_Alarm_turn(u8 on);

void Lcd_Jinghua_turn(u8 on);
void Lcd_Xiangxun_turn(u8 on);
void Lcd_Shajun_turn(u8 on);
void Lcd_Honggan_turn(u8 on);
void Lcd_Zidong_turn(u8 on);
	
#endif

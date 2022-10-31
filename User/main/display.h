#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "type_def.h"

void show_time(u8 hour, u8 minite);
void hide_time(void);

void show_temperature(int num);
void show_humidity(int num);
void show_3_number(u16 num);
void show_4_number(u16 num);

void show_BT(u8 on);
void show_Wifi(u8 on);
void show_clock(u8 on);
void show_alarm(u8 on);

void show_mode(WorkMode mode);
void show_current_mode(u8 enable);

#endif

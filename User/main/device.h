#ifndef __DEVICE_H
#define __DEVICE_H

#include "sys.h"

void Device_Init(void);

u8 LIGHT_is_On(void);
u8 HEAT_is_On(void);
u8 FAN_is_On(void);
u8 FLZ_is_On(void);
u8 CY_is_On(void);
u8 XX_is_On(void);
u8 UV_is_On(void);

void LIGHT_turn(u8 on);
void HEAT_turn(u8 on);
void FAN_turn(u8 on);
void FLZ_turn(u8 on);
void CY_turn(u8 on);
void XX_turn(u8 on);
void UV_turn(u8 on);

void ALL_turn_off(void);

#endif

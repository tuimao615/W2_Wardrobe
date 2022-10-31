#ifndef _BUZZER_H
#define _BUZZER_H

#include "sys.h"
 
typedef unsigned char u8;
typedef const unsigned char uc8;
 
#define d_Sound_Di							0
#define d_Sound_Di_Di						1
#define d_Sound_Di_Di_Di				2
#define d_Sound_Di_Di_Di_Di_Di	3
#define d_Sound_Di_2s						4
#define d_Sound_Di_5s						5	
 
extern	void Buzzer_Start(u8 I);
extern	void Buzzer_Output(void);
 
#endif

#ifndef __DATETIME_H
#define __DATETIME_H

#include "type_def.h"

void set_sys_time(u16 date, u16 time);
u8 get_sys_hour(void);
u8 get_sys_minite(void);

void update_sys_time(void);

#endif

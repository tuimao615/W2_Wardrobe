#include "datetime.h"

static u16 sys_date;
static u16 sys_time;

void set_system_time(u16 date, u16 time)
{
	sys_date = date;
	sys_time = time;
}

u8 get_sys_hour(void)
{
	return 88;
}

u8 get_sys_minite(void)
{
	return 88;
}

void update_system_time(void)
{
	sys_time++;
}

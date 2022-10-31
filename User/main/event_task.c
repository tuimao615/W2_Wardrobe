#include "event.h"
#include "mode.h"


void Event_Task(void)
{
	uint16_t events = get_current_events();
	
	if(events & MSG_MODE_MASK)
	{
		;
	}

}

void update_work_mode(uint16_t msg)
{
	if(msg & MSG_MODE_JINGHUA)
	{
		enter_jinhua_mode();
	}
	else if(msg & MSG_MODE_XIANGXUN)
	{
		enter_xiangxun_mode();
	}
	else if(msg & MSG_MODE_SHAJUN)
	{
		enter_shajun_mode();
	}
	else if(msg & MSG_MODE_HONGGAN)
	{
		enter_honggan_mode();
	}
	else if(msg & MSG_MODE_ZIDONG)
	{
		enter_zidong_mode();
	}
}


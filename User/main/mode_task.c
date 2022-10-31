#include "mode.h"
#include "device.h"


extern void run_jinghua_mode();
extern void run_xiangxun_mode();
extern void run_shajun_mode();
extern void run_honggan_mode();
extern void run_zidong_mode();

void Mode_Task(void)
{
	WorkMode mode = get_work_mode();
	
	switch(mode)
	{
		case WORK_MODE_JINGHUA:
			run_jinghua_mode();			//判断是否结束净化状态还是继续
			break;
		
		case WORK_MODE_XIANGXUN:
			run_xiangxun_mode();		//判断是否结束香薰状态还是继续
			break;
		
		case WORK_MODE_SHAJUN:
			run_shajun_mode();			//判断是否结束杀菌状态还是继续
			break;
		
		case WORK_MODE_HONGGAN:
			run_honggan_mode();			//判断是否结束烘干状态还是继续
			break;
		
		case WORK_MODE_ZIDONG:
			run_zidong_mode();			//判断是否结束自动状态还是继续
			break;
		
		case WORK_MODE_CHUNJING:
			run_chunjing_mode();			//判断是否结束自动状态还是继续
			break;
		
		
		default:
			break;
	}
	
}

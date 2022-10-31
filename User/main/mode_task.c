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
			run_jinghua_mode();			//�ж��Ƿ��������״̬���Ǽ���
			break;
		
		case WORK_MODE_XIANGXUN:
			run_xiangxun_mode();		//�ж��Ƿ������޹״̬���Ǽ���
			break;
		
		case WORK_MODE_SHAJUN:
			run_shajun_mode();			//�ж��Ƿ����ɱ��״̬���Ǽ���
			break;
		
		case WORK_MODE_HONGGAN:
			run_honggan_mode();			//�ж��Ƿ�������״̬���Ǽ���
			break;
		
		case WORK_MODE_ZIDONG:
			run_zidong_mode();			//�ж��Ƿ�����Զ�״̬���Ǽ���
			break;
		
		case WORK_MODE_CHUNJING:
			run_chunjing_mode();			//�ж��Ƿ�����Զ�״̬���Ǽ���
			break;
		
		
		default:
			break;
	}
	
}

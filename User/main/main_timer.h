#ifndef __MAIN_TASK_H
#define	__MAIN_TASK_H

#include "sys.h"

#define ONE_SECOND_PEROID		1000				//���´��������ݼ��1��
#define KEY_TASK_PEROID			100					//ɨ����̼��0.1��
#define MODE_TASK_PERROID		1000*60				//����ģʽ��ѯ���60��
#define DISPLAY_TASK_PEROID		100					
#define STATUS_CHECK_PEROID 	1000				//�Զ�����״̬ȷ�ϼ��1��
#define HEAT_TIME_PEROID 		1000				//����Ƭ״̬��ѯ���1��
#define PIR_TIME_PEROID 		1000					//����״̬�����1��
#define HEAT_TIME				135					//����Ƭ��Ƭ����ʱ�� ��λ/��

void timer_delay(uint16_t ms);

#endif

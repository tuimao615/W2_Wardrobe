#ifndef __MAIN_TASK_H
#define	__MAIN_TASK_H

#include "sys.h"

#define ONE_SECOND_PEROID		1000				//更新传感器数据间隔1秒
#define KEY_TASK_PEROID			100					//扫描键盘间隔0.1秒
#define MODE_TASK_PERROID		1000*60				//任务模式查询间隔60秒
#define DISPLAY_TASK_PEROID		100					
#define STATUS_CHECK_PEROID 	1000				//自动控制状态确认间隔1秒
#define HEAT_TIME_PEROID 		1000				//加热片状态查询间隔1秒
#define PIR_TIME_PEROID 		1000					//红外状态检测间隔1秒
#define HEAT_TIME				135					//加热片单片加热时长 单位/秒

void timer_delay(uint16_t ms);

#endif

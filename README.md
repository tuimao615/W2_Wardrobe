# W2_Wardrobe
A smart wardrobe based on STM32F103 and Tuya smart cloud service
7.8
智能层板V1.0	基本功能、红外感应、自动运行、模式切换
自动运行功能：
Main/main_timer.c	TIMER_MAIN_IRQ_HANDLER总定时器中断函数里通过不同的计数来执行不同判断语句实现自动运行：
	①line_90每隔60s执行Mode_Task()，检测判断当前模式后执行对应的设备操作
	②line_102 每隔1s判断各参数是否超标，超标则改变到相应模式
	③line_137 如果在烘干模式下，则每隔1s执行分时加热，单边加热时长由宏定义HEAT_TIME决定
	④line_187 红外感应检测
Main/main_timer.h	此文件里定义了不同的时间计数宏定义参数
Main/mode.c	此文件里定义了各个模式的功能函数，line_11定义了设备的初始模式
Main/device.c	此文件里定义了各个器件的开关函数

Tuya/protocol.c	如果涂鸦云端添加了新的dpid功能函数，需要对照下载云端sdk文件对照着修改protocol.c文件，需要修改的地方有以下几处：
	①line_82 结构体添加成员
	②line_154 all_data_update()函数为所有数据的上传函数，如有需要需对照新的sdk里的all_data_update()函数添加新的上传函数
	③line_196 此后为各个下发接收函数，需对照新的sdk里的添加修改
	④line_493 对照新的sdk添加case
Tuya/protocol.h 	line_34处可修改对应的项目id，不同id对应着不同的涂鸦云端的项目，注释有所标注
7.11
智能层板V1.1	增加蜂鸣器报警功能
蜂鸣器报警功能:
Main/main_timer.h 	line_99 在自动控制判断里加入蜂鸣器检测函数，超标时执行函数并改变sign的值，当sign值改变时蜂鸣器开始报警，但蜂鸣器声音过小
7.21
智能层板V1.2	增加灯管控制,屏蔽蜂鸣器报警功能（蜂鸣器未完善）
Config/device_gpio.c	line_10初始化灯带串口
Config/device_gpio.h	line_6增加灯带引脚定义
Main/device.c			line_81增加灯带开关函数LIGHT_turn()后续开发调用此函数开关灯带
7.25
智能层板V1.3	修改V1.1的蜂鸣器报警效果，无V1.2的灯管控制
Config/timer_main.c	line_38 增加了一个蜂鸣器的定时器Timer_BUZ_Config()
Main/main_timer.c	line_265	蜂鸣器的定时器中断函数，负责模拟PWM波
line_46	定义了两个函数分别为蜂鸣器定时器中断的开关函数，函数里的参数为单响时间AlarmON和间隔时间AlarmOFF
					line_123	自动控制里加入开关函数负责在切换时开关蜂鸣器
					line_154	此处负责实现蜂鸣器每次报警后间隔响三声后不再报警

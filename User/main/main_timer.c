#include "main_timer.h"
#include "timer.h"
#include "buzzer.h"
#include "type_def.h"
#include "datetime.h"
#include "config.h"
#include "sensor_DHT20.h"
#include "sensor_JW01.h"
#include "mode.h"
#include "device_gpio.h"
#include "device.h"
#include "sys.h"

//#define FAN PAout(5)
#define FAN_TIME 100
uint16_t time_ms;

uint16_t one_second_peroid = ONE_SECOND_PEROID;     //1000
uint16_t key_task_peroid = KEY_TASK_PEROID;
uint16_t mode_task_peroid = MODE_TASK_PERROID;
uint16_t display_task_peroid = DISPLAY_TASK_PEROID;
uint16_t status_check_peroid = STATUS_CHECK_PEROID;
uint16_t heat_time_peroid = HEAT_TIME_PEROID;
uint16_t pir_time_peroid = PIR_TIME_PEROID;
uint16_t countdown = 1;            	//标志位，预热60秒后置0

int heat_flag = 1;					//加热片片选标志
int heat_time = HEAT_TIME;
//temperature = DHT20_get_temperature();
unsigned int  i;
unsigned int count=0;
unsigned int buz_time=0;
uint16_t AlarmON = 0;
uint32_t AlarmOFF = 0;
unsigned int alarm_count = 0;			//报警间隔计数
unsigned int alarm_circle = 0;			//报警次数

extern void DHT11_update(void);
extern void DHT20_update(void);
extern void JW01_update(void);

extern u8 Key_Task(void);
extern void Mode_Task(void);
extern void Display_Task(u8 update);

void AlarmTIMEset()
{
    AlarmON = 1500;
    AlarmOFF = 350000;
    TIM_Cmd(TIMER_BUZ,ENABLE);
}
void AlarmTIMEreset()
{
    AlarmON = 0;
    AlarmOFF = 0;
    TIM_Cmd(TIMER_BUZ, DISABLE);
}

TIMER_MAIN_IRQ_HANDLER							//主定时器TIM3 执行每秒更新传感器数据、每分上传数据、检测按键功能
{
    if(TIM_GetITStatus(TIMER_MAIN,TIM_IT_Update) != RESET ) //是否发生中断
    {
        TIM_ClearITPendingBit(TIMER_MAIN, TIM_FLAG_Update);     //清除中断待处理位

        //time_ms--;

        //Datetime
        if(--one_second_peroid == 0)						//1秒执行一次
        {
            one_second_peroid = ONE_SECOND_PEROID; 			//恢复到1000的计数
            DHT20_update();									//更新读取DHT20数据
            JW01_update();									//更新读取JW01数据
#ifdef USE_TUYA_MODULE
            /*extern*/ void all_data_update(void);
            static int count = 0;
            count++;
            if(count==10)
            {
                count = 0;
                all_data_update();							//每60秒更新上报所有数据
            }
#endif
        }

        //Control
        //u8 update_mode = 0;
        if(--key_task_peroid == 0)							//每0.1秒检测按键
        {
            key_task_peroid = KEY_TASK_PEROID;
            /*update_mode =*/ Key_Task();
        }

        //Mode
        if(--mode_task_peroid == 0)							//每60秒判断一次当前模式并作出模式操作
        {
            //DHT11_update();
            if(countdown != 0)
            {
                countdown = 0;
            }
            mode_task_peroid = MODE_TASK_PERROID;
            Mode_Task();
        }

        //View
        if(--display_task_peroid == 0)						//每0.1秒
        {
            display_task_peroid = DISPLAY_TASK_PEROID;
            //Display_Task(update_mode);
            Buzzer_Output();								//检测蜂鸣器模块是否发声
        }

        //自动控制条件判断
        if(--status_check_peroid == 0 && countdown == 0)							//每1秒判断当前各指标，是否需要变更状态
        {
            status_check_peroid = STATUS_CHECK_PEROID;
            if(get_work_mode() == WORK_MODE_ZIDONG)
            {
                //当温度<10，或者湿度>65，启动烘干模式
                if(DHT20_get_temperature()<10 || DHT20_get_humidity()>65)					//判断温度大于25，湿度大于65
                {
                    enter_honggan_mode();
                    AlarmTIMEset();
                }
                //TVOC>0.6，或者CH2O>0.1，或者CO2>700，启动净化模式
                if(JW01_get_TVOC()>600 || JW01_get_CH2O()>100 || JW01_get_CO2()>1000)						//判断  恢复
                {
                    enter_jinhua_mode();
                    AlarmTIMEset();
                }
            }
            if(get_work_mode() == WORK_MODE_HONGGAN && get_work_type() == AUTO_CONTROL)		//开启烘干后达到条件停止烘干
            {
                //温度>10并且湿度<50，则结束烘干模式返回自动模式
                if(DHT20_get_temperature()>10 && DHT20_get_humidity()<50)					//判断湿度小于50
                {
                    enter_zidong_mode();
                    AlarmTIMEreset();

                }
            }
            if(get_work_mode() == WORK_MODE_JINGHUA && get_work_type() == AUTO_CONTROL)		//开启净化后达到条件停止净化
            {
                //TVOC<0.4并且CH2O<0.05并且CO2<500，则结束净化模式返回自动模式
                if(JW01_get_TVOC()<400 && JW01_get_CH2O()<60 && JW01_get_CO2()<800)					//判断   恢复
                {
                    enter_zidong_mode();
                    AlarmTIMEreset();
                }
            }

        }
        //蜂鸣器报警
        if(AlarmON != 0)
        {
            AlarmON --;
            TIM_Cmd(TIMER_BUZ,ENABLE);
        }
        if(AlarmON == 0 && AlarmOFF != 0)
        {
            AlarmOFF --;
            TIM_Cmd(TIMER_BUZ,DISABLE);
            if(AlarmON == 0 && AlarmOFF == 0)
            {
								alarm_circle ++;
                AlarmTIMEset();
            }
        }
        if(alarm_circle == 2)
        {
            alarm_circle = 0;
            AlarmTIMEreset();
        }

        //分时加热
        if(--heat_time_peroid == 0)						//1秒执行一次
        {
            heat_time_peroid = HEAT_TIME_PEROID;
            if(get_work_mode() == WORK_MODE_HONGGAN)
            {
                if(heat_flag == 1)
                {
                    if(heat_time == 5)
                    {
                        heat_time --;
                        HEATONE_OFF();

                    }
                    else if(heat_time == 0)
                    {
                        HEATTWO_ON();
                        heat_flag = 0;
                        heat_time = HEAT_TIME;
                    }
                    else
                    {
                        heat_time --;
                    }
                }
                if(heat_flag == 0)
                {
                    if(heat_time == 5)
                    {

                        heat_time --;
                        HEATTWO_OFF();

                    }
                    else if(heat_time == 0)
                    {
                        HEATONE_ON();
                        heat_flag = 1;
                        heat_time = HEAT_TIME;
                    }
                    else
                    {
                        heat_time --;
                    }
                }
            }

        }


        //红外感应检测
        if(--pir_time_peroid == 0)						//每0.5秒
        {
            pir_time_peroid = PIR_TIME_PEROID;
            //USART_SendData(USART3,GPIO_ReadInputDataBit(PIR_GPIO,PIR_GPIO_PIN));

            if(GPIO_ReadInputDataBit(PIR_GPIO,PIR_GPIO_PIN)==0)		//没检测到人不停止CY、UV 或者恢复
            {
                if(get_work_mode() == WORK_MODE_JINGHUA)
                {
                    CY_turn(1);							//开启臭氧
                    FAN_turn(1);							//开启风扇
                }
                if(get_work_mode() == WORK_MODE_SHAJUN)
                {
                    UV_turn(1);

                }

            }
            else
            {   //检测到人停止CY、UV
                if(get_work_mode() == WORK_MODE_JINGHUA)
                {
                    FAN_turn(0);
                    CY_turn(0);
                }
                if(get_work_mode() == WORK_MODE_SHAJUN)
                {
                    UV_turn(0);

                }
            }


        }
    }
}


TIMER_BUZ_IRQ_HANDLER							//
{
    if(TIM_GetITStatus(TIMER_BUZ,TIM_IT_Update) != RESET) //是否发生中断
    {

        TIM_ClearITPendingBit(TIMER_BUZ, TIM_FLAG_Update);     //清除中断待处理
        if(1)
        {
            if(buz_time<200)
            {
                buz_time++;
                BUZ_ON();
            }
            else
            {
                buz_time++;
                BUZ_OFF();
            }
            if(buz_time == 400)
            {
                buz_time = 0;
            }
        }

    }


}

//void reset_mode_task_timer()
//{
//	mode_task_peroid = MODE_TASK_PERROID;
//}

//void timer_delay_ms(uint16_t ms)
//{
//  time_ms = ms;
//  while(time_ms);
//}

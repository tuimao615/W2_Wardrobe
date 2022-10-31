#include "timer.h"

 /**
  * @file   TIM2_Config
  * @brief  调用函数库，初始化定时器2的配置
  * @param  无
  * @retval 无
  */
void Timer_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;

	//Clock
	RCC_APB1PeriphClockCmd(TIMER_PWM_RCC_APB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = (10-1);//自动重装载值10--定时时间(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler =(4800-1);//预分频值，+1为分频系数 
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIMER_PWM, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIMER_PWM,TIM_IT_Update,ENABLE);	//使能中断源
	TIM_Cmd(TIMER_PWM, ENABLE);  	//使能TIMx外设						 	
}

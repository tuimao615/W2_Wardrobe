#include "timer.h"
#include "config.h"


 /**
  * @file   TIM3_Config
  * @brief  调用函数库，初始化定时器2的配置
  * @param  无
  * @retval 无
  */
void Timer_Main_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;

	//Clock
	RCC_APB1PeriphClockCmd(TIMER_MAIN_RCC_APB, ENABLE); 
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = (10-1);//自动重装载值10--定时时间(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1);//预分频值，+1为分频系数 
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIMER_MAIN, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	
	//中断
	NVIC_Enable(TIMER_MAIN_IRQ, 2, 1);
	
	TIM_ITConfig(TIMER_MAIN,TIM_IT_Update,ENABLE);	//使能中断源


	TIM_Cmd(TIMER_MAIN, ENABLE);					 	
}

void Timer_BUZ_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;

	//Clock
	RCC_APB1PeriphClockCmd(TIMER_BUZ_RCC_APB, ENABLE); 
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = (800-1);//自动重装载值10--定时时间(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler	 =(1-1);//预分频值，+1为分频系数 
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIMER_BUZ, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	
	//中断
	NVIC_Enable(TIMER_BUZ_IRQ, 3, 3);
	
	TIM_ITConfig(TIMER_BUZ,TIM_IT_Update,ENABLE);	//使能中断源


	//TIM_Cmd(TIMER_BUZ, ENABLE);					 	
}

void Timer_CSB_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
	
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = (900-1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =(1-1); //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
							 
}

#include "timer.h"

 /**
  * @file   TIM2_Config
  * @brief  ���ú����⣬��ʼ����ʱ��2������
  * @param  ��
  * @retval ��
  */
void Timer_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;

	//Clock
	RCC_APB1PeriphClockCmd(TIMER_PWM_RCC_APB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = (10-1);//�Զ���װ��ֵ10--��ʱʱ��(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler =(4800-1);//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMER_PWM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIMER_PWM,TIM_IT_Update,ENABLE);	//ʹ���ж�Դ
	TIM_Cmd(TIMER_PWM, ENABLE);  	//ʹ��TIMx����						 	
}

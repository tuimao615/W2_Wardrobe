#include "timer.h"
#include "config.h"


 /**
  * @file   TIM3_Config
  * @brief  ���ú����⣬��ʼ����ʱ��2������
  * @param  ��
  * @retval ��
  */
void Timer_Main_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;

	//Clock
	RCC_APB1PeriphClockCmd(TIMER_MAIN_RCC_APB, ENABLE); 
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = (10-1);//�Զ���װ��ֵ10--��ʱʱ��(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1);//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMER_MAIN, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	
	//�ж�
	NVIC_Enable(TIMER_MAIN_IRQ, 2, 1);
	
	TIM_ITConfig(TIMER_MAIN,TIM_IT_Update,ENABLE);	//ʹ���ж�Դ


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
	
	//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
	TIM_TimeBaseStructure.TIM_Period = (800-1);//�Զ���װ��ֵ10--��ʱʱ��(10*4800/48M)s 
	TIM_TimeBaseStructure.TIM_Prescaler	 =(1-1);//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMER_BUZ, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	
	//�ж�
	NVIC_Enable(TIMER_BUZ_IRQ, 3, 3);
	
	TIM_ITConfig(TIMER_BUZ,TIM_IT_Update,ENABLE);	//ʹ���ж�Դ


	//TIM_Cmd(TIMER_BUZ, ENABLE);					 	
}

void Timer_CSB_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
	
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = (900-1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =(1-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
							 
}

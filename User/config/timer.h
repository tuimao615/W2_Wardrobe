#ifndef __TIMER_H
#define	__TIMER_H

#include "sys.h"




#define TIMER_MAIN							TIM4
#define TIMER_MAIN_RCC_APB		  RCC_APB1Periph_TIM4
#define TIMER_MAIN_IRQ					TIM4_IRQn
#define TIMER_MAIN_IRQ_HANDLER	void TIM4_IRQHandler(void)

#define TIMER_BUZ							TIM2
#define TIMER_BUZ_RCC_APB		  RCC_APB1Periph_TIM2
#define TIMER_BUZ_IRQ					TIM2_IRQn
#define TIMER_BUZ_IRQ_HANDLER	void TIM2_IRQHandler(void)
	
#define TIMER_CSB							TIM3
#define TIMER_CSB_RCC_APB		  RCC_APB1Periph_TIM3
#define TIMER_CSB_IRQ					TIM3_IRQn
#define TIMER_CSB_IRQ_HANDLER	void TIM3_IRQHandler(void)

#define TIMER_PWM							TIM14
#define TIMER_PWM_RCC_APB		  RCC_APB1Periph_TIM14
#define TIMER_PWM_IRQ					TIM14_IRQn
#define TIMER_PWM_IRQ_HANDLER	void TIM14_IRQHandler(void)


void Timer_Main_Config(void);
void Timer_BUZ_Config(void);
void Timer_PWM_Config(void);
void Timer_CSB_Config(void);

#endif

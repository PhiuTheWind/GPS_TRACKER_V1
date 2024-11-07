#include "TIM.h"
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

#ifdef TIM_Timer
	void TIM4_Init(void) //
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStructure.TIM_Prescaler = SystemCoreClock/1000000-1;
		TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;
		
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
		TIM_Cmd(TIM4,ENABLE);
		
	}

	void Delay_us(uint32_t us)
	{
		TIM_SetCounter(TIM4,0);
		while(TIM_GetCounter(TIM4) < us);
	}
	void Delay_ms(uint32_t ms)
	{
		while(ms)
		{
			Delay_us(1000);
			--ms;
		}
	}	
#endif

#ifdef Sys_Timer
	void Systick_Init(void)
	{
		SysTick->CTRL = 0;
		SysTick->LOAD = 0x00FFFFFF;
		SysTick->VAL = 0;
		SysTick->CTRL = 0x00000005;
	}
	void Delay_us(uint32_t us)
	{
		while(us)
		{
			SysTick->LOAD = SystemCoreClock/1000000-1;
			SysTick->VAL = 0;
			while((SysTick->CTRL & 0x00010000) == 0);
			--us;
		}
	}

	void Delay_ms(uint32_t ms)
	{
		while(ms)
		{
			SysTick->LOAD = SystemCoreClock/1000-1;
			SysTick->VAL = 0;
			while((SysTick->CTRL & 0x00010000) == 0);
			--ms;
		}
	}
#endif

#ifdef Sys_Interrupt
	void Systick_Interrupt(void)
	{
		SysTick_Config(SystemCoreClock/1000*Sys_Interrupt);
	}
#endif

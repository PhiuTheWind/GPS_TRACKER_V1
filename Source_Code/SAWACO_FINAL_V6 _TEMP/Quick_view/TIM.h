#include "common.h"


#ifdef TIM_Timer

	void TIM4_Init(void);
#endif

#ifdef Sys_Timer
	void Systick_Init(void);
#endif

#ifdef Sys_Interrupt 
	void Systick_Interrupt(void); //Max 233ms
#endif
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#include "Common.h"



#ifdef STANDBY
#define LED_Pin GPIO_Pin_13
#define WKUP_Pin GPIO_Pin_0
#define Sleep_Pin GPIO_Pin_15

void Standby_Init(void);
void Standby_ModeEntry(void);
void WKUP_EXTI0Mode(void);
void LED_Init(void);

void Sleep_Init(void);
void Sleep_Latch(void);

#define LSIFreq 40000

void IWDG_Init(int checktime_ms); //Min = 6.4ms/ Max = 26214.4ms

#endif


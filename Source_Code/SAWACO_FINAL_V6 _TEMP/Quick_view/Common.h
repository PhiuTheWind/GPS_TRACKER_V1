#define COMMON_H
#ifdef COMMON_H
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "math.h"
#include "string.h"
#include <stdlib.h>

//Mode.h
#define STANDBY

//TIM.h
#define Sys_Interrupt 500 //ms
#define TIM_Timer TIM4

//USART.h
#define USART4G USART1 
#define USART4G_Baudrate 115200

#define USARTGPS USART2 
#define USARTGPS_Baudrate 115200

#define USARTBLE USART3
#define USARTBLE_Baudrate 9600
//I2C.h
#define I2C_DS3231

#endif



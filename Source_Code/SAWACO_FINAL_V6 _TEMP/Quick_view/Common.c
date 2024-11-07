#include "Common.h"



uint32_t AHB(uint32_t PERIPH)
{
	if(PERIPH >= DMA1_BASE && PERIPH <=DMA1_Channel7_BASE) return RCC_AHBPeriph_DMA1;
	else if(PERIPH >= DMA2_BASE && PERIPH <=DMA2_Channel5_BASE) return RCC_AHBPeriph_DMA2;
	else if (PERIPH == CRC_BASE) return RCC_AHBPeriph_CRC;
	else return 0;
}

uint32_t APB1(uint32_t PERIPH)
{
	switch (PERIPH)
	{
		case TIM1_BASE:
				return RCC_APB1Periph_TIM2;
		case TIM2_BASE:
				return RCC_APB1Periph_TIM2;
		case TIM3_BASE:
				return RCC_APB1Periph_TIM3;
		case TIM4_BASE:
				return RCC_APB1Periph_TIM4;
		case TIM5_BASE:
				return RCC_APB1Periph_TIM5;
		case TIM6_BASE:
				return RCC_APB1Periph_TIM6;
		case TIM7_BASE:
				return RCC_APB1Periph_TIM7;
		case TIM12_BASE:
				return RCC_APB1Periph_TIM12;
		case TIM13_BASE:
				return RCC_APB1Periph_TIM13;
		case TIM14_BASE:
				return RCC_APB1Periph_TIM14;
		case WWDG_BASE:
				return RCC_APB1Periph_WWDG;
		case SPI2_BASE:
				return RCC_APB1Periph_SPI2;
		case SPI3_BASE:
				return RCC_APB1Periph_SPI3;
		case USART2_BASE:
				return RCC_APB1Periph_USART2;
		case USART3_BASE:
				return RCC_APB1Periph_USART3;
		case UART4_BASE:
				return RCC_APB1Periph_UART4;
		case UART5_BASE:
				return RCC_APB1Periph_UART5;
		case I2C1_BASE:
				return RCC_APB1Periph_I2C1;
		case I2C2_BASE:
				return RCC_APB1Periph_I2C2;
		case CAN1_BASE:
				return RCC_APB1Periph_CAN1;
		case CAN2_BASE:
				return RCC_APB1Periph_CAN2;
		case BKP_BASE:
				return RCC_APB1Periph_BKP;
		case PWR_BASE:
				return RCC_APB1Periph_PWR;
		case DAC_BASE:
				return RCC_APB1Periph_DAC;
		case CEC_BASE:
				return RCC_APB1Periph_CEC;
		default:
				return 0;
	}
}


uint32_t APB2(uint32_t PERIPH)
{
	switch (PERIPH) 
	{
		case GPIOA_BASE:
				return RCC_APB2Periph_GPIOA;
		case GPIOB_BASE:
				return RCC_APB2Periph_GPIOB;
		case GPIOC_BASE:
				return RCC_APB2Periph_GPIOC;
		case GPIOD_BASE:
				return RCC_APB2Periph_GPIOD;
		case GPIOE_BASE:
				return RCC_APB2Periph_GPIOE;
		case GPIOF_BASE:
				return RCC_APB2Periph_GPIOF;
		case GPIOG_BASE:
				return RCC_APB2Periph_GPIOG;
		case ADC1_BASE:
				return RCC_APB2Periph_ADC1;
		case ADC2_BASE:
				return RCC_APB2Periph_ADC2;
		case TIM1_BASE:
				return RCC_APB2Periph_TIM1;
		case SPI1_BASE:
				return RCC_APB2Periph_SPI1;
		case TIM8_BASE:
				return RCC_APB2Periph_TIM8;
		case USART1_BASE:
				return RCC_APB2Periph_USART1;
		case ADC3_BASE:
				return RCC_APB2Periph_ADC3;
		case TIM15_BASE:
				return RCC_APB2Periph_TIM15;
		case TIM16_BASE:
				return RCC_APB2Periph_TIM16;
		case TIM17_BASE:
				return RCC_APB2Periph_TIM17;
		case TIM9_BASE:
				return RCC_APB2Periph_TIM9;
		case TIM10_BASE:
				return RCC_APB2Periph_TIM10;
		case TIM11_BASE:
				return RCC_APB2Periph_TIM11;
		default:
				return 0;
	}
}


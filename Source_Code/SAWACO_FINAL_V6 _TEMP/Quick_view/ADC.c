#include "ADC.h"
#include "TIM.h"

void ADC_Battery_Init(unsigned int * ADCConvertedValue)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/*ADC*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
  /* Configure PC.04 (ADC Channel16) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DMA1 channel1 configuration ----------------------------------------------*/
   DMA_DeInit(DMA1_Channel1);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
   DMA_InitStructure.DMA_BufferSize = 1;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);
   
   /* Enable DMA1 channel1 */
   DMA_Cmd(DMA1_Channel1, ENABLE);
   
   /* ADC1 configuration ------------------------------------------------------*/
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_InitStructure.ADC_ScanConvMode = ENABLE;
   ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfChannel = 1;
   ADC_Init(ADC1, &ADC_InitStructure);
 
   /* ADC1 regular channel14 configuration */ 
   ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);
	
   /* Enable ADC1 DMA */
   ADC_DMACmd(ADC1, ENABLE);
   
   /* Enable ADC1 */
   ADC_Cmd(ADC1, ENABLE);
 
   /* Enable ADC1 reset calibration register */   
   ADC_ResetCalibration(ADC1);
   /* Check the end of ADC1 reset calibration register */
   while(ADC_GetResetCalibrationStatus(ADC1));
 
   /* Start ADC1 calibration */
   ADC_StartCalibration(ADC1);
   /* Check the end of ADC1 calibration */
   while(ADC_GetCalibrationStatus(ADC1));
      
   /* Start ADC1 Software Conversion */ 
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

float ADC_Battery_Read(unsigned int ADCConvertedValue, uint16_t Loop)
{
	float y[2];
	float Sum = 0;
	float Alpha = 0.5;
	y[1] = Alpha * ((float)ADCConvertedValue) / 4095;
	Sum += y[1];
	
	for (uint16_t i = 0; i < Loop; i++) {
			y[0] = y[1];
			y[1] = Alpha * ((float)ADCConvertedValue) / 4095 + (1.0 - Alpha) * y[0];
			Sum += y[1];
			Delay_us(10);
	}

	return (Sum / Loop) * VOLTAGE_FULL_CHARGE;
}



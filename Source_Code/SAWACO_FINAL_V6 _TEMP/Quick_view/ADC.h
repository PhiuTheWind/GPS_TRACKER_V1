#ifndef ADC_H
#define ADC_H

#include "Common.h"

#define VOLTAGE_FULL_CHARGE 4.2
#define Voltage_To_Percent(Vol) (120.1*Vol-399.3) 
void ADC_Battery_Init(unsigned int * ADCConvertedValue);
float ADC_Battery_Read(unsigned int ADCConvertedValue, uint16_t Loop);

#endif


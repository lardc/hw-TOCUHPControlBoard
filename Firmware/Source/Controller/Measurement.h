#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include "stdinc.h"

// Functions
//
uint16_t MEASURE_BatteryVoltage(uint16_t ADC1Channel, uint16_t RegisterOffset, uint16_t RegisterK);
void MEASURE_ConvertADCtoValx(volatile uint16_t *InputArray, float *OutputArray, uint16_t ArrayOffset, uint16_t DataLength,
							  uint16_t RegisterOffset, uint16_t RegisterK, uint16_t RegisterP0, uint16_t RegisterP1, uint16_t RegisterP2);

#endif // __MEASUREMENT_H

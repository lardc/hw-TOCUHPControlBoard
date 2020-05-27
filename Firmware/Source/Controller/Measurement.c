#include "Measurement.h"
#include "DataTable.h"
#include "ZwADC.h"


// Functions
//

uint16_t MEASURE_BatteryVoltage(uint16_t ADC1Channel, uint16_t RegisterOffset, uint16_t RegisterK)
{
	float Offset = (float)((int16_t)DataTable[RegisterOffset]);
	float K = (float)DataTable[RegisterK] / 1000;
	float result = ((float)ADC_Measure(ADC1, ADC1Channel) - Offset) * ADC_REF_VOLTAGE / ADC_RESOLUTION * K;

	return (result > 0) ? result : 0;
}
//------------------------------------------------------------------------------

void MEASURE_ConvertADCtoValx(volatile uint16_t *InputArray, float *OutputArray, uint16_t ArrayOffset, uint16_t DataLength,
							  uint16_t RegisterOffset, uint16_t RegisterK, uint16_t RegisterP0, uint16_t RegisterP1, uint16_t RegisterP2)
{
	uint16_t i;
	float tmp;

	float Offset = (float)((int16_t)DataTable[RegisterOffset]);
	float K = (float)DataTable[RegisterK] / 1000;
	//
	float P0 = (float)((int16_t)DataTable[RegisterP0]);
	float P1 = (float)DataTable[RegisterP1] / 1000;
	float P2 = (float)((int16_t)DataTable[RegisterP2]) / 1e6;

	for (i = 0; i < DataLength; ++i)
	{
		tmp = ((float)InputArray[i] - Offset) * ADC_REF_VOLTAGE / ADC_RESOLUTION * K;
		tmp = tmp * tmp * P2 + tmp * P1 + P0;
		OutputArray[i * 2 + ArrayOffset] = (tmp > 0) ? tmp : 0;
	}
}

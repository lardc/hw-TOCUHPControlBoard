#include "Measurement.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "ZwADC.h"

uint16_t MEASURE_GetBatteryVoltage()
{
	float Offset = (float)((int16_t)DataTable[REG_V_BAT_OFFSET]);
	float K = (float)DataTable[REG_V_BAT_K] / 1000;
	float result = ((float)ADC_Measure(ADC1, BAT_ADC1_CH) - Offset) * ADC_REF_VOLTAGE / ADC_RESOLUTION * K;
	
	return (result > 0) ? result : 0;
}
//------------------------------------------------

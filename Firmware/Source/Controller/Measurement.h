#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include "stdinc.h"

// Functions
//
float MEASURE_BatteryVoltage();
//float MEASURE_DUTCurrent();
//
// Raw ADC values
uint16_t MEASURE_BatteryVoltageRaw();
uint16_t MEASURE_DUTCurrentRaw();

#endif // __MEASUREMENT_H

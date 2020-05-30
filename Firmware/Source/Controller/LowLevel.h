#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Functions
//
void LL_ToggleBoardLED();
void LL_Fan(bool State);
void LL_BatteryDischarge(bool State);
void LL_ExternalLED(bool State);
void LL_MeanWellRelay(bool State);
void LL_PSBoardOutput(bool State);
void LL_SoftSpiSRCK(bool State);
void LL_SoftSpiRCK(bool State);
void LL_SoftSpiData(bool State);
void LL_WriteToGateRegister(uint16_t Data);
void LL_ForceSYNC(bool State);
bool LL_GetSYNCState();

#endif //__LOWLEVEL_H

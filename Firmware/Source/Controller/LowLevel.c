// Header
#include "LowLevel.h"
// Include
#include "Board.h"
#include "Delay.h"

// Functions
//
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_LED);
}
//-----------------------------

void LL_Fan(bool State)
{
	GPIO_SetState(GPIO_FAN, State);
}
//-----------------------------

void LL_BatteryDischarge(bool State)
{
	// Разряд происходит при низком уровне на пине
	GPIO_SetState(GPIO_BAT_CHARGE, !State);
}
//-----------------------------

void LL_ExternalLED(bool State)
{
	GPIO_SetState(GPIO_LED_EXT, State);
}
//-----------------------------

void LL_MeanWellRelay(bool State)
{
	GPIO_SetState(GPIO_MW_CTRL, State);
}
//-----------------------------

void LL_PSBoardOutput(bool State)
{
	GPIO_SetState(GPIO_HVPS_CTRL, State);
}
//-----------------------------

void LL_SoftSpiSRCK(bool State)
{
	GPIO_SetState(GPIO_SRCK, State);
}
//-----------------------------

void LL_SoftSpiRCK(bool State)
{
	GPIO_SetState(GPIO_RCK, State);
}
//-----------------------------

void LL_SoftSpiData(bool State)
{
	GPIO_SetState(GPIO_DATA, State);
}
//-----------------------------

void LL_WriteToGateRegister(uint16_t Data)
{
	for(uint8_t i = 0; i < 16; ++i)
	{
		LL_SoftSpiData((Data >> i) & 0x1);
		DELAY_US(1);
		LL_SoftSpiSRCK(TRUE);
		DELAY_US(1);
		LL_SoftSpiSRCK(FALSE);
	}

	LL_SoftSpiRCK(TRUE);
	DELAY_US(1);
	LL_SoftSpiRCK(FALSE);
	LL_SoftSpiData(FALSE);
}
//-----------------------------

void LL_ForceSYNC(bool State)
{
	// Синхронизация происходит при низком уровне на пине
	GPIO_SetState(GPIO_SYNC, !State);
}
//-----------------------------

bool LL_GetSYNCState()
{
	return GPIO_GetState(GPIO_SYNC);
}
//-----------------------------

// Header
#include "LowLevel.h"
// Include
#include "Board.h"

// Functions
//
void LL_ToggleBoardLED()
{
	GPIO_Bit_Toggle(GPIOB, Pin_4);
}
//-----------------------------

void LL_Fan(bool State)
{
	GPIO_SetState(GPIO_FAN, State);
}
//-----------------------------

void LL_BatteryDischarge(bool State)
{
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

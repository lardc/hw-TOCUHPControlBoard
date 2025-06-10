// Header
#include "LowLevel.h"
// Include
#include "Board.h"
#include "Delay.h"
#include "DataTable.h"

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

void LL_ToggleExternalLED()
{
	GPIO_Toggle(GPIO_LED_EXT);
}
//-----------------------------

void LL_MeanWellRelay(bool State)
{
	GPIO_SetState(GPIO_MW_CTRL, State);
}
//-----------------------------

void LL_PSBoardOutput(bool State)
{
	// Включение питания происходит при низком уровне на пине
	GPIO_SetState(GPIO_HVPS_CTRL, !State);
}
//-----------------------------

void LL_SoftSPILatch()
{
	GPIO_SetState(GPIO_RCK, true);
	DELAY_US(1);
	GPIO_SetState(GPIO_RCK, false);
}
//-----------------------------

void LL_WriteToGateRegister(uint16_t Word, bool Latch)
{
	for(int i = 15; i >= 0; i--)
	{
		// Выставление данных
		GPIO_SetState(GPIO_DATA, (Word >> i) & 0x1);

		// Тактирование
		DELAY_US(1);
		GPIO_SetState(GPIO_SRCK, true);
		DELAY_US(1);
		GPIO_SetState(GPIO_SRCK, false);
	}

	if(Latch)
		LL_SoftSPILatch();
}
//-----------------------------

void LL_SoftSPIOutputEnable(bool State)
{
	GPIO_SetState(GPIO_SYNC, !State);
}
//-----------------------------

bool LL_GetSYNCState()
{
	return !GPIO_GetState(GPIO_SYNC_EXT);
}
//-----------------------------

bool LL_IsOutputVoltageHigh()
{
	return DataTable[REG_USE_OUT_VOLTAGE_MONITOR] ? GPIO_GetState(GPIO_VOUT_STATE) : FALSE;
}
//-----------------------------

void LL_PulseSYNC()
{
	LL_SoftSPIOutputEnable(true);
	DELAY_US(90);
	LL_SoftSPIOutputEnable(false);
}
//-----------------------------

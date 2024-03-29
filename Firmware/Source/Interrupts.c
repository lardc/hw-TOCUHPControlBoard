// Include
#include "Interrupts.h"
//
#include "Controller.h"
#include "LowLevel.h"
#include "Board.h"
#include "SysConfig.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
//

// Variables
//
bool Impulse = false;

// Functions prototypes
void INT_SyncTimeoutControl(bool State);
//

// Functions
//
void EXTI9_5_IRQHandler()
{
	if(CONTROL_CheckDeviceSubState(SS_WaitingSync))
	{
		if(LL_GetSYNCState())
		{
			Impulse = false;

			CONTROL_SynchronizationTimeout = CONTROL_TimeCounter + DataTable[REG_SYNC_WAIT_TIMEOUT];
			CONTROL_PsBoardDisableTimeout = CONTROL_TimeCounter + DataTable[REG_PS_BOARD_DISABLE_TIMEOUT];
			CONTROL_AfterPulseTimeout = CONTROL_TimeCounter + DataTable[REG_AFTER_PULSE_TIMEOUT];

			DataTable[REG_OP_RESULT] = OPRESULT_OK;
			CONTROL_InitBatteryChargeProcess();
		}
		else
			Impulse = true;

		CONTROL_HandleFanLogic(Impulse);
		CONTROL_HandleLEDLogic(Impulse);
	}

	EXTI_FlagReset(EXTI_8);
}
//-----------------------------------------

void TIM1_BRK_TIM15_IRQHandler()
{
	if(CONTROL_CheckDeviceSubState(SS_WaitingSync))
		INT_SyncTimeoutControl(!LL_IsOutputVoltageHigh());
}
//-----------------------------------------

void TIM3_IRQHandler()
{
	if(TIM_StatusCheck(TIM3))
	{
		INT_SyncTimeoutControl(false);

		if(CONTROL_CheckDeviceSubState(SS_WaitingSync))
			CONTROL_CurrentEmergencyStop(DF_SYNC_TOO_LONG);

		TIM_StatusClear(TIM3);
	}
}
//-----------------------------------------

void USART1_IRQHandler()
{
	if(ZwSCI_RecieveCheck(USART1))
	{
		ZwSCI_RegisterToFIFO(USART1);
		ZwSCI_RecieveFlagClear(USART1);
	}
}
//-----------------------------------------
void USB_LP_CAN_RX0_IRQHandler()
{
	if(NCAN_RecieveCheck())
	{
		NCAN_RecieveData();
		NCAN_RecieveFlagReset();
	}
}
//-----------------------------------------

void TIM7_IRQHandler()
{
	static uint16_t LED_BlinkTimeCounter = 0;

	if(TIM_StatusCheck(TIM7))
	{
		CONTROL_TimeCounter++;
		if(++LED_BlinkTimeCounter > TIME_LED_BLINK)
		{
			LL_ToggleBoardLED();
			LED_BlinkTimeCounter = 0;
		}

		CONTROL_HandleFanLogic(Impulse);

		TIM_StatusClear(TIM7);
	}
}
//-----------------------------------------

void INT_SyncTimeoutControl(bool State)
{
	State ? TIM_Start(TIM3) : TIM_Stop(TIM3);
	TIM_Reset(TIM3);
}
//-----------------------------------------

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

// Defines
#define TIMEOUT_COEF_D		60000
//

// Functions prototypes
void INT_SyncTimeoutControl(bool State);
//

// Functions
//
void EXTI9_5_IRQHandler()
{
	float Current = 0, TimeoutCoefficient = 0;

	if(CONTROL_CheckDeviceSubState(SS_WaitingSync))
	{
		if(LL_GetSYNCState())
		{
			CONTROL_SetDeviceSubState(SS_None);

			INT_SyncTimeoutControl(false);

			DataTable[REG_OP_RESULT] = OPRESULT_OK;

			CONTROL_HandleFanLogic(true);
			LL_ExternalLED(true);
			LL_WriteToGateRegister(0);
			CONTROL_LEDTimeout = CONTROL_TimeCounter + TIME_EXT_LED_BLINK;
		}
		else
		{
			INT_SyncTimeoutControl(true);

			Current = (float)DataTable[REG_VOLTAGE_SETPOINT] / DataTable[REG_RESISTANCE_PER_LSB] * DataTable[REG_GATE_REGISTER];
			TimeoutCoefficient = (float)(DataTable[REG_VOLTAGE_SETPOINT] * Current) / TIMEOUT_COEF_D;

			if(TimeoutCoefficient < 1)
				TimeoutCoefficient = 1;

			CONTROL_AfterPulseTimeout = CONTROL_TimeCounter + DataTable[REG_AFTER_PULSE_TIMEOUT] * TimeoutCoefficient;

			CONTROL_StartBatteryCharge();
		}
	}

	EXTI_FlagReset(EXTI_8);
}
//-----------------------------------------

void TIM3_IRQHandler()
{
	if(TIM_StatusCheck(TIM3))
	{
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
		CONTROL_HandleFanLogic(false);
		CONTROL_HandleSynchronizationTimeout();

		CONTROL_TimeCounter++;
		if(++LED_BlinkTimeCounter > TIME_LED_BLINK)
		{
			LL_ToggleBoardLED();
			LED_BlinkTimeCounter = 0;
		}

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

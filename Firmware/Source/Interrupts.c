// Include
#include "Interrupts.h"
//
#include <InitConfig.h>
#include "stm32f30x.h"
#include "stdinc.h"
#include "DataTable.h"
#include "Controller.h"
#include "DeviceProfile.h"
#include "LowLevel.h"
#include "Board.h"

// Variables
//
//volatile Int64U LED_BlinkTimeCounter = 0;
//volatile bool Overflow90 = FALSE;
//volatile bool Overflow10 = FALSE;

// Functions
//
void EXTI3_IRQnHandler()
{
//	Overflow90 = TRUE;
	EXTI_FlagReset(EXTI_3);

	// замкнуть транзисторы на время
	CONTROL_SetDeviceState(DS_Config);
}
//-----------------------------------------

void EXTI9_5_IRQHandler()
{
	if (EXTI_FlagCheck(EXTI_8))
	{
//		Overflow10 = TRUE;
		EXTI_FlagReset(EXTI_8);
	}
}
//-----------------------------------------

void USART1_IRQHandler()
{
	if (ZwSCI_RecieveCheck(USART1))
	{
		ZwSCI_RegisterToFIFO(USART1);
		ZwSCI_RecieveFlagClear(USART1);
	}
}
//-----------------------------------------

void USB_LP_CAN_RX0_IRQHandler()
{
	if (NCAN_RecieveCheck())
	{
		NCAN_RecieveData();
		NCAN_RecieveFlagReset();
	}
}
//-----------------------------------------

void TIM7_IRQHandler()
{
	static uint16_t LED_BlinkTimeCounter = 0;

	if (TIM_StatusCheck(TIM7))
		{
		CONTROL_TimeCounter++;
			if (++LED_BlinkTimeCounter > TIME_LED_BLINK)
			{
				LL_ToggleBoardLED();
				LED_BlinkTimeCounter = 0;
			}

			TIM_StatusClear(TIM7);
		}
}
//-----------------------------------------

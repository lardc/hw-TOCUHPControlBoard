// Header
#include "LowLevel.h"

// Functions
//
// LED on board
void LL_ToggleBoardLED()
{
	GPIO_Bit_Toggle(GPIOB, Pin_4);
}

// Unit cooler
void LL_ExternalFan(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_0) : GPIO_Bit_Rst(GPIOB, Pin_0);
}
//-----------------------------

// Connect resistor to capasitors to dischargee him else charge
void LL_CapChargeDischarge(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_2) : GPIO_Bit_Rst(GPIOB, Pin_2);
}
//-----------------------------

// Led 24 V
void LL_LedPower(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_1) : GPIO_Bit_Rst(GPIOB, Pin_1);
}
//-----------------------------

// Power Relay
void LL_PowerRelay(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_11) : GPIO_Bit_Rst(GPIOB, Pin_11);
}
//-----------------------------

// PSBoard clock
void LL_PSBoard(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_10) : GPIO_Bit_Rst(GPIOB, Pin_10);
}
//-----------------------------

// Порты GateRaw SerialInterface

// SRCK
void LL_GateRawSRCK(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_5) : GPIO_Bit_Rst(GPIOA, Pin_5);
}
//-----------------------------

// RCK
void LL_GateRawRCK(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_4) : GPIO_Bit_Rst(GPIOA, Pin_4);
}
//-----------------------------

// DATA
void LL_GateRawData(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_7) : GPIO_Bit_Rst(GPIOA, Pin_7);
}
//-----------------------------

#include "Interrupts.h"
#include "InitConfig.h"
#include "SysConfig.h"

// Functions
//
void CONTROL_Init();
void CONTROL_Idle();

int main()
{
	__disable_irq();
	SCB->VTOR = (uint32_t) BOOT_LOADER_MAIN_PR_ADDR;
	__enable_irq();

	// Настройка системной частоты тактирования
	SysClk_Config();

	// Настройка портов
	IO_Config();

	// Настройка внешних прерываний
	EI_Config();

	// Настройка CAN
	CAN_Config();

	// Настройка UART
	UART_Config();

	// Настройка таймеров
//	Timer3_Config();
	Timer7_Config(); //	Системный счетчик, период 1мС
	Timer15_Config(); //	Цифровой регулятор

	// Настройка АЦП
	ADC_Init();

	// Настройка ДМА
//	DMA_Init();				// Надо настроить

	// Настройка сторожевого таймера
	WatchDog_Config();

	// Инициализация логики контроллера
	CONTROL_Init();

	// Фоновый цикл
	while(TRUE)
	{
		CONTROL_Idle();
	}
	return 0;
}

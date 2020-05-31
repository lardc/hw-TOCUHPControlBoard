#include "InitConfig.h"
#include "Board.h"
#include "SysConfig.h"

// Functions
//
Boolean SysClk_Config()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------

void EI_Config()
{
	EXTI_Config(EXTI_PA, EXTI_8, RISE_TRIG, 0);
	EXTI_EnableInterrupt(EXTI3_IRQn, 0, true);
}
//------------------------------------------------

void IO_Config()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	
	// Аналаговые входы
	GPIO_Config(GPIOA, Pin_3, Analog, NoPull, HighSpeed, NoPull);
	
	// Линия синхронизации (вход - выход)
	GPIO_Config(GPIOA, Pin_8, Output, OpenDrain, HighSpeed, NoPull);
	GPIO_SetState(GPIO_SYNC, true);
	
	// Выходы
	GPIO_InitPushPullOutput(GPIO_RCK);
	GPIO_InitPushPullOutput(GPIO_SRCK);
	GPIO_InitPushPullOutput(GPIO_DATA);
	GPIO_InitPushPullOutput(GPIO_FAN);
	GPIO_InitPushPullOutput(GPIO_LED);
	GPIO_InitPushPullOutput(GPIO_LED_EXT);
	GPIO_InitPushPullOutput(GPIO_BAT_CHARGE);
	GPIO_InitPushPullOutput(GPIO_HVPS_CTRL);
	GPIO_InitPushPullOutput(GPIO_MW_CTRL);
	
	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART_TX, AltFn_7);
}
//------------------------------------------------

void CAN_Config()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, 0, 0); // Фильтр 0 пропускает все сообщения
}
//------------------------------------------------

void UART_Config()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//------------------------------------------------

void ADC_Init()
{
	RCC_ADC_Clk_EN(ADC_12_ClkEN);
	
	ADC_Calibration(ADC1);
	ADC_SoftTrigConfig(ADC1);
	ADC_Enable(ADC1);
}
//------------------------------------------------

void Timer7_Config()
{
	TIM_Clock_En(TIM_7);
	TIM_Config(TIM7, SYSCLK, TIMER7_uS);
	TIM_Interupt(TIM7, 0, true);
	TIM_Start(TIM7);
}
//------------------------------------------------

void WatchDog_Config()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//------------------------------------------------

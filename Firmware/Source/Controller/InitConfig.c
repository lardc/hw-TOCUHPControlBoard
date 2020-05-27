#include <InitConfig.h>

// Functions
//
Boolean SysClk_Config()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------------------------------------

void EI_Config()
{
//	// Overflow 90%
//	EXTI_Config(EXTI_PB, EXTI_4, FALL_TRIG, 0);
//	EXTI_EnableInterrupt(EXTI4_IRQn, 0, true);
//
//	// Overflow 10%
//	EXTI_Config(EXTI_PB, EXTI_8, FALL_TRIG, 0);
//	EXTI_EnableInterrupt(EXTI9_5_IRQn, 0, true);

    EXTI_Config(EXTI_PA, EXTI_8, FALL_TRIG, 0);
    EXTI_EnableInterrupt(EXTI3_IRQn, 0, true);

}
//------------------------------------------------------------------------------

void IO_Config()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);

	GPIO_Config(GPIOA, Pin_3, Analog, NoPull, HighSpeed, NoPull);

	// Цифровые входы
	// PA8 - SYNC_LINE
	GPIO_Config(GPIOA, Pin_8, Input, NoPull, HighSpeed, NoPull);

	// Выходы
	// PA4 - RCK
	GPIO_Config(GPIOA, Pin_4, Output, PushPull, HighSpeed, NoPull);

	// PA5 - SRCK
	GPIO_Config(GPIOA, Pin_5, Output, PushPull, HighSpeed, NoPull);

	// PB0 - FAN
	GPIO_Config(GPIOB, Pin_0, Output, PushPull, HighSpeed, NoPull);

	// PB1 - LED_24V
	GPIO_Config(GPIOB, Pin_1, Output, PushPull, HighSpeed, NoPull);

	// PB2 - BAT_CHARGE
	GPIO_Config(GPIOB, Pin_2, Output, PushPull, HighSpeed, NoPull);

	// PB4 - LED
	GPIO_Config(GPIOB, Pin_4, Output, PushPull, HighSpeed, NoPull);

	// PB10 - PS_BOARD
	GPIO_Config(GPIOB, Pin_10, Output, PushPull, HighSpeed, NoPull);

	// PB11 - POWER_RELLAY
	GPIO_Config(GPIOB, Pin_11, Output, PushPull, HighSpeed, NoPull);

	// Альтернативные функции
	GPIO_Config(GPIOA, Pin_11, AltFn, PushPull, HighSpeed, NoPull); // PA11 (CAN RX)
	GPIO_AltFn(GPIOA, Pin_11, AltFn_9);
	GPIO_Config(GPIOA, Pin_12, AltFn, PushPull, HighSpeed, NoPull); // PA12 (CAN TX)
	GPIO_AltFn(GPIOA, Pin_12, AltFn_9);
	GPIO_Config(GPIOA, Pin_9, AltFn, PushPull, HighSpeed, NoPull); // PA9(USART1 TX)
	GPIO_AltFn(GPIOA, Pin_9, AltFn_7);
	GPIO_Config(GPIOA, Pin_10, AltFn, PushPull, HighSpeed, NoPull); // PA10(USART1 RX)
	GPIO_AltFn(GPIOA, Pin_10, AltFn_7);
}
//------------------------------------------------------------------------------

void CAN_Config()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, FALSE);
	NCAN_FIFOInterrupt(TRUE);
	NCAN_FilterInit(0, 0, 0); // Фильтр 0 пропускает все сообщения
}
//------------------------------------------------------------------------------

void UART_Config()
{
	USART_Init(USART1, SYSCLK, USART_BAUDRATE);
	USART_Recieve_Interupt(USART1, 0, true);
}
//------------------------------------------------------------------------------

void ADC_Init()
{
//	RCC_ADC_Clk_EN(ADC_12_ClkEN);
//	ADC_Calibration(ADC1);
//	ADC_SoftTrigConfig(ADC1);
//	ADC_ChannelSet_SampleTime(ADC1, 1, ADC_SMPL_TIME_7_5);
//	ADC_ChannelSet_SampleTime(ADC1, 2, ADC_SMPL_TIME_7_5);
//	ADC_Enable(ADC1);


    RCC_ADC_Clk_EN(ADC_12_ClkEN);
    ADC_Calibration(ADC1);
    ADC_SoftTrigConfig(ADC1);
    ADC_ChannelSeqLen(ADC1, ADC_DMA_BUFF_SIZE);
    for (uint8_t i = 1; i <= ADC_DMA_BUFF_SIZE; ++i)
        ADC_ChannelSet_Sequence(ADC1, 4, i);
    ADC_ChannelSet_SampleTime(ADC1, 4, ADC_SMPL_TIME_7_5);
    ADC_DMAConfig(ADC1);
    ADC_Enable(ADC1);
}
//------------------------------------------------------------------------------

void Timer7_Config()
{
	TIM_Clock_En(TIM_7);
	TIM_Config(TIM7, SYSCLK, TIMER7_uS);
	TIM_Interupt(TIM7, 0, true);
	TIM_Start(TIM7);
}

void Timer15_Config()
{
    TIM_Clock_En(TIM_15);
    TIM_Config(TIM15, SYSCLK, TIMER15_uS);
    TIM_Interupt(TIM15, TIM15_PRIORITY, true);
    TIM_MasterMode(TIM15, MMS_UPDATE);
}
//------------------------------------------------------------------------------

void WatchDog_Config()
{
	IWDG_Config();
	IWDG_ConfigureFastUpdate();
}
//------------------------------------------------------------------------------


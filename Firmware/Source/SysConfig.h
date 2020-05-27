#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H


// Flash loader options
#define BOOT_LOADER_VARIABLE			(*((volatile uint32_t *)0x20000000))
#define BOOT_LOADER_REQUEST				0x12345678
#define BOOT_LOADER_MAIN_PR_ADDR		0x08008000
//-----------------------------------------------

// System clock
#define SYSCLK							70000000	// �������� ������� ��������� ���� ����������
#define QUARTZ_FREQUENCY				20000000	// ������� ������
// ----------------------------------------------

// USART
#define USART_BAUDRATE					115200		// �������� USART
#define USART_FIFOlen					32			// ����� FIFO USART
// ----------------------------------------------

// Timers
#define TIMER15_uS						30
#define TIMER7_uS						1000

#define TIM15_PRIORITY					31
#define TIM7_PRIORITY					62
// ----------------------------------------------

// CAN
#define CAN_BAUDRATE					1000000		// ������� CAN
// ----------------------------------------------

//
#define ADC_DMA_BUFF_SIZE				8			// ������ ������ ��� ���
// ----------------------------------------------

#define CHARGE							0			// ����� �������������
#define DISCHARGE						1			// ������ �������������
// ----------------------------------------------



#endif // __SYSCONFIG_H

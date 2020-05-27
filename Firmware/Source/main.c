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

	// ��������� ��������� ������� ������������
	SysClk_Config();

	// ��������� ������
	IO_Config();

	// ��������� ������� ����������
	EI_Config();

	// ��������� CAN
	CAN_Config();

	// ��������� UART
	UART_Config();

	// ��������� ��������
//	Timer3_Config();
	Timer7_Config(); //	��������� �������, ������ 1��
	Timer15_Config(); //	�������� ���������

	// ��������� ���
	ADC_Init();

	// ��������� ���
//	DMA_Init();				// ���� ���������

	// ��������� ����������� �������
	WatchDog_Config();

	// ������������� ������ �����������
	CONTROL_Init();

	// ������� ����
	while(TRUE)
	{
		CONTROL_Idle();
	}
	return 0;
}

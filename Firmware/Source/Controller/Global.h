#ifndef __GLOBAL_H
#define __GLOBAL_H

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS		1000	// ������� ���������� SCCI (� ��)
#define EP_WRITE_COUNT			0		// ���������� �������� ��� ������
#define EP_COUNT				0		// ���������� �������� ��� ������
#define ENABLE_LOCKING			FALSE	// ������ NV ��������� �������

// ��������� ���������
#define TIME_LED_BLINK			500		// ������� ����������� (� ��)
#define TIME_EXT_LED_BLINK		500		// ����� ��������� �������� ���������� (� ��)

// ��������� ���
#define BAT_ADC1_CH				4		// ����� ������ ��� �������
#define ADC_REF_VOLTAGE			3300.0f	// ������� ���������� (� ��)
#define ADC_RESOLUTION			4095	// ���������� ���

#endif //  __GLOBAL_H

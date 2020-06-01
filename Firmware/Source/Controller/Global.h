#ifndef __GLOBAL_H
#define __GLOBAL_H

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS		1000	// Таймаут интерфейса SCCI (в мс)
#define EP_WRITE_COUNT			0		// Количество массивов для записи
#define EP_COUNT				0		// Количество массивов для чтения
#define ENABLE_LOCKING			FALSE	// Защита NV регистров паролем

// Временные параметры
#define TIME_LED_BLINK			500		// Мигание светодиодом (в мс)
#define TIME_EXT_LED_BLINK		500		// Время загорания внешнего светодиода (в мс)

// Параметры АЦП
#define BAT_ADC1_CH				4		// Номер канала АЦП батареи
#define ADC_REF_VOLTAGE			3300.0f	// Опорное напряжение (в мВ)
#define ADC_RESOLUTION			4095	// Разрешение АЦП

#endif //  __GLOBAL_H

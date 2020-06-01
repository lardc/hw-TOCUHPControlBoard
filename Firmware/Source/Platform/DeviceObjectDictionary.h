#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning

#define ACT_DBG_FAN						10	// Импульсное включение вентилятора
#define ACT_DBG_DISCHARGE				11	// Импульсное включение реле разряда конденстаторов
#define ACT_DBG_EXT_LED					12	// Импульсное включение внешней индикации
#define ACT_DBG_MW_RELAY				13	// Импульсное включение реле питания БП MeanWell
#define ACT_DBG_PSBOARD_OUTPUT			14	// Импульсное включение сигнала для PSBOARD
#define ACT_DBG_GATE_CONTROL			15	// Запись значения в сдвиговый регистр управления затворами

#define ACT_PULSE_CONFIG				100	// Конфигурация напряжения и настройка затворов транзисторов

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------

// Регистры
// Сохраняемые регистры
#define REG_V_BAT_OFFSET				0	// Смещение оцифрованного напряжения батареи 1 (в тиках)
#define REG_V_BAT_K						1	// Коэффициент пересчёта напряжения АЦП (в мВ) в напряжение батареи (в В) x1000
#define REG_VOLTAGE_ERROR_LIMIT			2	// Допустимая ошибка регулирования напряжения (в В)
#define REG_BAT_CHARGE_TIMEOUT			3	// Таймаут выхода батареи на заданное напряжение (в мс)
#define REG_FAN_OPERATE_TIME			4	// Время работы вентилятора после импульса (в с)
#define REG_FAN_OPERATE_PERIOD			5	// В простое вентилятор включается не реже чем (в с)

// Несохраняемы регистры чтения-записи
#define REG_VOLTAGE_SETPOINT			128	// Значение задания напряжения (в В)
#define REG_GATE_REGISTER				129	// Значение-маска конфигурации затворов

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции
//
#define REG_ACTUAL_BAT_VOLTAGE			200	// Текущее напряжение на батарее (в В)
// -----------------------------

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0
#define DF_BATTERY						1	// Проблема заряда батареи

// Problem
#define PROBLEM_NONE					0

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

#endif //  __DEV_OBJ_DIC_H

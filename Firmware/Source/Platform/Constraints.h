#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
#define BAT_VOLTAGE_MIN				0		// в В
#define BAT_VOLTAGE_MAX				1500	// в В
#define BAT_VOLTAGE_DEF				500		// в В

#define GAIN_COEFFICIENT_MIN		1
#define GAIN_COEFFICIENT_MAX		10000

#define GAIN_VOLTAGE_DEF			1000

#define VOLTAGE_ERROR_MIN			1		// в % / 10
#define VOLTAGE_ERROR_MAX			100		// в % / 10
#define VOLTAGE_ERROR_DEF			5		// в % / 10

#define CHARGE_TIMEOUT_MIN			1000	// в мс
#define CHARGE_TIMEOUT_MAX			60000	// в мс
#define CHARGE_TIMEOUT_DEF			10000	// в мс

#define FAN_OPERATE_MIN				10		// в сек
#define FAN_OPERATE_MAX				300		// в сек
#define FAN_OPERATE_DEF				60		// в сек

#define RECHARGE_TIMEOUT_MIN		0		// в мс
#define RECHARGE_TIMEOUT_MAX		500		// в мс
#define RECHARGE_TIMEOUT_DEF		10		// в мс

#define RESISTANCE_PER_LSB_MIN		240		// в Ом
#define RESISTANCE_PER_LSB_MAX		10000	// в Ом
#define RESISTANCE_PER_LSB_DEF		3000	// в Ом

#define CURRENT_MAX_PER_BIT_MIN		1		// в А
#define CURRENT_MAX_PER_BIT_MAX		1500	// в А
#define CURRENT_MAX_PER_BIT_DEF		50		// в А

#define SYNC_WAIT_TIMEOUT_MIN		0		// в мс
#define SYNC_WAIT_TIMEOUT_MAX		10000	// в мс
#define SYNC_WAIT_TIMEOUT_DEF		20		// в мс

#define AFTER_PULSE_TIMEOUT_MIN		0		// в мс
#define AFTER_PULSE_TIMEOUT_MAX		30000	// в мс
#define AFTER_PULSE_TIMEOUT_DEF		1000	// в мс

// Types
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];

#endif // __CONSTRAINTS_H

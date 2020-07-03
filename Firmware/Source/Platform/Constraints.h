#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
#define BAT_VOLTAGE_MIN				400		// ג ֲ
#define BAT_VOLTAGE_MAX				1500	// ג ֲ
#define BAT_VOLTAGE_DEF				500		// ג ֲ

#define GAIN_COEFFICIENT_MIN		1
#define GAIN_COEFFICIENT_MAX		10000

#define GAIN_VOLTAGE_DEF			1000

#define VOLTAGE_ERROR_MIN			1		// ג ֲ
#define VOLTAGE_ERROR_MAX			20		// ג ֲ
#define VOLTAGE_ERROR_DEF			5		// ג ֲ

#define CHARGE_TIMEOUT_MIN			1000	// ג לס
#define CHARGE_TIMEOUT_MAX			60000	// ג לס
#define CHARGE_TIMEOUT_DEF			10000	// ג לס

#define FAN_OPERATE_MIN				10		// ג סוך
#define FAN_OPERATE_MAX				300		// ג סוך
#define FAN_OPERATE_DEF				60		// ג סוך

#define RECHARGE_TIMEOUT_MIN		0		// ג לס
#define RECHARGE_TIMEOUT_MAX		100		// ג לס
#define RECHARGE_TIMEOUT_DEF		10		// ג לס

#define RESISTANCE_PER_LSB_MIN		500		// ג ־ל
#define RESISTANCE_PER_LSB_MAX		10000	// ג ־ל
#define RESISTANCE_PER_LSB_DEF		3000	// ג ־ל

#define CURRENT_MAX_PER_BIT_MIN		1		// ג ְ
#define CURRENT_MAX_PER_BIT_MAX		100		// ג ְ
#define CURRENT_MAX_PER_BIT_DEF		50		// ג ְ

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

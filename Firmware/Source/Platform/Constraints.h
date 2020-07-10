#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
#define BAT_VOLTAGE_MIN				400		// � �
#define BAT_VOLTAGE_MAX				1500	// � �
#define BAT_VOLTAGE_DEF				500		// � �

#define GAIN_COEFFICIENT_MIN		1
#define GAIN_COEFFICIENT_MAX		10000

#define GAIN_VOLTAGE_DEF			1000

#define VOLTAGE_ERROR_MIN			1		// � �
#define VOLTAGE_ERROR_MAX			20		// � �
#define VOLTAGE_ERROR_DEF			5		// � �

#define CHARGE_TIMEOUT_MIN			1000	// � ��
#define CHARGE_TIMEOUT_MAX			60000	// � ��
#define CHARGE_TIMEOUT_DEF			10000	// � ��

#define FAN_OPERATE_MIN				10		// � ���
#define FAN_OPERATE_MAX				300		// � ���
#define FAN_OPERATE_DEF				60		// � ���

#define RECHARGE_TIMEOUT_MIN		0		// � ��
#define RECHARGE_TIMEOUT_MAX		100		// � ��
#define RECHARGE_TIMEOUT_DEF		10		// � ��

#define RESISTANCE_PER_LSB_MIN		500		// � ��
#define RESISTANCE_PER_LSB_MAX		10000	// � ��
#define RESISTANCE_PER_LSB_DEF		3000	// � ��

#define CURRENT_MAX_PER_BIT_MIN		1		// � �
#define CURRENT_MAX_PER_BIT_MAX		100		// � �
#define CURRENT_MAX_PER_BIT_DEF		50		// � �

#define SYNC_WAIT_TIMEOUT_MIN		0		// � ��
#define SYNC_WAIT_TIMEOUT_MAX		10000	// � ��
#define SYNC_WAIT_TIMEOUT_DEF		20		// � ��

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

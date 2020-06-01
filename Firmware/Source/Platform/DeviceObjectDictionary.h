#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
#define ACT_ENABLE_POWER				1	// ������� � ��������� ��������
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_FAULT_CLEAR					3	// ������� fault
#define ACT_WARNING_CLEAR				4	// ������� warning

#define ACT_DBG_FAN						10	// ���������� ��������� �����������
#define ACT_DBG_DISCHARGE				11	// ���������� ��������� ���� ������� ��������������
#define ACT_DBG_EXT_LED					12	// ���������� ��������� ������� ���������
#define ACT_DBG_MW_RELAY				13	// ���������� ��������� ���� ������� �� MeanWell
#define ACT_DBG_PSBOARD_OUTPUT			14	// ���������� ��������� ������� ��� PSBOARD
#define ACT_DBG_GATE_CONTROL			15	// ������ �������� � ��������� ������� ���������� ���������

#define ACT_PULSE_CONFIG				100	// ������������ ���������� � ��������� �������� ������������

#define ACT_SAVE_TO_ROM					200	// ���������� ���������������� ������ �� FLASH ����������
#define ACT_RESTORE_FROM_ROM			201	// �������������� ������ �� FLASH
#define ACT_RESET_TO_DEFAULT			202	// ����� DataTable � ��������� �� ���������

#define ACT_BOOT_LOADER_REQUEST			320	// ���������� ���������� � ����� ��������������������
// -----------------------------

// ��������
// ����������� ��������
#define REG_V_BAT_OFFSET				0	// �������� ������������� ���������� ������� 1 (� �����)
#define REG_V_BAT_K						1	// ����������� ��������� ���������� ��� (� ��) � ���������� ������� (� �) x1000
#define REG_VOLTAGE_ERROR_LIMIT			2	// ���������� ������ ������������� ���������� (� �)
#define REG_BAT_CHARGE_TIMEOUT			3	// ������� ������ ������� �� �������� ���������� (� ��)
#define REG_FAN_OPERATE_TIME			4	// ����� ������ ����������� ����� �������� (� �)
#define REG_FAN_OPERATE_PERIOD			5	// � ������� ���������� ���������� �� ���� ��� (� �)

// ������������ �������� ������-������
#define REG_VOLTAGE_SETPOINT			128	// �������� ������� ���������� (� �)
#define REG_GATE_REGISTER				129	// ��������-����� ������������ ��������

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_OP_RESULT					197	// ������� ���������� ��������
//
#define REG_ACTUAL_BAT_VOLTAGE			200	// ������� ���������� �� ������� (� �)
// -----------------------------

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0
#define DF_BATTERY						1	// �������� ������ �������

// Problem
#define PROBLEM_NONE					0

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	//  �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	//  ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	//  ������������ ����

#endif //  __DEV_OBJ_DIC_H

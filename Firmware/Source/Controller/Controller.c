// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "Delay.h"
#include "DataTable.h"
#include "DeviceProfile.h"
#include "Interrupts.h"
#include "Global.h"
#include "LowLevel.h"
#include "Measurement.h"
#include "SysConfig.h"

// Macro
//
#define ABS(a)	(((a) < 0) ? -(a) : (a))

// Types
//
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
static Boolean CycleActive = false;
static uint16_t ActualBatteryVoltage = 0, TargetBatteryVoltage = 0;

volatile Int64U CONTROL_TimeCounter = 0;
Int64U CONTROL_ChargeTimeout = 0, CONTROL_LEDTimeout = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState);
void CONTROL_SwitchToFault(Int16U Reason);
void Delay_mS(uint32_t Delay);
void CONTROL_WatchDogUpdate();
void CONTROL_StartBatteryCharge();
void CONTROL_BatteryChargeLogic();
void CONTROL_HandleLEDLogic();
void CONTROL_SampleBatteryVoltage();
void CONTROL_ResetToDefaultState();
void CONTROL_ResetHardware();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT];
	Int16U EPSized[EP_COUNT];
	pInt16U EPCounters[EP_COUNT];
	pInt16U EPDatas[EP_COUNT];
	
	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, false);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();

	CONTROL_ResetToDefaultState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	DataTable[REG_ACTUAL_BAT_VOLTAGE] = 0;
	
	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();

	CONTROL_ResetHardware();
	CONTROL_SetDeviceState(DS_None);
}
//------------------------------------------

void CONTROL_ResetHardware()
{
	LL_Fan(false);
	LL_ExternalLED(false);
	LL_MeanWellRelay(false);
	LL_PSBoardOutput(false);
	LL_ForceSYNC(false);
	//
	LL_BatteryDischarge(true);
}
//------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	
	CONTROL_SampleBatteryVoltage();
	CONTROL_BatteryChargeLogic();

	CONTROL_HandleLEDLogic();

	CONTROL_WatchDogUpdate();
}
//------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if(CONTROL_State == DS_None)
				{
					LL_MeanWellRelay(true);
					CONTROL_StartBatteryCharge();
				}
				else if(CONTROL_State != DS_Ready || CONTROL_State != DS_InProcess)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if(CONTROL_State == DS_Ready || CONTROL_State == DS_InProcess)
				{
					CONTROL_ResetToDefaultState();
				}
				else if(CONTROL_State != DS_None)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
				{
					CONTROL_ResetToDefaultState();
				}
			}
			break;
			
		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = 0;
			break;
			
		case ACT_PULSE_CONFIG:
			{
				if(CONTROL_State == DS_Ready)
				{
					DataTable[REG_OP_RESULT] = OPRESULT_NONE;

					LL_WriteToGateRegister(DataTable[REG_GATE_REGISTER]);
					CONTROL_StartBatteryCharge();
				}
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;

			// Блок отладочных функция
			
		case ACT_DBG_FAN:
			{
				LL_Fan(true);
				Delay_mS(1000);
				LL_Fan(false);
			}
			break;
			
		case ACT_DBG_DISCHARGE:
			{
				LL_BatteryDischarge(true);
				Delay_mS(1000);
				LL_BatteryDischarge(false);
			}
			break;
			
		case ACT_DBG_EXT_LED:
			{
				LL_ExternalLED(true);
				Delay_mS(1000);
				LL_ExternalLED(false);
			}
			break;
			
		case ACT_DBG_MW_RELAY:
			{
				LL_MeanWellRelay(true);
				Delay_mS(1000);
				LL_MeanWellRelay(false);
			}
			break;
			
		case ACT_DBG_PSBOARD_OUTPUT:
			{
				LL_PSBoardOutput(true);
				Delay_mS(1000);
				LL_PSBoardOutput(false);
			}
			break;
			
		case ACT_DBG_GATE_CONTROL:
			LL_WriteToGateRegister(DataTable[REG_GATE_REGISTER]);
			break;
			
		default:
			return false;
	}
	
	return true;
}
//------------------------------------------

void CONTROL_StartBatteryCharge()
{
	TargetBatteryVoltage = DataTable[REG_VOLTAGE_SETPOINT];
	CONTROL_ChargeTimeout = CONTROL_TimeCounter + DataTable[REG_BAT_CHARGE_TIMEOUT];

	CONTROL_SetDeviceState(DS_InProcess);
}
//------------------------------------------

void CONTROL_BatteryChargeLogic()
{
	int16_t VoltageError = (int16_t)TargetBatteryVoltage - ActualBatteryVoltage;
	int16_t VoltageErrorLimit = DataTable[REG_VOLTAGE_SETPOINT];
	
	// Поддержание напряжения на батарее
	if(CONTROL_State == DS_InProcess || CONTROL_State == DS_Ready)
	{
		if(VoltageError > VoltageErrorLimit)
		{
			// Зона активного заряда
			LL_PSBoardOutput(true);
			LL_BatteryDischarge(false);
		}
		else if(VoltageError < -2 * VoltageErrorLimit)
		{
			// Зона активного разряда
			LL_PSBoardOutput(false);
			LL_BatteryDischarge(true);
		}
		else if(VoltageError < -VoltageErrorLimit)
		{
			// Зона пассивного разряда
			LL_PSBoardOutput(false);
			LL_BatteryDischarge(false);
		}
	}
	
	// Условие смены состояния
	if(CONTROL_State == DS_InProcess)
	{
		if(ABS(VoltageError) < VoltageErrorLimit)
			CONTROL_SetDeviceState(DS_Ready);
		else if(CONTROL_TimeCounter > CONTROL_ChargeTimeout)
			CONTROL_SwitchToFault(DF_BATTERY);
	}
}
//------------------------------------------

void CONTROL_HandleFanLogic(bool IsImpulse)
{
	static uint32_t IncrementCounter = 0;
	static uint64_t FanOnTimeout = 0;

	// Увеличение счётчика в простое
	if (!IsImpulse)
		IncrementCounter++;

	// Включение вентилятора
	if ((IncrementCounter > ((uint32_t)DataTable[REG_FAN_OPERATE_PERIOD] * 1000)) || IsImpulse)
	{
		IncrementCounter = 0;
		FanOnTimeout = CONTROL_TimeCounter + ((uint32_t)DataTable[REG_FAN_OPERATE_TIME] * 1000);
		LL_Fan(true);
	}

	// Отключение вентилятора
	if (FanOnTimeout && (CONTROL_TimeCounter > FanOnTimeout))
	{
		FanOnTimeout = 0;
		LL_Fan(false);
	}
}
//-----------------------------------------------

void CONTROL_HandleLEDLogic()
{
	if (CONTROL_LEDTimeout && (CONTROL_TimeCounter > CONTROL_LEDTimeout))
	{
		CONTROL_LEDTimeout = 0;
		LL_ExternalLED(false);
	}
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_ResetHardware();

	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//------------------------------------------

void Delay_mS(uint32_t Delay)
{
	uint64_t Counter = (uint64_t)CONTROL_TimeCounter + Delay;
	while(Counter > CONTROL_TimeCounter)
		CONTROL_WatchDogUpdate();
}
//------------------------------------------

void CONTROL_WatchDogUpdate()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------

void CONTROL_SampleBatteryVoltage()
{
	ActualBatteryVoltage = MEASURE_GetBatteryVoltage();
	DataTable[REG_ACTUAL_BAT_VOLTAGE] = ActualBatteryVoltage;
}
//------------------------------------------

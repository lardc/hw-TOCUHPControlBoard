// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "Delay.h"
#include "SysConfig.h"
#include "DataTable.h"
#include "SCCISlave.h"
#include "DeviceProfile.h"
#include "BCCITypes.h"
#include "Constraints.h"
#include "Interrupts.h"
#include "Global.h"
#include "LowLevel.h"
#include "InitConfig.h"
#include "Measurement.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();
//
// Вспомогательные состояния
typedef enum __SubState
{
	SS_None = 0,
	SS_WaitVoltage = 1,
	SS_VoltageReady = 2,
	SS_WaitTransistor = 2,
	SS_TransistorReady = 2,
} SubState;

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
static Boolean CycleActive = false;
Int16U TransistorTableMask(Int16U SetCurent);
//
volatile Int64U CONTROL_TimeCounter = 0;
//static Int64U CONTROL_TimeCounterDelay = 0;
//static Int64U CONTROL_TimeFanLastTurnOn = 0;
//static Int64U CONTROL_TimeIdleSendTOCU = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState);
void CONTROL_Idle();
void CONTROL_Init();
void CONTROL_BatteryVoltageMonitor();
void Delay_mS(uint32_t Delay);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_ResetToDefaults();
void CONTROL_WatchDogUpdate();
void CONTROL_SetVoltageCapasitor();
void CONTROL_TransistorOn(Int16U Mask);

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
	//
	CONTROL_SetDeviceState(DS_None);
	CONTROL_ResetToDefaults();
}
//------------------------------------------

void CONTROL_ResetToDefaults()
{
	DataTable[REG_FAULT_REASON] = 0;
	DataTable[REG_DISABLE_REASON] = 0;
	DataTable[REG_WARNING] = 0;
	DataTable[REG_PROBLEM] = 0;
	
	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	
	CONTROL_WatchDogUpdate();
	CONTROL_BatteryVoltageMonitor();
}
//-----------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if((CONTROL_State == DS_None) || (CONTROL_State == DS_Config) || (CONTROL_State == DS_Ready))
				{
					if(CONTROL_State == DS_None)
					{
						CONTROL_SetDeviceState(DS_Config);
						LL_LedPower(ENABLE);
						LL_PowerRelay(ENABLE);
						SUB_State = SS_WaitVoltage;
						CONTROL_BatteryVoltageMonitor();
						if(DataTable[REG_FAN_STATE])
						{
							LL_ExternalFan(ENABLE);
						}
					}
				}
				else *pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if((CONTROL_State == DS_None) || (CONTROL_State == DS_Config) || (CONTROL_State == DS_Ready))
				{
					LL_PSBoard(DISABLE);
					LL_PowerRelay(DISABLE);
					LL_CapChargeDischarge(DISCHARGE);
					LL_ExternalFan(DISABLE);
					LL_LedPower(DISABLE);
					//
					CONTROL_SetDeviceState(DS_None);
				}
				else *pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_PULSE_CONFIG:
			{
				if((CONTROL_State == DS_Ready) || (CONTROL_State == DS_Config))
				{
					CONTROL_SetDeviceState(DS_Config);
					SUB_State == SS_WaitTransistor;
					//CONTROL_TransistorOn(TransistorTableMask(DataTable[REG_CURRENT_VALUE]));
					SUB_State == SS_TransistorReady;
					
				}
				else *pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_PULSE_START:
			{
				if(CONTROL_State == DS_Ready)
				{
					CONTROL_SetDeviceState(DS_InProcess);
				}
				else *pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
				{
					CONTROL_ResetToDefaults();
					CONTROL_SetDeviceState(DS_None);
					SUB_State = SS_None;
				}
			}
			break;
			
		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = 0;
			break;

			// Блок отладочных функция
			
		case ACT_DBG_FAN:
			{
				LL_Fan(true);
				Delay_mS(1000);
				LL_Fan(false);
			}
			break;
			
		case ACT_DBG_CHARGE:
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
			
		case ACT_DBG_PSBOARD_SWITCH:
			{
				LL_PSBoardOutput(true);
				Delay_mS(1000);
				LL_PSBoardOutput(false);
			}
			break;
			
		case ACT_DBG_GATE_CONTROL:
			LL_WriteToGateRegister(DataTable[REG_DBG_GATE_DATA]);
			break;
			
		default:
			return false;
	}

	return true;
}
//-----------------------------------------------

void CONTROL_BatteryVoltageMonitor()
{
	float BatteryVoltageNow;
	
	if((CONTROL_State == DS_Ready) || (CONTROL_State == DS_Config) || (CONTROL_State == DS_InProcess))
	{
		float BatteryVoltage = MEASURE_BatteryVoltage(BAT_ADC1_CH, REG_V_BAT_OFFSET, REG_V_BAT_K);
		DataTable[REG_DBG_VSO_VALUE] = BatteryVoltage;
		
		float MaxVoltage = DataTable[REG_VOLTAGE_VALUE] + V_BAT_THRESHOLD_MAX * DataTable[REG_VOLTAGE_VALUE];
		float MinVoltage = DataTable[REG_VOLTAGE_VALUE] - V_BAT_THRESHOLD_MIN * DataTable[REG_VOLTAGE_VALUE];
		
		if((BatteryVoltageNow >= MinVoltage) && (BatteryVoltageNow <= MaxVoltage))
		{
			LL_CapChargeDischarge(CHARGE);
			LL_PSBoard(DISABLE);
			SUB_State = SS_VoltageReady;
		}
		else
		{
			if(BatteryVoltageNow < MinVoltage)
			{
				SUB_State = SS_WaitVoltage;
				LL_CapChargeDischarge(CHARGE);
				LL_PSBoard(ENABLE);
			}
			else
			{
				SUB_State = SS_WaitVoltage;
				LL_PSBoard(DISABLE);
				LL_CapChargeDischarge(DISCHARGE);
			}
		}
		
	}
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	SUB_State = SS_None;
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;
}
//-----------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//-----------------------------------------------

void Delay_mS(uint32_t Delay)
{
	uint64_t Counter = (uint64_t)CONTROL_TimeCounter + Delay;
	while(Counter > CONTROL_TimeCounter)
		CONTROL_WatchDogUpdate();
}
//-----------------------------------------------

void CONTROL_WatchDogUpdate()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//-----------------------------------------------

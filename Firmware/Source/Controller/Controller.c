//-----------------------------------------------
// Основная логика
//-----------------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "stm32f3xx.h"   // перестал видится в один прекрасный момент

#include "SysConfig.h"
#include "stdinc.h"
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
	SS_None = 0, // Неопределённое
	SS_WaitVoltage = 1, 	// Ожидание выхода на заданное напряжение
	SS_VoltageReady = 2, 	// Напряжение установлено
	SS_WaitTransistor = 2, 	// Ожидание установки транзисторов
	SS_TransistorReady = 2, // Напряжение установлено
} SubState;

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
static Boolean CycleActive = FALSE;
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
void CONTROL_Logic();
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
	EPROMServiceConfig EPROMService =
			{ (FUNC_EPROM_WriteValues) &NFLASH_WriteDT, (FUNC_EPROM_ReadValues) &NFLASH_ReadDT };
	// Инициализация data table
	DT_Init(EPROMService, FALSE);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	//
	CONTROL_SetDeviceState(DS_None);
	CONTROL_ResetToDefaults();
	//
	// Настройка TOCU
//	CUSTINT_SendTOCU(0, FALSE, FALSE, FALSE);
}
//------------------------------------------------------------------------------

void CONTROL_ResetToDefaults()
{
	DataTable[REG_FAULT_REASON] = 0;
	DataTable[REG_DISABLE_REASON] = 0;
	DataTable[REG_WARNING] = 0;
	DataTable[REG_PROBLEM] = 0;
	//
//	DataTable[REG_MEAS_CURRENT_VALUE] = 0;
//	DataTable[REG_MEAS_TIME_DELAY] = 0;
//	DataTable[REG_MEAS_TIME_ON] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------------------------------------------

void CONTROL_Idle()
{
	CONTROL_WatchDogUpdate(); // Сброс WD
	DEVPROFILE_ProcessRequests(); // Проверка сообщений по CAN и UART
	CONTROL_BatteryVoltageMonitor(); // Проверка напряжения на батареях
	CONTROL_Logic(); // Основная логика
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
					CONTROL_SetVoltageCapasitor();
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
				CONTROL_TransistorOn(TransistorTableMask(DataTable[REG_CURRENT_VALUE]));
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
		{
			DataTable[REG_WARNING] = 0;
		}
			break;

			// Debug actions
			//
		case ACT_DBG_FAN:
		{
			LL_ExternalFan(ENABLE);
			Delay_mS(1000);
			LL_ExternalFan(DISABLE);
		}
			break;

		case ACT_DBG_CHARGE:
		{
			LL_CapChargeDischarge(CHARGE);
			Delay_mS(1000);
			LL_CapChargeDischarge(DISCHARGE);
		}
			break;

		case ACT_DBG_RCK:
		{
			LL_RCK(ENABLE);
			Delay_mS(1000);
			LL_RCK(DISABLE);
		}
			break;

		case ACT_DBG_SRCK:
		{
			LL_SRCK(ENABLE);
			Delay_mS(1000);
			LL_SRCK(DISABLE);
		}
			break;

		case ACT_DBG_LEDPWR:
		{
			LL_LedPower(ENABLE);
			Delay_mS(1000);
			LL_LedPower(DISABLE);
		}
			break;

		case ACT_DBG_RELAYPWR:
		{
			LL_PowerRelay(ENABLE);
			Delay_mS(1000);
			LL_PowerRelay(DISABLE);
		}
			break;

		case ACT_DBG_PSBOARD:
		{
			LL_PSBoard(ENABLE);
			Delay_mS(1000);
			LL_PSBoard(DISABLE);
		}
			break;

		case ACT_DBG_VSO:
		{
			DataTable[REG_DBG_VSO_VALUE] = MEASURE_BatteryVoltage();
		}
			break;

		default:
		{
			return FALSE;
		}
	}
	return TRUE;
}
//-----------------------------------------------

void CONTROL_BatteryVoltageMonitor()
{
	float BatteryVoltageNow;

	if((CONTROL_State == DS_Ready) || (CONTROL_State == DS_Config) || (CONTROL_State == DS_InProcess))
	{
		BatteryVoltage = MEASURE_BatteryVoltage();
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

void CONTROL_Logic()
{
	if((CONTROL_State == DS_Config) || (CONTROL_State == DS_InProcess))
	{
		if((SUB_State == SS_VoltageReady)&&(SUB_State == SS_TransistorReady))
		{
			CONTROL_SetDeviceState(DS_Ready);
		}
	}
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
//	CUSTINT_SendTOCU(0, FALSE, FALSE, FALSE);
//	LL_RelayControl(FALSE);
//	LL_ExternalFan(FALSE);
//	LL_ExternalLED(FALSE);

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
	uint64_t Counter = (uint64_t) CONTROL_TimeCounter + Delay;
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

// Вычисление маски для включения транзисторов
Int16U TransistorTableMask(Int16U SetCurent)
{
	Int16U Mask = 0;
	Int16U Sdvig = 0;
	float TableCurent[] = { 1.5, 3, 6, 12, 24, 48, 96, 192, 348, 768, 1536 }; // A=125-2500, U=600 В
//	float TableCurent2[] = { 2.5, 5, 10, 20, 40, 80, 160, 320, 640, 1280}; 				// A=125-2500, U=1000 В
//	float TableCurent3[] = { 3.8, 3.5, 15, 30, 60, 120, 240, 480, 960, 1920, 1536 }; 	// A=125-2500, U=1500 В

//	float TableCurent4[] = { 1.5, 3, 6, 12, 24, 48, 96, 192, 348, 768, 1536 }; 			// A=5-100, U=500 В
//	float TableCurent5[] = { 1.5, 3, 6, 12, 24, 48, 96, 192, 348, 768, 1536 }; 			// A=5-100, U=1000 В
//	float TableCurent6[] = { 1.5, 3, 6, 12, 24, 48, 96, 192, 348, 768, 1536 }; 			// A=5-100, U=1500 В

	Int16U MassivSize = sizeof(TableCurent) / sizeof(float);

	if(SetCurent > DataTable[DUT_CURRENT_MAX])
	{
		SetCurent = DataTable[DUT_CURRENT_MAX];
	}
	while(Sdvig < MassivSize)
	{
		if(SetCurent == TableCurent[Sdvig])
		{
			Mask |= 1 << Sdvig;
			break;
		}
		else
		{
			if(SetCurent > TableCurent[Sdvig])
			{
				if(Sdvig == (MassivSize++))
				{
					Mask |= Mask | (1 << (Sdvig));
					SetCurent -= TableCurent[Sdvig];
					Sdvig = 0;
				}
				else
				{
					Sdvig++;
				}
			}
			else
			{
				if(Sdvig > 0)
				{
					Mask |= Mask | (1 << (Sdvig - 1));
					SetCurent -= TableCurent[Sdvig - 1];
					Sdvig = 0;
				}
				else
				{
					if(Mask == 0)
					{
						Mask = 1;
					}
					break;
				}
			}
		}
	}
	return Mask;
}
//-----------------------------------------------

void CONTROL_TransistorOn(Int16U Mask)
{
	// >8 бит, то надо разделить
//	Int8U HighByte = (Int8U) (CLEAR_BIT(Mask, BIT8 && BIT9&& BIT10));
//	Int8U LowByte = (Int8U) (Mask >> 8);

	// еще передать данные по SPI
	LL_SRCK(ENABLE);
	LL_SRCK(DISABLE);
}
//-----------------------------------------------

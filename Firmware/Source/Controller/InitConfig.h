#ifndef __INITCONFIG_H
#define __INITCONFIG_H

#include "stdinc.h"

//Functions
//
Boolean SysClk_Config();
void EI_Config();
void IO_Config();
void CAN_Config();
void UART_Config();
void ADC_Init();
void Timer7_Config();
void WatchDog_Config();

#endif //__INITCONFIG_H

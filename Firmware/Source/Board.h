#ifndef __BOARD_H
#define __BOARD_H

#include "Delay.h"
#include "stm32f30x.h"

#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwTIM.h"
#include "ZwDAC.h"
#include "ZwDMA.h"
#include "ZwADC.h"
#include "ZwEXTI.h"
#include "ZwSCI.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"

// Определения для выходных портов
static const GPIO_PS GPIO_RCK			= {GPIOA, Pin_4};
static const GPIO_PS GPIO_SRCK			= {GPIOA, Pin_5};
static const GPIO_PS GPIO_DATA			= {GPIOA, Pin_7};
static const GPIO_PS GPIO_FAN			= {GPIOB, Pin_0};
static const GPIO_PS GPIO_LED			= {GPIOB, Pin_4};
static const GPIO_PS GPIO_LED_EXT		= {GPIOB, Pin_1};
static const GPIO_PS GPIO_BAT_CHARGE	= {GPIOB, Pin_2};
static const GPIO_PS GPIO_HVPS_CTRL		= {GPIOB, Pin_10};
static const GPIO_PS GPIO_MW_CTRL		= {GPIOB, Pin_11};

// Определения для портов альтернативных функций
static const GPIO_PS GPIO_ALT_CAN_RX	= {GPIOA, Pin_11};
static const GPIO_PS GPIO_ALT_CAN_TX	= {GPIOA, Pin_12};
static const GPIO_PS GPIO_ALT_UART_RX	= {GPIOA, Pin_10};
static const GPIO_PS GPIO_ALT_UART_TX	= {GPIOA, Pin_9};

#endif // __BOARD_H

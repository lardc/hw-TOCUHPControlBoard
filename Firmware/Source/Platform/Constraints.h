// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
//
#define DUT_CURRENT_MIN			125		// â À
#define DUT_CURRENT_MAX			2500	// â À
#define DUT_CURRENT_DEF			125		// â À

#define DUT_VOLTAGE_MIN			500		// â Â
#define DUT_VOLTAGE_MAX			1500	// â Â
#define DUT_VOLTAGE_DEF			500		// â Â

////
//#define CURRENT_SHUNT_MIN		10		// â ìÎì õ1000
//#define CURRENT_SHUNT_MAX		10000	// â ìÎì õ1000
//#define CURRENT_SHUNT_DEF		1000	// â ìÎì õ1000
////
//#define GAIN_COEFFICIENT_MIN	1
//#define GAIN_COEFFICIENT_MAX	10000
////
//#define GAIN_CURRENT_DEF		206
//#define GAIN_VOLTAGE_DEF		1100


// Types
//
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;


// Variables
//
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];


#endif // __CONSTRAINTS_H

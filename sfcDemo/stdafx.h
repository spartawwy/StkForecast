// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <cassert>
// only choice one in follow

//#define TO_GET_HISDATA_F2F  1
//#define GET_DATA_DIRECT_USE_TUSHARE  1
//#define TRY_GET_STK_DATA  1
//#define  TEST_TSL_HQAPI  1
#define  LITTLE_TEST

//-----------------------------
 __int64 Time2Val(int y, int m, int d, int hour, int min, int sec);

#ifndef __DEFINES_H
#define __DEFINES_H
#include "AP_HAL_Mock.h"

extern AP_HAL_Mock mockHal;

#define MOCK_HAL mockHal
#define private public
#define protected public

#endif


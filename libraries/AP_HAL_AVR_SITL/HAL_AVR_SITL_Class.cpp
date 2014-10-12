/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_AVR_SITL

#include <AP_HAL_AVR.h>
#include <AP_HAL_AVR_private.h>
#include <AP_HAL_AVR_SITL.h>
#include "AP_HAL_AVR_SITL_Namespace.h"
#include "HAL_AVR_SITL_Class.h"
#include "Scheduler.h"
#include "AnalogIn.h"
#include "UARTDriver.h"
#include "Storage.h"
#include "RCInput.h"
#include "RCOutput.h"
#include "SITL_State.h"
#include "Util.h"

#include <AP_HAL_Empty.h>
#include <AP_HAL_Empty_Private.h>
#include <stdlib.h>
#include <stdio.h>

using namespace AVR_SITL;

static SITLScheduler sitlScheduler;
static SITLEEPROMStorage sitlEEPROMStorage;
static SITLRCInput  sitlRCInput;
static SITLRCOutput sitlRCOutput;
static SITLAnalogIn sitlAnalogIn;

// use the Empty HAL for hardware we don't emulate
static Empty::EmptyGPIO emptyGPIO;
static Empty::EmptySemaphore emptyI2Csemaphore;
static Empty::EmptyI2CDriver emptyI2C(&emptyI2Csemaphore);
static Empty::EmptySPIDeviceManager emptySPI;

SITLUARTDriver sitlUart0Driver(0);
static SITLUARTDriver sitlUart1Driver(1);
static SITLUARTDriver sitlUart2Driver(2);
static SITLUARTDriver sitlUart3Driver(3);
static SITLUARTDriver sitlUart4Driver(4);

static SITLUtil utilInstance;

HAL_AVR_SITL::HAL_AVR_SITL() :
    AP_HAL::HAL(
	    &sitlUart0Driver,  /* uartA */
        &sitlUart1Driver,  /* uartB */
        &sitlUart2Driver,  /* uartC */
        &sitlUart3Driver,  /* uartD */
        &sitlUart4Driver,  /* uartE */
        &emptyI2C, /* i2c */
        &emptySPI, /* spi */
        &sitlAnalogIn, /* analogin */
        &sitlEEPROMStorage, /* storage */
        &sitlUart0Driver, /* console */
        &emptyGPIO, /* gpio */
        &sitlRCInput,  /* rcinput */
        &sitlRCOutput, /* rcoutput */
        &sitlScheduler, /* scheduler */
        &utilInstance) /* util */
{}

void HAL_AVR_SITL::init(int argc, char * const argv[]) const 
{
	fprintf(stdout, "HAL_AVR_SITL::init\n" );
    AVR_SITL::SITL_State::init(argc, argv);
    scheduler->init(NULL);
    uartA->begin(115200);

    rcin->init(NULL);
    rcout->init(NULL);

    //spi->init(NULL);
    //i2c->begin();
    //i2c->setTimeout(100);
    analogin->init(NULL);
}

const HAL_AVR_SITL AP_HAL_AVR_SITL;

#endif // CONFIG_HAL_BOARD == HAL_BOARD_AVR_SITL

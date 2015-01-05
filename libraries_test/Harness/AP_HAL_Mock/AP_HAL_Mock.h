#include "AP_HAL.h"
#include "UARTDriverMock.h"
#include "I2CDriverMock.h"
#include "SPIDeviceManagerMock.h"
#include "SPIDriverMock.h"
#include "AnalogInMock.h"
#include "AnalogSourceMock.h"
#include "DigitalSourceMock.h"
#include "GPIOMock.h"
#include "StorageMock.h"
#include "RCInputMock.h"
#include "RCOutputMock.h"
#include "SchedulerMock.h"
#include "UtilMock.h"

class AP_HAL_Mock : public AP_HAL::HAL
{
public:
    AP_HAL_Mock( ) : AP_HAL::HAL( &a,
                                  &b,
                                  &c,
                                  &d,
                                  &e,
                                  &i2c,
                                  &spiDeviceManager,
                                  &analogIn,
                                  &storage,
                                  &console,
                                  &gpio,
                                  &rcInput,
                                  &rcOutput,
                                  &scheduler,
                                  &util ) {}
                                  
    MOCK_CONST_METHOD2( init, void (int argc, char * const argv[]) );

public:
    UARTDriverMock a;
    UARTDriverMock b;
    UARTDriverMock c;
    UARTDriverMock d;
    UARTDriverMock e;
    I2CDriverMock i2c;
    SPIDeviceManagerMock spiDeviceManager;
    AnalogInMock analogIn;
    StorageMock storage;
    UARTDriverMock console;
    GPIOMock gpio;
    RCInputMock rcInput;
    RCOutputMock rcOutput;
    SchedulerMock scheduler;
    UtilMock util;    
};
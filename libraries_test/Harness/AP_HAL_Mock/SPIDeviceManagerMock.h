#ifndef __SPIDeviceManagerMock_H
#define __SPIDeviceManagerMock_H

#include <gmock/gmock.h>
#include "SPIDriver.h"

class SPIDeviceManagerMock : public AP_HAL::SPIDeviceManager
{
public:
    MOCK_METHOD1(init, void(void*));
    MOCK_METHOD1(device, AP_HAL::SPIDeviceDriver*(enum AP_HAL::SPIDevice));
};

#endif


#ifndef __SPIDeviceDriverMock_H
#define __SPIDeviceDriverMock_H

#include <gmock/gmock.h>
#include "SPIDriver.h"

class SPIDeviceDriverMock : public AP_HAL::SPIDeviceDriver
{
public:
    MOCK_METHOD0(init, void() );
    MOCK_METHOD0(get_semaphore, AP_HAL::Semaphore*());
    MOCK_METHOD3(transaction, void(const uint8_t *tx, uint8_t *rx, uint16_t len));
    MOCK_METHOD0(cs_assert, void());
    MOCK_METHOD0(cs_release, void());
    MOCK_METHOD1(transfer, uint8_t(uint8_t data));
    MOCK_METHOD2(transfer, void(const uint8_t *data, uint16_t len));
    MOCK_METHOD1(set_bus_speed, void(enum bus_speed speed));
};

#endif


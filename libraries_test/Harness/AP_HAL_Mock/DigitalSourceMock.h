#ifndef __DigitalSourceMock_H
#define __DigitalSourceMock_H

#include <gmock/gmock.h>
#include "GPIO.h"

class DigitalSourceMock : public AP_HAL::DigitalSource
{
public:
    MOCK_METHOD1(mode, void(uint8_t output));
    MOCK_METHOD0(read, uint8_t());
    MOCK_METHOD1(write, void(uint8_t value));
    MOCK_METHOD0(toggle, void());
};

#endif


#ifndef __AnalogInMock_H
#define __AnalogInMock_H

#include <gmock/gmock.h>
#include "AnalogIn.h"

class AnalogInMock : public AP_HAL::AnalogIn
{
public:
    MOCK_METHOD1(init, void(void* implspecific));
    MOCK_METHOD1(channel, AP_HAL::AnalogSource*(int16_t n));
    MOCK_METHOD0(board_voltage, float(void));
    MOCK_METHOD0(servorail_voltage, float(void));
    MOCK_METHOD0(power_status_flags, uint16_t(void));
};

#endif


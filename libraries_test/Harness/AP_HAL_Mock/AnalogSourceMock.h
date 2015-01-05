#ifndef __AnalogSourceMock_H
#define __AnalogSourceMock_H

#include <gmock/gmock.h>
#include "AnalogIn.h"

class AnalogSourceMock : public AP_HAL::AnalogSource
{
public:
    MOCK_METHOD0(read_average, float());
    MOCK_METHOD0(read_latest, float());
    MOCK_METHOD1(set_pin, void(uint8_t p));
    MOCK_METHOD1(set_stop_pin, void(uint8_t p));
    MOCK_METHOD1(set_settle_time, void(uint16_t settle_time_ms));
    MOCK_METHOD0(voltage_average, float());
    MOCK_METHOD0(voltage_latest, float());
    MOCK_METHOD0(voltage_average_ratiometric, float());
};

#endif


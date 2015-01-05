#ifndef __RCOutputMock_H
#define __RCOutputMock_H

#include <gmock/gmock.h>
#include "RCOutput.h"

class RCOutputMock : public AP_HAL::RCOutput
{
public:
    MOCK_METHOD1(init, void(void* implspecific));
    MOCK_METHOD2(set_freq, void(uint32_t chmask, uint16_t freq_hz));
    MOCK_METHOD1(get_freq, uint16_t(uint8_t ch));
    MOCK_METHOD1(enable_ch, void(uint8_t ch));
    MOCK_METHOD1(disable_ch, void(uint8_t ch));
    MOCK_METHOD2(write, void(uint8_t ch, uint16_t period_us));
    MOCK_METHOD3(write, void(uint8_t ch, uint16_t* period_us, uint8_t len));
    MOCK_METHOD1(read, uint16_t(uint8_t ch));
    MOCK_METHOD2(read, void(uint16_t* period_us, uint8_t len));
    MOCK_METHOD2(set_safety_pwm, void(uint32_t chmask, uint16_t period_us));
    MOCK_METHOD2(set_failsafe_pwm, void(uint32_t chmask, uint16_t period_us));
    MOCK_METHOD0(force_safety_off, void(void));
};

#endif

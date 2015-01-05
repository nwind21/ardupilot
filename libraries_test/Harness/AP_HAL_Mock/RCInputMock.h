#ifndef __RCInputMock_H
#define __RCInputMock_H

#include <gmock/gmock.h>
#include "RCInput.h"

class RCInputMock : public AP_HAL::RCInput
{
public:
    MOCK_METHOD1(init, void(void* implspecific));
    MOCK_METHOD0(new_input, bool());
    MOCK_METHOD0(num_channels, uint8_t());
    MOCK_METHOD1(read, uint16_t(uint8_t ch));
    MOCK_METHOD2(read, uint8_t(uint16_t* periods, uint8_t len));
    MOCK_METHOD2(set_overrides, bool(int16_t *overrides, uint8_t len));
    MOCK_METHOD2(set_override, bool(uint8_t channel, int16_t override));
    MOCK_METHOD0(clear_overrides, void());
};

#endif

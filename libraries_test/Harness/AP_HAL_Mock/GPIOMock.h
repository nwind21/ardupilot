#ifndef __GPIOMock_H
#define __GPIOMock_H

#include <gmock/gmock.h>
#include "GPIO.h"

class GPIOMock : public AP_HAL::GPIO
{
public:
    MOCK_METHOD0(init, void());
    MOCK_METHOD2(pinMode, void(uint8_t pin, uint8_t output));
    MOCK_METHOD1(read, uint8_t(uint8_t pin));
    MOCK_METHOD2(write, void(uint8_t pin, uint8_t value));
    MOCK_METHOD1(toggle, void(uint8_t pin));
    MOCK_METHOD1(analogPinToDigitalPin, int8_t(uint8_t pin));
    MOCK_METHOD1(channel, AP_HAL::DigitalSource*(uint16_t n));
    MOCK_METHOD3(attach_interrupt, bool(uint8_t interrupt_num, AP_HAL::Proc p, uint8_t mode));
    MOCK_METHOD0(usb_connected, bool(void));
};

#endif



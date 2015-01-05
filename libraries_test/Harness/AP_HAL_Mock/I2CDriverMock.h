#ifndef __I2CDriverMock_H
#define __I2CDriverMock_H

#include <gmock/gmock.h>
#include "I2CDriver.h"

class I2CDriverMock : public AP_HAL::I2CDriver
{
public:
    MOCK_METHOD0(begin,void());
    MOCK_METHOD0(end, void());
    MOCK_METHOD1(setTimeout, void(uint16_t ms));
    MOCK_METHOD1(setHighSpeed, void(bool active));
    MOCK_METHOD3(write,uint8_t (uint8_t addr, uint8_t len, uint8_t* data));
    MOCK_METHOD3(writeRegister, uint8_t(uint8_t addr, uint8_t reg, uint8_t val));
    MOCK_METHOD4(writeRegisters, uint8_t(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data));
    MOCK_METHOD3(read, uint8_t(uint8_t addr, uint8_t len, uint8_t* data));
    MOCK_METHOD3(readRegister, uint8_t(uint8_t addr, uint8_t reg, uint8_t* data));
    MOCK_METHOD4(readRegisters, uint8_t(uint8_t addr, uint8_t reg, uint8_t len, uint8_t* data));
    MOCK_METHOD0(lockup_count, uint8_t());
    MOCK_METHOD0(get_semaphore, AP_HAL::Semaphore*() );
};

#endif
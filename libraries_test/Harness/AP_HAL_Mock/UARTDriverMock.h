#ifndef __UARTDriverMock_H
#define __UARTDriverMock_H

#include <gmock/gmock.h>
#include "UARTDriver.h"

class UARTDriverMock : public AP_HAL::UARTDriver
{
public:
    MOCK_METHOD1( begin, void(uint32_t baud));
    MOCK_METHOD3( begin, void(uint32_t baud, uint16_t rxSpace, uint16_t txSpace) );
    MOCK_METHOD0( end, void() );
    MOCK_METHOD0( flush, void() );
    MOCK_METHOD0( is_initialized, bool() );
    MOCK_METHOD1( set_blocking_writes, void(bool blocking) );
    MOCK_METHOD0( tx_pending, bool() );

    MOCK_METHOD1( set_flow_control, void( enum flow_control flow_control_setting) );
    MOCK_METHOD0( get_flow_control, enum flow_control() );
    
    // AP_HAL::BetterStream
    void print_P(const prog_char_t *s) {}
    void println_P(const prog_char_t *s) {}
    void printf(const char *s, ...)
            __attribute__ ((format(__printf__, 2, 3))) {}
    void _printf_P(const prog_char *s, ...)
            __attribute__ ((format(__printf__, 2, 3))) {}

    void vprintf(const char *s, va_list ap) {}
    void vprintf_P(const prog_char *s, va_list ap) {}
    
    // AP_HAL::Stream
    virtual int16_t available() { return 1; };
    virtual int16_t txspace() { return 1; };
    virtual int16_t read() { return 1; };
    
    // AP_HAL::Print
    virtual size_t write(uint8_t) { return 1;};
    virtual size_t write(const uint8_t *buffer, size_t size) { return size; };
};

#endif
#ifndef __StorageMock_H
#define __StorageMock_H

#include <gmock/gmock.h>
#include "Storage.h"

class StorageMock : public AP_HAL::Storage
{
public:
    MOCK_METHOD1(init, void(void*));
    MOCK_METHOD1(read_byte, uint8_t(uint16_t loc));
    MOCK_METHOD1(read_word, uint16_t(uint16_t loc));
    MOCK_METHOD1(read_dword, uint32_t(uint16_t loc));
    MOCK_METHOD3(read_block, void(void *dst, uint16_t src, size_t n));
    MOCK_METHOD2(write_byte, void(uint16_t loc, uint8_t value));
    MOCK_METHOD2(write_word, void(uint16_t loc, uint16_t value));
    MOCK_METHOD2(write_dword, void(uint16_t loc, uint32_t value));
    MOCK_METHOD3(write_block, void(uint16_t dst, const void* src, size_t n));
};

#endif


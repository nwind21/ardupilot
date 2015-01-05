#ifndef __UtilMock_H
#define __UtilMock_H

#include <gmock/gmock.h>
#include "Util.h"

class UtilMock : public AP_HAL::Util
{
public:
    MOCK_METHOD1(run_debug_shell, bool(AP_HAL::BetterStream *stream));
    MOCK_METHOD0(safety_switch_state, enum safety_state(void));
    MOCK_METHOD1(set_system_clock, void(uint64_t time_utc_usec));
    MOCK_METHOD1(get_system_id, bool(char buf[40]));
    MOCK_METHOD0(available_memory, uint16_t(void));
    MOCK_METHOD2(commandline_arguments, void(uint8_t &argc, char * const *&argv));
};

#endif

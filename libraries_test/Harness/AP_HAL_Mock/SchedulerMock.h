#ifndef __SchedulerMock_H
#define __SchedulerMock_H

#include <gmock/gmock.h>
#include "Scheduler.h"

class SchedulerMock : public AP_HAL::Scheduler 
{
public:
    MOCK_METHOD1(init, void(void* implspecific));
    MOCK_METHOD1(delay, void(uint16_t ms));
    MOCK_METHOD0(millis, uint32_t());
    MOCK_METHOD0(micros, uint32_t());
    MOCK_METHOD1(delay_microseconds, void(uint16_t us));
    MOCK_METHOD2(register_delay_callback, void(AP_HAL::Proc, uint16_t min_time_ms));
    MOCK_METHOD1(register_timer_process, void(AP_HAL::MemberProc));
    MOCK_METHOD1(register_io_process, void(AP_HAL::MemberProc));
    MOCK_METHOD0(suspend_timer_procs, void());
    MOCK_METHOD0(resume_timer_procs, void());
    MOCK_METHOD0(in_timerprocess, bool());
    MOCK_METHOD2(register_timer_failsafe, void(AP_HAL::Proc, uint32_t period_us));
    MOCK_METHOD0(system_initializing, bool());
    MOCK_METHOD0(system_initialized, void());
    MOCK_METHOD1(panic, void(const prog_char_t *errormsg));
    MOCK_METHOD1(reboot, void(bool hold_in_bootloader));
    MOCK_METHOD1(set_timer_speed, void(uint16_t speed_hz));
    MOCK_METHOD1(stop_clock, void(uint64_t time_usec));
};

#endif


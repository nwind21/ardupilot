
#ifndef __AP_HAL_AVR_SITL_MAIN_H__
#define __AP_HAL_AVR_SITL_MAIN_H__

#include <stdlib.h>
#include <signal.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_AVR_SITL

#define AP_HAL_MAIN() extern "C" {\
    struct sigaction oldIntAction; \
    struct sigaction oldTermAction; \
    void _signal_handler (int signal_value) \
    { \
        fprintf(stderr, "Caught SIGTERM/SIGINT sending kill...\n"); \
        AVR_SITL::SITL_State::cleanup();\
        fprintf(stderr, "Restoring action...\n"); \
        sigaction (SIGINT, &oldIntAction, NULL); \
        sigaction (SIGTERM, &oldTermAction, NULL); \
        raise( signal_value ); \
    }\
    int main (int argc, char * const argv[]) {	\
        struct sigaction action; \
        action.sa_handler = _signal_handler; \
        action.sa_flags = 0; \
        sigemptyset (&action.sa_mask); \
        sigaction (SIGINT, &action, &oldIntAction); \
        sigaction (SIGTERM, &action, &oldTermAction); \
        fprintf( stdout, "AP_HAL_MAIN -> hal.init\n"); \
        hal.init(argc, argv); \
        setup(); \
        hal.scheduler->system_initialized(); \
        fprintf( stdout, "AP_HAL_MAIN -> hal.init -> main loop\n"); \
        while( 1 ) { \
            loop(); \
            AVR_SITL::SITL_State::loop_hook(); \
        } \
        return 0;\
    }\
}
#endif


#endif // __AP_HAL_AVR_SITL_MAIN_H__

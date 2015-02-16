/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_AVR_SITL

#include <AP_HAL_AVR.h>
#include <AP_HAL_AVR_SITL.h>
#include "AP_HAL_AVR_SITL_Namespace.h"
#include "HAL_AVR_SITL_Class.h"
#include "UARTDriver.h"
#include "Scheduler.h"
#include <pthread.h>
#include <zmq.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>

#include <AP_Param.h>

extern AVR_SITL::SITLUARTDriver sitlUart0Driver;
    
#ifdef __CYGWIN__
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void print_trace() {
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    int child_pid = fork();
    if (!child_pid) {
        dup2(2,1); // redirect output to stderr
        fprintf(stdout,"stack trace for %s pid=%s\n",name_buf,pid_buf);
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
    } else {
        waitpid(child_pid,NULL,0);
    }
}
#endif

extern const AP_HAL::HAL& hal;

using namespace AVR_SITL;

enum SITL_State::vehicle_type SITL_State::_vehicle;
uint16_t SITL_State::_framerate;
uint16_t SITL_State::_base_port = 5760;
uint16_t SITL_State::_rcout_port = 5502;
uint16_t SITL_State::_rctx_port = 5501;
uint16_t SITL_State::_fdm_port = 5503;
//struct sockaddr_in SITL_State::_rcout_addr;
pid_t SITL_State::_parent_pid;
uint32_t SITL_State::_update_count;
bool SITL_State::_motors_on;
uint16_t SITL_State::sonar_pin_value;
uint16_t SITL_State::airspeed_pin_value;
uint16_t SITL_State::voltage_pin_value;
uint16_t SITL_State::current_pin_value;
float SITL_State::_initial_height;
float SITL_State::_current;

AP_Baro_HIL *SITL_State::_barometer;
AP_InertialSensor_HIL *SITL_State::_ins;
SITLScheduler *SITL_State::_scheduler;
AP_Compass_HIL *SITL_State::_compass;

int SITL_State::_rctx_fd; // MAVLink RC TX in
void* SITL_State::_zmq_context;
void* SITL_State::_sitl_zmq_fdm_publisher;
void* SITL_State::_sitl_zmq_fdm_kill;
void* SITL_State::_sitl_zmq_fdm_sync;
pthread_t SITL_State::_fdmUpdateProcessCtx;
SITL_State::threadCtx SITL_State::gpsThreadCtx;
SITL_State::threadCtx SITL_State::insThreadCtx;
SITL_State::threadCtx SITL_State::compassThreadCtx;
SITL_State::threadCtx SITL_State::barometerThreadCtx;

SITL *SITL_State::_sitl;

uint16_t SITL_State::pwm_output[11];
uint16_t SITL_State::last_pwm_output[11];
uint16_t SITL_State::pwm_input[8];
bool SITL_State::new_rc_input;

// catch floating point exceptions
void SITL_State::_sig_fpe(int signum)
{
	fprintf(stderr, "ERROR: Floating point exception\n");
	exit(1);
}

void SITL_State::_usage(void)
{
	fprintf(stdout, "Options:\n");
	fprintf(stdout, "\t-w          wipe eeprom and dataflash\n");
	fprintf(stdout, "\t-r RATE     set SITL framerate\n");
	fprintf(stdout, "\t-H HEIGHT   initial barometric height\n");
	fprintf(stdout, "\t-C          use console instead of TCP ports\n");
	fprintf(stdout, "\t-I          set instance of SITL (adds 10*instance to all port numbers)\n");
}

void SITL_State::_parse_command_line(int argc, char * const argv[])
{
	int opt;

	signal(SIGFPE, _sig_fpe);
	// No-op SIGPIPE handler
	signal(SIGPIPE, SIG_IGN);

    setvbuf(stdout, (char *)0, _IONBF, 0);
    setvbuf(stderr, (char *)0, _IONBF, 0);

//    fprintf(stdout, "SITL_State::_parse_command_line\n" );
	while ((opt = getopt(argc, argv, "swhr:H:CI:P:")) != -1) {
		switch (opt) {
		case 'w':
			AP_Param::erase_all();
			unlink("dataflash.bin");
			break;
		case 'r':
			_framerate = (unsigned)atoi(optarg);
			break;
		case 'H':
			_initial_height = atof(optarg);
			break;
		case 'C':
			AVR_SITL::SITLUARTDriver::_console = true;
			break;
		case 'I': {
            uint8_t instance = atoi(optarg);
            _base_port  += instance * 10;
            _rcout_port += instance * 10;
            _rctx_port += instance * 10;
        }
			break;
		case 'P':
            _set_param_default(optarg);
			break;
		default:
			_usage();
			exit(1);
		}
	}

	fprintf(stdout, "SITL_State::_parse_command_line  => Starting sketch '%s'\n", SKETCH);

	if (strcmp(SKETCH, "ArduCopter") == 0) {
		_vehicle = ArduCopter;
		if (_framerate == 0) {
			_framerate = 200;
		}
	} else {
		_vehicle = ArduPlane;
		if (_framerate == 0) {
			_framerate = 50;
		}
	}
    fprintf(stdout, "SITL_State::_parse_command_line  => Rate set at '%d'\n", _framerate);
	_sitl_setup();
}


void SITL_State::_set_param_default(char *parm)
{
    char *p = strchr(parm, '=');
    if (p == NULL) {
        printf("Please specify parameter as NAME=VALUE");
        exit(1);
    }
    float value = atof(p+1);
    *p = 0;
    enum ap_var_type var_type;
    AP_Param *vp = AP_Param::find(parm, &var_type);
    if (vp == NULL) {
        printf("Unknown parameter %s\n", parm);
        exit(1);
    }
    if (var_type == AP_PARAM_FLOAT) {
        ((AP_Float *)vp)->set_and_save(value);
    } else if (var_type == AP_PARAM_INT32) {
        ((AP_Int32 *)vp)->set_and_save(value);
    } else if (var_type == AP_PARAM_INT16) {
        ((AP_Int16 *)vp)->set_and_save(value);
    } else if (var_type == AP_PARAM_INT8) {
        ((AP_Int8 *)vp)->set_and_save(value);
    } else {
        printf("Unable to set parameter %s\n", parm);
        exit(1);
    }
    printf("Set parameter %s to %f\n", parm, value);
}


/*
  setup for SITL handling
 */
void SITL_State::_sitl_setup(void)
{
#ifndef __CYGWIN__
	_parent_pid = getppid();
#endif
//	_rcout_addr.sin_family = AF_INET;
//	_rcout_addr.sin_port = htons(_rcout_port);
//	inet_pton(AF_INET, "127.0.0.1", &_rcout_addr.sin_addr);

	// find the barometer object if it exists
	_sitl = (SITL *)AP_Param::find_object("SIM_");
	_barometer = (AP_Baro_HIL *)AP_Param::find_object("GND_");
	_ins = (AP_InertialSensor_HIL *)AP_Param::find_object("INS_");
	_compass = (AP_Compass_HIL *)AP_Param::find_object("COMPASS_");

    if (_sitl != NULL) {
        fprintf(stderr, "SITL_State::_sitl_setup => Setting SITL initial values, height: %f\n", _initial_height );
        // setup some initial values
#ifndef HIL_MODE
        _update_barometer(_initial_height);
        _update_ins(0, 0, 0, 0, 0, 0, 0, 0, -9.8, 0, _initial_height);
        _update_compass(0, 0, 0);
        _update_gps(0, 0, 0, 0, 0, 0, false);
#endif
    }
    _zmq_context = zmq_ctx_new ();

    _setup_zmq_fdm();

    /////////////////////////////////////
    // Start subscribers
    int rc = 0;
    gpsThreadCtx.zmqContext = _zmq_context;
    gpsThreadCtx.func = _fdmGPSUpdate;
    gpsThreadCtx.id = 1;
    rc = pthread_create( &_fdmUpdateProcessCtx,
                         NULL,
                         SITL_State::_fdmSubscriber,
                        &gpsThreadCtx );
    if ( rc != 0 )
    {
        fprintf( stderr,
                "SITL_State::_sitl_setup => Failed to start GPS thread\n" );
        exit(1);
    }

    insThreadCtx.zmqContext = _zmq_context;
    insThreadCtx.func = _fdmInsUpdate;
    insThreadCtx.id = 2;
    rc = pthread_create( &_fdmUpdateProcessCtx,
                         NULL,
                         SITL_State::_fdmSubscriber,
                         &insThreadCtx );
    if ( rc != 0 )
    {
        fprintf( stderr,
                "SITL_State::_sitl_setup => Failed to start INS thread\n" );
        exit(1);
    }

    compassThreadCtx.zmqContext = _zmq_context;
    compassThreadCtx.func = _fdmCompassUpdate;
    compassThreadCtx.id = 3;
    rc = pthread_create( &_fdmUpdateProcessCtx,
                         NULL,
                         SITL_State::_fdmSubscriber,
                         &compassThreadCtx );
    if ( rc != 0 )
    {
        fprintf( stderr,
                "SITL_State::_sitl_setup => Failed to start Compass thread\n" );
        exit(1);
    }

    barometerThreadCtx.zmqContext = _zmq_context;
    barometerThreadCtx.func = _fdmBarometerUpdate;
    barometerThreadCtx.id = 4;
    rc = pthread_create( &_fdmUpdateProcessCtx,
                         NULL,
                         SITL_State::_fdmSubscriber,
                         &barometerThreadCtx );
    if ( rc != 0 )
    {
        fprintf( stderr,
                "SITL_State::_sitl_setup => Failed to start Barometer thread\n" );
        exit(1);
    }


    // Wait for all threads to start
    {
        uint8_t threadCnt = 0;
        uint8_t byte = 0;
        while ( threadCnt < 4 )
        {
            zmq_recv( _sitl_zmq_fdm_sync,
                      &byte,
                      sizeof(byte),
                      0 );
            zmq_send( _sitl_zmq_fdm_sync,
                      (void*)&byte,
                      sizeof(byte),
                      0 );
            threadCnt++;
        }
        fprintf( stderr,
                 "SITL_State::_sitl_setup => All threads have reported in\n" );
    }

    // Let's idle a little since the cleanup can happen pretty quick
    uint32_t now = hal.scheduler->micros();
    while ( hal.scheduler->micros() - now < 1000000 );

	_setup_timer();
    _setup_mavlink_rctx();
    fprintf( stderr,
             "SITL_State::_sitl_setup => complete\n" );
}

///////////////////////////////////////////////////////////////////////
// THREAD UPDATE FUNCTIONs
//     These are used
void SITL_State::_fdmGPSUpdate( sitl_fdm& fdm,
                                SITL* sitl )
{
    // Update
    _update_gps( fdm.latitude,
                 fdm.longitude,
                 fdm.altitude,
                 fdm.speedN,
                 fdm.speedE,
                 fdm.speedD,
                 !sitl->gps_disable );
}

void SITL_State::_fdmInsUpdate( sitl_fdm& fdm,
                                SITL* sitl )
{
    _update_ins(fdm.rollDeg,
                fdm.pitchDeg,
                fdm.yawDeg,
                fdm.rollRate,
                fdm.pitchRate,
                fdm.yawRate,
                fdm.xAccel,
                fdm.yAccel,
                fdm.zAccel,
                fdm.airspeed,
                fdm.altitude );
}

void SITL_State::_fdmBarometerUpdate( sitl_fdm& fdm,
                                      SITL* sitl )
{
    _update_barometer( fdm.altitude );
}

void SITL_State::_fdmCompassUpdate( sitl_fdm& fdm,
                                    SITL* sitl )
{
    _update_compass( fdm.rollDeg,
                     fdm.pitchDeg,
                     fdm.yawDeg );
}

#define ZMQ_CHECK_RETVAL( operation, msg ) \
if ( operation == -1 ) \
{ \
        fprintf( stdout, msg ); \
        return NULL; \
} \

void* SITL_State::_fdmSubscriber( void* arg )
{

    sitl_fdm fdm;
    threadCtx *ctx = (threadCtx*)arg;

//    fprintf( stderr,
  //           "SITL_State::_fdmSubscriber starting. %d\n", ctx->id );

    void* _fdm_subscriber = zmq_socket( ctx->zmqContext,
                                        ZMQ_SUB );
    void* _fdm_kill_subscriber = zmq_socket( ctx->zmqContext,
                                             ZMQ_SUB );
    void* _fdm_sync = zmq_socket( ctx->zmqContext,
                                  ZMQ_REQ );
    zmq_pollitem_t items[] =
    {
        { _fdm_subscriber, 0, ZMQ_POLLIN, 0 },
        { _fdm_kill_subscriber, 0, ZMQ_POLLIN, 0 }
    };

    // Connect all the ZMQ sockets
    ZMQ_CHECK_RETVAL( zmq_connect( _fdm_subscriber,
                                   "tcp://localhost:5503" ),
                      "SITL_State::_fdmSubscriber failed to connect to subscribe socket.\n" );

    ZMQ_CHECK_RETVAL( zmq_connect( _fdm_kill_subscriber,
                                   "inproc://kill" ),
                      "SITL_State::_fdmSubscriber failed to connect to kill socket.\n" );

    ZMQ_CHECK_RETVAL( zmq_connect( _fdm_sync,
                                   "inproc://sync" ),
                      "SITL_State::_fdmSubscriber failed to connect to sync socket.\n" );


    // Set sock options for subscriber ports
    ZMQ_CHECK_RETVAL( zmq_setsockopt ( _fdm_kill_subscriber,
                                       ZMQ_SUBSCRIBE,
                                       NULL,
                                       0 ),
                      "SITL_State::_fdmSubscriber failed to kill subscribe.\n" );

    ZMQ_CHECK_RETVAL( zmq_setsockopt ( _fdm_subscriber,
                                       ZMQ_SUBSCRIBE,
                                       NULL,
                                       0 ),
                      "SITL_State::_fdmSubscriber failed to fdm subscribe.\n" );

    uint8_t byte;
    zmq_send ( _fdm_sync,
               (void*)&byte,
               sizeof(byte),
               0 );
    zmq_recv( _fdm_sync,
              &byte,
              sizeof(byte),
              0 );


    // Enter the main loop, the function that actually processes the FDM packet is
    // a function pointer.
    fprintf( stderr,
             "[ %d ] UP!\n", ctx->id );
    uint32_t cnt = 0;
    while( 1 )
    {
        // Block till data arrives
        int rc = zmq_poll( items,
                           2,
                           -1 );
        (void)rc;

        if ( items[0].revents & ZMQ_POLLIN )
        {
            service_fdm( _fdm_subscriber,
                         fdm );
            (*ctx->func)( fdm,
                          _sitl );
            cnt++;
        }

        if ( items[1].revents & ZMQ_POLLIN )
        {
            uint8_t killByte;
            zmq_recv( _fdm_kill_subscriber,
                      &killByte,
                      sizeof(killByte),
                      0 );
            break;
        }
    }

    // Send to the waiting process that thread has terminated successfully.
    {
        uint8_t syncByte = (uint8_t)(ctx->id & 0xFF);
        zmq_send ( _fdm_sync,
                   (void*)&syncByte,
                   sizeof(syncByte),
                   0 );
        zmq_recv( _fdm_sync,
                  &syncByte,
                  sizeof(syncByte),
                  0 );
    }

    int linger = 1;
    int rc = 0;
    rc = zmq_setsockopt ( _fdm_subscriber,
                          ZMQ_LINGER,
                          &linger,
                          sizeof(linger) );
    rc = zmq_setsockopt ( _fdm_kill_subscriber,
                          ZMQ_LINGER,
                          &linger,
                          sizeof(linger) );
    rc = zmq_setsockopt ( _fdm_sync,
                          ZMQ_LINGER,
                          &linger,
                          sizeof(linger) );
    if ( rc != 0 )
    {
        fprintf( stderr,
                 "[ %d ] WARN => linger could not be set\n", ctx->id );    
    }
    zmq_close( _fdm_subscriber );
    zmq_close( _fdm_kill_subscriber );
    zmq_close( _fdm_sync );
    fprintf( stderr,
             "[ %d ] DOWN!  p: %d\n", ctx->id, cnt);
    return NULL;
}

/*
  setup a SITL MavLink TX listening UDP port
 */
void SITL_State::_setup_mavlink_rctx(void)
{
//	fprintf( stdout, "SITL_State::_setup_mavlink_rctx\n" );
	int one=1, ret;
	struct sockaddr_in sockaddr;

	memset(&sockaddr,0,sizeof(sockaddr));

#ifdef HAVE_SOCK_SIN_LEN
	sockaddr.sin_len = sizeof(sockaddr);
#endif
	sockaddr.sin_port = htons(_rctx_port);
	sockaddr.sin_family = AF_INET;

	_rctx_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (_rctx_fd == -1) {
		fprintf(stderr, "SITL: socket failed - %s\n", strerror(errno));
		exit(1);
	}

	/* we want to be able to re-use ports quickly */
	setsockopt(_rctx_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	ret = bind(_rctx_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if (ret == -1) {
		fprintf(stderr, "SITL: bind failed on port %u - %s\n",
			(unsigned)ntohs(sockaddr.sin_port), strerror(errno));
		exit(1);
	}

	AVR_SITL::SITLUARTDriver::_set_nonblocking(_rctx_fd);
}

/*
  setup a SITL FDM ZeroMQ publisher port
 */
void SITL_State::_setup_zmq_fdm(void)
{
//	fprintf( stdout, "SITL_State::_setup_zmq_fdm\n" );

    _sitl_zmq_fdm_publisher = zmq_socket ( _zmq_context,
                                           ZMQ_PUB );
    int rc = zmq_bind (_sitl_zmq_fdm_publisher, "tcp://*:5502");
    if (rc == -1)
    {
        fprintf( stderr, "SITL_State::_setup_zmq_fdm => failed to zmq_bind publisher.\n" );
        exit( 1 );
    }
        


    _sitl_zmq_fdm_kill = zmq_socket( _zmq_context,
                                     ZMQ_PUB );
    rc = zmq_bind (_sitl_zmq_fdm_kill, "inproc://kill");
    if (rc == -1)
    {
        fprintf( stderr, "SITL_State::_setup_zmq_fdm => failed to zmq_bind kill socket\n" );
        exit( 1 );
    }

    _sitl_zmq_fdm_sync = zmq_socket( _zmq_context,
                                     ZMQ_REP );
    rc = zmq_bind (_sitl_zmq_fdm_sync, "inproc://sync");
    if (rc == -1)
    {
        fprintf( stdout, "SITL_State::_setup_zmq_fdm => failed to zmq_bind sync socket\n" );
        exit( 1 );
    }
    return;
}


/*
  timer called at 1kHz
 */
void SITL_State::_timer_handler(int signum)
{
	//static uint32_t last_update_count;
    static uint32_t last_pwm_input;

	static bool in_timer;

	if (in_timer || _scheduler->interrupts_are_blocked() || _sitl == NULL)
    {
		return;
    }

    _scheduler->sitl_begin_atomic();
	in_timer = true;

#ifndef __CYGWIN__
	/* make sure we die if our parent dies */
	if (kill(_parent_pid, 0) != 0){
		exit(1);
	}
#else

	static uint16_t count = 0;
	static uint32_t last_report;

    count++;
	if (hal.scheduler->millis() - last_report > 1000) {
		fprintf(stdout, "TH %u cps\n", count);
	//	print_trace();
		count = 0;
		last_report = hal.scheduler->millis();
	}
#endif

    // simulate RC input at 50Hz
    if (hal.scheduler->millis() - last_pwm_input >= 20 && _sitl->rc_fail == 0) {
        last_pwm_input = hal.scheduler->millis();
        new_rc_input = true;
    }

#ifndef HIL_MODE
	// send RC output to flight sim
	_simulator_output();
#endif

	/* check for packet from MavLink RC TX port, not zeromq since the mavlink ports
       comes in from the MavLink libraries */
	_service_rctx();

	// trigger all APM timers. We do this last as it can re-enable
	// interrupts, which can lead to recursion
	_scheduler->timer_event();

    _scheduler->sitl_end_atomic();
	in_timer = false;
}

#ifndef HIL_MODE
/*
  check for a SITL FDM packet
 */
void SITL_State::_service_rctx(void)
{

	ssize_t size;
	struct pwm_packet {
		uint16_t pwm[8];
	};
	union {
		struct pwm_packet pwm_pkt;
	} d;

	size = recv(_rctx_fd, &d, sizeof(d), MSG_DONTWAIT);
	switch (size) {
	case 16:
        {
            // a packet giving the receiver PWM inputs
            uint8_t i;
            for (i=0; i<8; i++) {
                // setup the pwn input for the RC channel inputs
                if (d.pwm_pkt.pwm[i] != 0) {
                    pwm_input[i] = d.pwm_pkt.pwm[i];
                }
            }
			//printf( "_service_rctx received rctx packet" );
            break;
        }
	}

}

/*
  check for a SITL FDM packet
 */
void SITL_State::service_fdm( void* subscriber, sitl_fdm& fdm )
{

	ssize_t size;
    sitl_fdm fg_pkt;

    size = zmq_recv( subscriber,
                     &fg_pkt,
                     sizeof(fg_pkt),
                     0 );
    switch (size) {
    case 140:

        if (fg_pkt.magic != 0x4c56414f) {
            fprintf(stdout, "Bad FDM packet - magic=0x%08x\n", fg_pkt.magic);
            return;
        }

        if (fg_pkt.latitude == 0 ||
            fg_pkt.longitude == 0 ||
            fg_pkt.altitude <= 0) {
            // garbage input
            return;
        }

        fdm = fg_pkt;
        // prevent bad inputs from SIM from corrupting our state
        double *v = &_sitl->state.latitude;
        for (uint8_t i=0; i<17; i++) {
                if (isinf(v[i]) || isnan(v[i]) || fabsf(v[i]) > 1.0e10) {
                v[i] = 0;
            }
        }
        break;
    };

}
#endif

/*
  apply servo rate filtering
  This allows simulation of servo lag
 */
void SITL_State::_apply_servo_filter(float deltat)
{
    if (_sitl->servo_rate < 1.0f) {
        // no limit
        return;
    }
    // 1000 usec == 90 degrees
    uint16_t max_change = deltat * _sitl->servo_rate * 1000 / 90;
    if (max_change == 0) {
        max_change = 1;
    }
    for (uint8_t i=0; i<11; i++) {
        int16_t change = (int16_t)pwm_output[i] - (int16_t)last_pwm_output[i];
        if (change > max_change) {
            pwm_output[i] = last_pwm_output[i] + max_change;
        } else if (change < -max_change) {
            pwm_output[i] = last_pwm_output[i] - max_change;
        }
    }
}


/*
  send RC outputs to simulator
 */
void SITL_State::_simulator_output(void)
{
	static uint32_t last_update_usec;
	struct {
		uint16_t pwm[11];
		uint16_t speed, direction, turbulance;
	} control;
	/* this maps the registers used for PWM outputs. The RC
	 * driver updates these whenever it wants the channel output
	 * to change */
	uint8_t i;

	if (last_update_usec == 0) {
		for (i=0; i<11; i++) {
			pwm_output[i] = 1000;
		}
		if (_vehicle == ArduPlane) {
			pwm_output[0] = pwm_output[1] = pwm_output[3] = 1500;
			pwm_output[7] = 1800;
		}
		for (i=0; i<11; i++) {
            last_pwm_output[i] = pwm_output[i];
        }
	}

    if (_sitl == NULL) {
        return;
    }

	// output at chosen framerate
    uint32_t now = hal.scheduler->micros();
	if (last_update_usec != 0 && now - last_update_usec < 1000000/_framerate) {
		return;
	}
    float deltat = (now - last_update_usec) * 1.0e-6f;
	last_update_usec = now;

    _apply_servo_filter(deltat);

	for (i=0; i<11; i++) {
		if (pwm_output[i] == 0xFFFF) {
			control.pwm[i] = 0;
		} else {
			control.pwm[i] = pwm_output[i];
		}
        last_pwm_output[i] = pwm_output[i];
	}

	if (_vehicle == ArduPlane) {
		// add in engine multiplier
		if (control.pwm[2] > 1000) {
			control.pwm[2] = ((control.pwm[2]-1000) * _sitl->engine_mul) + 1000;
			if (control.pwm[2] > 2000) control.pwm[2] = 2000;
		}
		_motors_on = ((control.pwm[2]-1000)/1000.0) > 0;
	} else {
		_motors_on = false;
        // apply engine multiplier to first motor
        control.pwm[0] = ((control.pwm[0]-1000) * _sitl->engine_mul) + 1000;
        // run checks on each motor
		for (i=0; i<4; i++) {
            // check motors do not exceed their limits
            if (control.pwm[i] > 2000) control.pwm[i] = 2000;
            if (control.pwm[i] < 1000) control.pwm[i] = 1000;
            // update motor_on flag
			if ((control.pwm[i]-1000)/1000.0 > 0) {
                _motors_on = true;
			}
		}
	}

    float throttle = _motors_on?(control.pwm[2]-1000) / 1000.0f:0;
    // lose 0.7V at full throttle
    float voltage = _sitl->batt_voltage - 0.7f*throttle;
    // assume 50A at full throttle
    _current = 50.0 * throttle;
    // assume 3DR power brick
    voltage_pin_value = ((voltage / 10.1) / 5.0) * 1024;
    current_pin_value = ((_current / 17.0) / 5.0) * 1024;

	// setup wind control
    float wind_speed = _sitl->wind_speed * 100;
    float altitude = _barometer?_barometer->get_altitude():0;
    if (altitude < 0) {
        altitude = 0;
    }
    if (altitude < 60) {
        wind_speed *= altitude / 60.0f;
    }
	control.speed      = wind_speed;
	float direction = _sitl->wind_direction;
	if (direction < 0) {
		direction += 360;
	}
	control.direction  = direction * 100;
	control.turbulance = _sitl->wind_turbulance * 100;

	// zero the wind for the first 15s to allow pitot calibration
	if (hal.scheduler->millis() < 15000) {
		control.speed = 0;
	}

    zmq_send ( _sitl_zmq_fdm_publisher,
               (void*)&control,
               sizeof(control),
               ZMQ_DONTWAIT );

}


/*
  setup a timer used to prod the ISRs
 */
void SITL_State::_setup_timer(void)
{
	struct itimerval it;
	struct sigaction act;

	act.sa_handler = _timer_handler;
        act.sa_flags = SA_RESTART|SA_NODEFER;
        sigemptyset(&act.sa_mask);
        sigaddset(&act.sa_mask, SIGALRM);
        sigaction(SIGALRM, &act, NULL);

	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 1000; // 1KHz
	it.it_value = it.it_interval;

	setitimer(ITIMER_REAL, &it, NULL);
}

// generate a random float between -1 and 1
float SITL_State::_rand_float(void)
{
    return ((((unsigned)random()) % 2000000) - 1.0e6) / 1.0e6;
}

// generate a random Vector3f of size 1
Vector3f SITL_State::_rand_vec3f(void)
{
	Vector3f v = Vector3f(_rand_float(),
                          _rand_float(),
                          _rand_float());
	if (v.length() != 0.0) {
		v.normalize();
	}
	return v;
}

void SITL_State::init(int argc, char * const argv[])
{
	fprintf(stdout, "SITL_State::init\n" );
    pwm_input[0] = pwm_input[1] = pwm_input[3] = 1500;
    pwm_input[4] = pwm_input[7] = 1800;
    pwm_input[2] = pwm_input[5] = pwm_input[6] = 1000;

    _scheduler = (SITLScheduler *)hal.scheduler;
	_parse_command_line(argc, argv);

}

void SITL_State::cleanup()
{
    
    // Clean up the 5760 port listen socket.
    sitlUart0Driver.end();

    // Send kill
    zmq_send( _sitl_zmq_fdm_kill,
              "",
              1,
              0 );


    fprintf(stderr,
            "SITL_State::cleanup  => timer disconnected, kill zmq message sent\n" );

    // Wait for all threads to report in
    {
        uint8_t threadCnt = 0;
        uint8_t syncByte = 0;

        while ( threadCnt < 4 )
        {
            zmq_recv( _sitl_zmq_fdm_sync,
                      &syncByte,
                      sizeof(syncByte),
                      0 );
            zmq_send( _sitl_zmq_fdm_sync,
                      (void*)&syncByte,
                      sizeof(syncByte),
                      0 );
            threadCnt++;
        }
        fprintf( stderr,
                 "SITL_State::cleanup => All threads have reported in!\n" );
    }

    // Mow set the lingers and shutdown
    int linger = 0;
    int rc;
    rc = zmq_setsockopt ( _sitl_zmq_fdm_publisher,
                          ZMQ_LINGER,
                          &linger,
                          sizeof(linger) );
    if ( rc != 0 )
        fprintf(stderr, "SITL_State::cleanup failed to set linger\n" );
    rc = zmq_setsockopt ( _sitl_zmq_fdm_kill,
                          ZMQ_LINGER,
                          &linger,
                          sizeof(linger) );
    if ( rc != 0 )
        fprintf(stderr, "SITL_State::cleanup failed to set linger\n" );
    rc = zmq_setsockopt ( _sitl_zmq_fdm_sync,
                          ZMQ_LINGER,
                          &linger,
                          sizeof(linger) );
    if ( rc != 0 )
        fprintf(stderr, "SITL_State::cleanup failed to set linger\n" );

    zmq_close ( _sitl_zmq_fdm_publisher );
    zmq_close ( _sitl_zmq_fdm_kill );
    zmq_close ( _sitl_zmq_fdm_sync );
    zmq_ctx_destroy( _zmq_context );
    fprintf(stderr, "SITL_State::cleanup => Complete!\n" );

    // Let's idle a little since the cleanup can happen pretty quick,
    uint32_t now = hal.scheduler->micros();
    while ( ( hal.scheduler->micros() - now ) < 2000000 );

    // Remove the timer
    //setitimer(ITIMER_REAL, NULL, NULL);

}

// wait for serial input, or 100usec
void SITL_State::loop_hook(void)
{
    struct timeval tv;
    fd_set fds;
    int fd, max_fd = 0;

    FD_ZERO(&fds);
    fd = ((AVR_SITL::SITLUARTDriver*)hal.uartA)->_fd;
    if (fd != -1) {
        FD_SET(fd, &fds);
        max_fd = max(fd, max_fd);
    }
    fd = ((AVR_SITL::SITLUARTDriver*)hal.uartB)->_fd;
    if (fd != -1) {
        FD_SET(fd, &fds);
        max_fd = max(fd, max_fd);
    }
    fd = ((AVR_SITL::SITLUARTDriver*)hal.uartC)->_fd;
    if (fd != -1) {
        FD_SET(fd, &fds);
        max_fd = max(fd, max_fd);
    }
    fd = ((AVR_SITL::SITLUARTDriver*)hal.uartD)->_fd;
    if (fd != -1) {
        FD_SET(fd, &fds);
        max_fd = max(fd, max_fd);
    }
    fd = ((AVR_SITL::SITLUARTDriver*)hal.uartE)->_fd;
    if (fd != -1) {
        FD_SET(fd, &fds);
        max_fd = max(fd, max_fd);
    }
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    fflush(stdout);
    fflush(stderr);
    select(max_fd+1, &fds, NULL, NULL, &tv);
}


#endif



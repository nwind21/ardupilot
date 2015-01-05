#ifndef __AP_INERTIALSENSORMOCKIMPL
#define __AP_INERTIALSENSORMOCKIMPL

#include <gmock/gmock.h>
#include "AP_InertialSensor.h"
#include "vector3.h"
#include "GyroSimulator.h"
#include "AccelSimulator.h"

class AP_InertialSensor_Mock : public AP_InertialSensor
{   
public:  
    MOCK_METHOD0( update, bool() );
    MOCK_CONST_METHOD0( get_delta_time, float() );
    MOCK_METHOD0( get_gyro_drift_rate, float (void) );
    MOCK_METHOD1( wait_for_sample, bool(uint16_t timeout_ms) );
    MOCK_METHOD1( _init_sensor, uint16_t( Sample_rate sample_rate ) );

public:
    void mockUpdateGyros();
    void mockUpdateAccel();
    void setGyroSimulator( GyroSimulator* gs ) { gyroSimulator = gs; }
    void setAccelSimulator( AccelSimulator* as ) { accelSimulator = as; }

    GyroSimulator* gyroSimulator;
    AccelSimulator* accelSimulator;
};

#endif


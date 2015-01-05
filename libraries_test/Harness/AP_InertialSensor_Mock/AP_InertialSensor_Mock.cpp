#include "AP_InertialSensor_Mock.h"

void AP_InertialSensor_Mock::mockUpdateGyros()
{
    int numGyros = min(get_gyro_count(), INS_MAX_INSTANCES);
    for ( int i = 0; i < numGyros; i++ )
    {
        gyroSimulator->update( _gyro[i] );
    }
}

void AP_InertialSensor_Mock::mockUpdateAccel()
{
    int numAccel = min(get_accel_count(), INS_MAX_INSTANCES);
    for ( int i = 0; i < numAccel; i++ )
    {
        accelSimulator->update( _gyro[i] );
    }
}
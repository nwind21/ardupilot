/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_INERTIAL_SENSOR_STUB_H__
#define __AP_INERTIAL_SENSOR_STUB_H__

#include <AP_Progmem.h>
#include "AP_InertialSensor.h"

#define HIL_MAX_LASTUPDATE_MS  (40000)

class AP_InertialSensor_HIL : public AP_InertialSensor
{
public:

    AP_InertialSensor_HIL();

    /* Concrete implementation of AP_InertialSensor functions: */
    virtual bool  update();
    virtual float get_delta_time() const;
    virtual float get_gyro_drift_rate();
    virtual bool wait_for_sample(uint16_t timeout_ms);
    virtual void set_accel(uint8_t instance, const Vector3f &accel);
    virtual void set_gyro(uint8_t instance, const Vector3f &gyro);
    virtual bool get_gyro_health(uint8_t instance) const;
    virtual bool get_accel_health(uint8_t instance) const;
    virtual uint8_t get_gyro_count(void) const;
    virtual uint8_t get_accel_count(void) const;

protected:
    virtual bool _sample_available();
    uint16_t _init_sensor( Sample_rate sample_rate );
    uint32_t _sample_period_usec;
    uint32_t _last_sample_usec;
    uint32_t _last_accel_usec[INS_MAX_INSTANCES];
    uint32_t _last_gyro_usec[INS_MAX_INSTANCES];
};

#endif // __AP_INERTIAL_SENSOR_STUB_H__

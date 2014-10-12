/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AP_HAL.h>
#if CONFIG_HAL_BOARD == HAL_BOARD_AVR_SITL

#include "AP_HAL_AVR_SITL.h"
#include "SITL_State.h"
#include "AnalogIn.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

using namespace AVR_SITL;

extern const AP_HAL::HAL& hal;

ADCSource::ADCSource(uint8_t pin) :
    _pin(pin)
{}

float ADCSource::read_average() {
	return read_latest();
}

float ADCSource::voltage_average() {
	return (5.0f/1023.0f) * read_average();
}

float ADCSource::voltage_latest() {
	return (5.0f/1023.0f) * read_latest();
}

float ADCSource::read_latest() {
    switch (_pin) {
    case ANALOG_INPUT_BOARD_VCC:
        return 1023;
        
    case 0:
        return AVR_SITL::SITL_State::sonar_pin_value;

    case 1:
        return AVR_SITL::SITL_State::airspeed_pin_value;

    case 12:
        return AVR_SITL::SITL_State::current_pin_value;

    case 13:
        return AVR_SITL::SITL_State::voltage_pin_value;

    case ANALOG_INPUT_NONE:
    default:
        return 0.0;
    }
}

void ADCSource::set_pin(uint8_t pin) {
    _pin = pin;
}

void SITLAnalogIn::init(void *ap_hal_scheduler) {
}

AP_HAL::AnalogSource* SITLAnalogIn::channel(int16_t pin) {
    return new ADCSource(pin);	
}

#endif

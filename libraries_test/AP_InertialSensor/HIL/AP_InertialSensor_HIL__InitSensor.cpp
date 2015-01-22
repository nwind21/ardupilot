#include "includes.h"
#include "defines.h"

#define private public
#define protected public
#include "AP_InertialSensor_HIL.h"

using ::testing::Return;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE:  BAT Fixture
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class AP_InertialSensor_HIL__InitSensor : public ::testing::Test
{
public:
    AP_InertialSensor_HIL classUT;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   HELPERS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   TESTS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TEST_F( AP_InertialSensor_HIL__InitSensor, ExceedMaxInstances_Ignored )
{
    // Making an assumption the sample rates start at 50Hz then doubles from there.
    //---------------------------------------------------------
    uint32_t expectedFrequency = 50; //Hz
    uint16_t retVal = 0xFF;
    for ( int i = (int)AP_InertialSensor::RATE_50HZ; i < (int)AP_InertialSensor::RATE_400HZ; i++ )
    {
        retVal = classUT._init_sensor( (AP_InertialSensor::Sample_rate)i );
        ASSERT_EQ( retVal,
                   AP_PRODUCT_ID_NONE );
        ASSERT_EQ( classUT._sample_period_usec,
                   1000000/expectedFrequency );
        expectedFrequency *= 2;
    }
    //---------------------------------------------------------
}

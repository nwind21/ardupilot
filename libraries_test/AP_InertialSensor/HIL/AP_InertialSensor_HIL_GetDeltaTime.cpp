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
class AP_InertialSensor_HIL_GetDeltaTime : public ::testing::Test
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
TEST_F( AP_InertialSensor_HIL_GetDeltaTime, DeltaTime_MatchDependsSamplePeriod )
{
    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );

    //---------------------------------------------------------
    ASSERT_EQ( classUT.get_delta_time(),
               20000 * 1.0e-6f );
    //---------------------------------------------------------
}


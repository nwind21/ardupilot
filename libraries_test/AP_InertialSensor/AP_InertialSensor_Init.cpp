#include "includes.h"
#include "defines.h"

#include "AP_InertialSensor_Mock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::Eq;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE:  BAT Fixture
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class AP_InertialSensor_Init : public ::testing::Test
{
public:
    AP_InertialSensor_Mock classUT;
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
TEST_F( AP_InertialSensor_Init, IfAccelScaleZero_InitializeToVectorOfOne )
{
    EXPECT_CALL( classUT, _init_sensor( Eq( AP_InertialSensor::RATE_50HZ ) ) )
        .Times(1);

    classUT.init( AP_InertialSensor::WARM_START,
                  AP_InertialSensor::RATE_50HZ );

    //---------------------------------------------------------
    for ( uint8_t i = 0; i < classUT.get_accel_count(); i++ )
    {
        ASSERT_TRUE( classUT._accel_scale[i].get() == Vector3f(1,1,1) );
    }
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_Init, WarmStart_DoesNotInitGyro )
{
    EXPECT_CALL( classUT, _init_sensor( Eq( AP_InertialSensor::RATE_50HZ ) ) )
        .Times(1);

    //---------------------------------------------------------
    classUT.init( AP_InertialSensor::WARM_START,
                  AP_InertialSensor::RATE_50HZ );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_Init, coldStart_callsInitGyro )
{
    EXPECT_CALL( classUT, _init_sensor( Eq( AP_InertialSensor::RATE_50HZ ) ) )
        .Times(1);

    EXPECT_CALL( MOCK_HAL.scheduler, delay( Eq(5) ) )
        .WillRepeatedly( Return() );

    EXPECT_CALL( classUT, update( ) )
        .WillRepeatedly( Return( true ) );

    //---------------------------------------------------------
    classUT.init( AP_InertialSensor::COLD_START,
                  AP_InertialSensor::RATE_50HZ );
    //---------------------------------------------------------
}

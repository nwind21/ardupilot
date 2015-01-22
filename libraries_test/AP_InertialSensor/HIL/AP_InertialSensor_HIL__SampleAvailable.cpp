#include "includes.h"
#include "defines.h"

#include "AP_InertialSensor_HIL.h"

using ::testing::Return;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE:  BAT Fixture
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class AP_InertialSensor_HIL__SampleAvailable : public ::testing::Test
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
TEST_F( AP_InertialSensor_HIL__SampleAvailable, ElapsedSamplePeriod_ReturnTrue )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return( 20001 )  );

    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );

    //---------------------------------------------------------
    ASSERT_EQ( classUT._sample_available(),
               true );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL__SampleAvailable, NotElapsedSamplePeriod_ReturnFalse )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0) )
        .WillOnce( Return(19999) );

    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );
    classUT.update();

    //---------------------------------------------------------
    ASSERT_EQ( classUT._sample_available(),
               false );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL__SampleAvailable, RolloverElapsedSamplePeriod_ReturnFalse )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0xFFFFFFFF) )
        .WillOnce( Return(20000) );

    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );
    classUT.update();

    //---------------------------------------------------------
    ASSERT_EQ( classUT._sample_available(),
               true );
    //---------------------------------------------------------
}

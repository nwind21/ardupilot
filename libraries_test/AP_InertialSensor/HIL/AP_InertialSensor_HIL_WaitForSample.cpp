#include <gtest.h>
#include "defines.h"

#define private public
#define protected public
#include "AP_InertialSensor_HIL.h"

using ::testing::Return;
using ::testing::_;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE:  BAT Fixture
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class AP_InertialSensor_HIL_WaitForSample : public ::testing::Test
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
TEST_F( AP_InertialSensor_HIL_WaitForSample, SampleAlreadyAvailable )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0) )
        .WillOnce( Return(20000) );

    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );
    classUT.update();

    //---------------------------------------------------------
    ASSERT_EQ( classUT.wait_for_sample( 1 ),
               true );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_WaitForSample, NotAvailable_ExceedsTimeout )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0) )
        .WillRepeatedly( Return(1) );

    EXPECT_CALL( MOCK_HAL.scheduler, delay(_))
        .Times(1);
        
    EXPECT_CALL( MOCK_HAL.scheduler, millis())
        .WillOnce( Return(0) )
        .WillOnce( Return(9) )
        .WillOnce( Return(10) );
        
    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );
    classUT.update();

    //---------------------------------------------------------
    ASSERT_EQ( classUT.wait_for_sample( 10 ),
               false );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_WaitForSample, NotAvailable_AvailableBeforeTimeout )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0) )
        .WillOnce( Return(1) )
        .WillOnce( Return(20000) );

    EXPECT_CALL( MOCK_HAL.scheduler, delay(_))
        .Times(1);
        
    EXPECT_CALL( MOCK_HAL.scheduler, millis())
        .WillOnce( Return(0) )
        .WillOnce( Return(9) );
        
    classUT._init_sensor( AP_InertialSensor::RATE_50HZ );
    classUT.update();

    //---------------------------------------------------------
    ASSERT_EQ( classUT.wait_for_sample( 10 ),
               true );
    //---------------------------------------------------------
}
#include <gtest.h>
#include "defines.h"

#include "AP_InertialSensor_HIL.h"

using ::testing::Return;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE:  BAT Fixture
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class AP_InertialSensor_HIL_GetAccelCount : public ::testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    AP_InertialSensor_HIL classUT;
    Vector3f vector1UT;
    Vector3f vector2UT;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   HELPERS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void AP_InertialSensor_HIL_GetAccelCount::SetUp()
{
}

void AP_InertialSensor_HIL_GetAccelCount::TearDown()
{
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   TESTS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TEST_F( AP_InertialSensor_HIL_GetAccelCount, Healthy_Returns2 )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return( 0 ) )
        .WillOnce( Return( 0 ) )
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS-1) );

    classUT.set_accel( 0, vector1UT );
    classUT.set_accel( 1, vector2UT );

    //----------------------------------------------------------------
    ASSERT_EQ( classUT.get_accel_count(),
               2 );
    //----------------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_GetAccelCount, ExceedUpdatePeriod_Returns1 )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return( 0 ) )
        .WillOnce( Return( 0 ) )
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) );

    classUT.set_accel( 0, vector1UT );
    classUT.set_accel( 1, vector2UT );

    ASSERT_EQ( classUT.get_accel_count(),
               1 );
}
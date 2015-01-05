#include <gtest.h>
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
class AP_InertialSensor_HIL_SetGyro : public ::testing::Test
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
void AP_InertialSensor_HIL_SetGyro::SetUp()
{
    // Give some values
    vector1UT( 1, 2, 3 );
    vector2UT( 2, 3, 4 );
}

void AP_InertialSensor_HIL_SetGyro::TearDown()
{
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   TESTS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TEST_F( AP_InertialSensor_HIL_SetGyro, CallWill_StoreGyroInCurrent )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) );

    //---------------------------------------------------------
    classUT.set_gyro( 0, vector1UT );
    ASSERT_EQ( classUT._gyro[0],
               vector1UT );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_SetGyro, CallWill_StoreMicroTimeInLastUpdate )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) );

    //---------------------------------------------------------
    classUT.set_gyro( 0, vector1UT );
    ASSERT_EQ( classUT._last_gyro_usec[0],
               (uint32_t)HIL_MAX_LASTUPDATE_MS );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_SetGyro, ExceedMaxInstances_Ignored )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .Times(0);

    //---------------------------------------------------------
    classUT.set_gyro( INS_MAX_INSTANCES, vector1UT );
    //---------------------------------------------------------
}


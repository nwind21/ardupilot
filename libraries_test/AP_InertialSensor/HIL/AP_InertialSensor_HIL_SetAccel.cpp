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
class AP_InertialSensor_HIL_SetAccel : public ::testing::Test
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
void AP_InertialSensor_HIL_SetAccel::SetUp()
{
    // Give some values
    vector1UT( 1, 2, 3 );
    vector2UT( 2, 3, 4 );
}

void AP_InertialSensor_HIL_SetAccel::TearDown()
{
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   TESTS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TEST_F( AP_InertialSensor_HIL_SetAccel, CallWill_StoreLastAccelInPrevious )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) )
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) );

    classUT.set_accel( 0, vector1UT );
    classUT.set_accel( 0, vector2UT );

    //---------------------------------------------------------
    ASSERT_EQ( classUT._previous_accel[0],
               vector1UT );
    ASSERT_NE( classUT._previous_accel[0],
               vector2UT );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_SetAccel, CallWill_StoreAccelInCurrent )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) );

    //---------------------------------------------------------
    classUT.set_accel( 0, vector1UT );
    ASSERT_EQ( classUT._accel[0],
               vector1UT );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_SetAccel, CallWill_StoreMicroTimeInLastUpdate )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(HIL_MAX_LASTUPDATE_MS) );

    //---------------------------------------------------------
    classUT.set_accel( 0, vector1UT );
    ASSERT_EQ( classUT._last_accel_usec[0],
               (uint32_t)HIL_MAX_LASTUPDATE_MS );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_SetAccel, ExceedMaxInstances_Ignored )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .Times(0);

    //---------------------------------------------------------
    classUT.set_accel( INS_MAX_INSTANCES, vector1UT );
    //---------------------------------------------------------
}


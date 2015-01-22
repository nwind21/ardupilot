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
class AP_InertialSensor_HIL_GetAccelHealth : public ::testing::Test
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
void AP_InertialSensor_HIL_GetAccelHealth::SetUp()
{
}

void AP_InertialSensor_HIL_GetAccelHealth::TearDown()
{
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//   TESTS
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
TEST_F( AP_InertialSensor_HIL_GetAccelHealth, RolloverHandled )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return( 0xFFFFFFFF ) )
        .WillOnce( Return( HIL_MAX_LASTUPDATE_MS ) )
        .WillOnce( Return( HIL_MAX_LASTUPDATE_MS-1 ) );

    classUT.set_accel( 0, vector1UT );

    //----------------------------------------------------------------
    ASSERT_EQ( classUT.get_accel_health( 0 ),
               false );
    ASSERT_EQ( classUT.get_accel_health( 0 ),
               true );
    //----------------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_GetAccelHealth, LastUpdateOk_ReturnsTrue )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .Times(INS_MAX_INSTANCES)
        .WillRepeatedly( Return( HIL_MAX_LASTUPDATE_MS - 1 ) );

    //----------------------------------------------------------------
    for ( int i = 0; i < INS_MAX_INSTANCES; i++ )
    {
        ASSERT_EQ( classUT.get_accel_health( i ),
                   true );
    }
    //----------------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_GetAccelHealth, LastUpdateExceeded_ReturnsFalse )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .Times(INS_MAX_INSTANCES)
        .WillRepeatedly( Return( HIL_MAX_LASTUPDATE_MS ) );

    //----------------------------------------------------------------
    for ( int i = 0; i < INS_MAX_INSTANCES; i++ )
    {
        ASSERT_EQ( classUT.get_accel_health( i ),
                   false );
    }
    //----------------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_GetAccelHealth, MaxPlusOneInstance_ReturnsFalse )
{
    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .Times(0);

    //----------------------------------------------------------------
    ASSERT_EQ( classUT.get_accel_health( INS_MAX_INSTANCES ),
               false );
    //----------------------------------------------------------------
}

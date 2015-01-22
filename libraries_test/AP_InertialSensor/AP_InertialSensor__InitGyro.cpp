#include "includes.h"
#include "defines.h"
#include <stdio.h>

#include "AP_InertialSensor_Mock.h"


using ::testing::Return;
using ::testing::_;
using ::testing::Eq;
using ::testing::Invoke;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE:  BAT Fixture
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class AP_InertialSensor__InitGyro : public ::testing::Test
{
public:
    void SetUp( ) { classUT.setGyroSimulator( &simulator ); }
    void TearDown( ) { simulator.clearAll(); }

public:
    AP_InertialSensor_Mock classUT;
    GyroSimulator simulator;
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
TEST_F( AP_InertialSensor__InitGyro, noDrift_returnsFixedOffsets )
{
    //pClassUT = new dummy( );

    EXPECT_CALL( MOCK_HAL.scheduler, delay( Eq(5) ) )
        .WillRepeatedly( Return() );

    EXPECT_CALL( classUT, update( ) )
        .WillRepeatedly( DoAll( Invoke( &classUT, &AP_InertialSensor_Mock::mockUpdateGyros ),
                                Return( true ) ) );

    //---------------------------------------------------------
    simulator.setAxisOffset( Vector3f( -1.0, 0.5, 0.3 ) );
    classUT._init_gyro( );

    int numGyros = min(classUT.get_gyro_count(), INS_MAX_INSTANCES);
    for ( int i = 0; i < numGyros; i++ )
    {
        ASSERT_FLOAT_EQ( classUT._gyro_offset[i].get().x,
                         -1.0 );
        ASSERT_FLOAT_EQ( classUT._gyro_offset[i].get().y,
                         0.5 );
        ASSERT_FLOAT_EQ( classUT._gyro_offset[i].get().z,
                         0.3 );
    }
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor__InitGyro, smallDrift_returnsApproxOffsets )
{

    EXPECT_CALL( MOCK_HAL.scheduler, delay( Eq(5) ) )
        .WillRepeatedly( Return() );

    EXPECT_CALL( classUT, update( ) )
        .WillRepeatedly( DoAll( Invoke( &classUT, &AP_InertialSensor_Mock::mockUpdateGyros ),
                                Return( true ) ) );

    simulator.setAxisOffset( Vector3f( -1.0, 0.5, 0.3 ) );
    simulator.setAxisNoise( Vector3f( 0.1, 0.1, 0.1 ) );

    //---------------------------------------------------------
    classUT._init_gyro( );

    int numGyros = min(classUT.get_gyro_count(), INS_MAX_INSTANCES);
    for ( int i = 0; i < numGyros; i++ )
    {
        ASSERT_TRUE( classUT._gyro_offset[i].get().x > -1.05 );
        ASSERT_TRUE( classUT._gyro_offset[i].get().x < -0.95 );
        ASSERT_TRUE( classUT._gyro_offset[i].get().y > 0.45 );
        ASSERT_TRUE( classUT._gyro_offset[i].get().y < 0.55 );
        ASSERT_TRUE( classUT._gyro_offset[i].get().z > 0.25 );
        ASSERT_TRUE( classUT._gyro_offset[i].get().z < 0.35 );
    }
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor__InitGyro, stub )
{
}

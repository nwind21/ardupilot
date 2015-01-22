#include "includes.h"
#include "defines.h"

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
    class AP_InertialSensor_HIL_Wrapper : public AP_InertialSensor_HIL
    {
    public:
        AP_InertialSensor_HIL_Wrapper() : AP_InertialSensor_HIL() { }
        virtual ~AP_InertialSensor_HIL_Wrapper() { }
        MOCK_METHOD0( _sample_available, bool() );
    };

public:
    AP_InertialSensor_HIL_WaitForSample() {  }
    ~AP_InertialSensor_HIL_WaitForSample() {  }
    void SetUp()
    {
        classUT = new AP_InertialSensor_HIL_Wrapper();
    }
    void TearDown()
    {
        delete classUT;
    }
    AP_InertialSensor_HIL_Wrapper* classUT;
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
TEST_F( AP_InertialSensor_HIL_WaitForSample, stub )
{
    EXPECT_CALL( *classUT, _sample_available())
        .Times(0);
}

TEST_F( AP_InertialSensor_HIL_WaitForSample, SampleAlreadyAvailable )
{
    EXPECT_CALL( *classUT, _sample_available())
        .WillOnce( Return(true) );

    classUT->_init_sensor( AP_InertialSensor::RATE_50HZ );

    //---------------------------------------------------------
    ASSERT_EQ( classUT->wait_for_sample( 1 ),
               true );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_WaitForSample, NotAvailable_ExceedsTimeout )
{
    EXPECT_CALL( *classUT, _sample_available())
        .WillRepeatedly( Return(false) );

    EXPECT_CALL( MOCK_HAL.scheduler, delay_microseconds(_))
        .Times(1);

    EXPECT_CALL( MOCK_HAL.scheduler, millis())
        .WillOnce( Return(0) )
        .WillOnce( Return(9) )
        .WillOnce( Return(10) );

    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0) );

    classUT->_init_sensor( AP_InertialSensor::RATE_50HZ );

    //---------------------------------------------------------

    ASSERT_EQ( classUT->wait_for_sample( 10 ),
               false );
    //---------------------------------------------------------
}

TEST_F( AP_InertialSensor_HIL_WaitForSample, NotAvailable_AvailableBeforeTimeout )
{
    EXPECT_CALL( *classUT, _sample_available())
        .WillOnce( Return(false) )
        .WillOnce( Return(true) );

    EXPECT_CALL( MOCK_HAL.scheduler, delay_microseconds(_))
        .Times(1);

    EXPECT_CALL( MOCK_HAL.scheduler, micros())
        .WillOnce( Return(0) );

    EXPECT_CALL( MOCK_HAL.scheduler, millis())
        .WillOnce( Return(0) )
        .WillOnce( Return(9) );

    classUT->_init_sensor( AP_InertialSensor::RATE_50HZ );

    //---------------------------------------------------------
    ASSERT_EQ( classUT->wait_for_sample( 10 ),
               true );
    //---------------------------------------------------------
}

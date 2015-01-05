#include <gtest.h>
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
class AP_InertialSensor__InitAccel : public ::testing::Test
{
public:
    void SetUp( ) { classUT.setAccelSimulator( &simulator ); } 
    void TearDown( ) { simulator.clearAll(); } 

public:
    AP_InertialSensor_Mock classUT;
    AccelSimulator simulator;
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
TEST_F( AP_InertialSensor__InitAccel, stub )
{
}
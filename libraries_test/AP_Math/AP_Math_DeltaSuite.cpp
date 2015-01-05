#include <gtest.h>
#include "AP_Math.h"

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//  FIXTURE TESTS
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
TEST( DeltaFixture, BAT )
{
    ASSERT_EQ( delta_u32( 0, 0 ),
               0 );
    ASSERT_EQ( delta_u32( 1, 0 ),
               1 );
    ASSERT_EQ( delta_u32( 0xFFFFFFFF, 0  ),
               0xFFFFFFFF );
}

TEST( DeltaFixture, CatchRollover )
{
    ASSERT_EQ( delta_u32( 0, 4 ),
               0xFFFFFFFF-4 );
    ASSERT_EQ( delta_u32( 1, 0xFFFFFFFE ),
               2 );
    ASSERT_EQ( delta_u32( 0xFFFFFFFD, 0xFFFFFFFE ),
               0xFFFFFFFE );
}




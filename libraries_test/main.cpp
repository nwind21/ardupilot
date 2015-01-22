#include <gtest/gtest.h>
#include "AP_HAL_Mock.h"

AP_HAL_Mock mockHal;
const AP_HAL::HAL& hal = mockHal;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

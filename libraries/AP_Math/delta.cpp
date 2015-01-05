#include "delta.h"

#define MAX_U32_VALUE 0xFFFFFFFF

uint32_t delta_u32( uint32_t large,
                    uint32_t small )
{
    return ( large < small ) ? ( large + ( MAX_U32_VALUE - small ) ) : ( large - small );
}
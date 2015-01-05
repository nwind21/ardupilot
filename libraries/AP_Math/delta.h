#ifndef __DELTA_H
#define __DELTA_H

#include <stdint.h>

/**
* \brief Delta two values, accomodate for roll-over
* \param large larger of the two values
* \param small smaller of the two values
*
* Take two values, the first is always the expected larger and caculate the delta
* while taking into account roll-over.
*
*/
uint32_t delta_u32( uint32_t large, uint32_t small );

#endif


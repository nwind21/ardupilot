#include "AccelSimulator.h"
#include <stdio.h>

void AccelSimulator::update( Vector3f& gyro )
{
    float noise = 0;

    gyro = m_axisOffset;
    
    // If noise is set via setAxisNoise then we'll add a little
    if ( m_usingNoise )
    {
        noise =( rand() % (int)( m_axisNoise.x * 100.0 ) ) / 100.0;
        noise *= ( rand() % 2 ) ? -1 : 1;
        gyro.x += noise;
        
        noise =( rand() % (int)( m_axisNoise.y * 100.0 ) ) / 100.0;
        noise *= ( rand() % 2 ) ? -1 : 1;
        gyro.y += noise;

        noise =( rand() % (int)( m_axisNoise.z * 100.0 ) ) / 100.0;
        noise *= ( rand() % 2 ) ? -1 : 1;
        gyro.z += noise;
    }
    
    // No convergence.  Every call alternate +/- convergance
    if ( m_noConvergence )
    {
        static int cnt = 0;
        gyro = m_axisOffset;
        if ( cnt % 2 )
        {
            gyro.x += m_convergenceDrift;
            gyro.y += m_convergenceDrift;
            gyro.z += m_convergenceDrift;
        }
        else
        {
            gyro.x -= m_convergenceDrift;
            gyro.y -= m_convergenceDrift;
            gyro.z -= m_convergenceDrift;
        }
        
    }
}


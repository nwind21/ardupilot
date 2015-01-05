#ifndef __GYROSIMULATION
#define __GYROSIMULATION

#include <stdlib.h>
#include <stdio.h>
#include "AP_Math.h"

class GyroSimulator
{
    
public:
    GyroSimulator() :
        m_usingNoise( false )
      , m_noConvergence( false )
      , m_convergenceDrift( 0.0 ) {}
    void update( Vector3f& gyro );

public:
    void setAxisOffset(Vector3f vector) { m_axisOffset = vector; }
    void setAxisNoise(Vector3f vector) { m_axisNoise = vector; m_usingNoise = true; }
    void setAxisNoConvergence( float val ) { m_convergenceDrift = val; m_noConvergence = true; }

    void clearAxisOffset() { m_axisOffset = Vector3f(0,0,0 ); }
    void clearAxisNoise() { m_axisNoise = Vector3f(0,0,0 ); m_usingNoise = false; }
    void clearAxisConvergence() { m_convergenceDrift = 0; m_noConvergence = false; }
    void clearAll()
    {
        clearAxisOffset();
        clearAxisNoise();
        clearAxisConvergence();
    }
    
private:
    Vector3f m_axisOffset;
    Vector3f m_axisNoise;
    bool m_usingNoise;
    bool m_noConvergence;
    float m_convergenceDrift;
};

#endif


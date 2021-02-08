/*
  ==============================================================================

    PhantomPhasor.cpp
    Created: 27 Jan 2021 13:03:12
    Author:  Matthew Maxwell

  ==============================================================================
*/

#include "PhantomPhasor.h"

#include "PhantomUtils.h"

PhantomPhasor::PhantomPhasor(AudioProcessorValueTreeState& vts)
    :   m_parameters(vts)
{
    p_shape = m_parameters.getRawParameterValue(Consts::_PHASOR_SHAPE_PARAM_ID);
    p_egInt = m_parameters.getRawParameterValue(Consts::_PHASOR_EG_INT_PARAM_ID);
    p_lfoInt = m_parameters.getRawParameterValue(Consts::_PHASOR_LFO_INT_PARAM_ID);
}

PhantomPhasor::~PhantomPhasor()
{
    p_shape = nullptr;
    p_egInt = nullptr;
    p_lfoInt = nullptr;
}

//==========================================================================
float PhantomPhasor::apply(float oldPhase, float egMod, float lfoMod) noexcept
{
    float envelope = *p_egInt * egMod * (*p_lfoInt * -0.5f + 1.0f);
    float lfo = *p_lfoInt * (lfoMod * 0.5f + 0.5f) * (*p_egInt * -0.5f + 1.0f);
    float mod = envelope + lfo;

    oldPhase /= (float) Consts::_WAVETABLE_SIZE;

    float phasor = evaluate(oldPhase);
    float phase = (phasor * mod) + (oldPhase * (1.0f - mod));

    return phase * (float) Consts::_WAVETABLE_SIZE;
}

//==============================================================================
float PhantomPhasor::evaluate(float phase) noexcept
{
    switch((int) *p_shape)
    {
        default:
        case 0:
            return sawtooth(phase);
            break;

        case 1:
        case 2:
            return square(phase);
            break;
    }
}

float PhantomPhasor::sawtooth(float x) noexcept
{
    float m;
    float xb = 0.01f;
    
    if(x <= xb)
    {
        m = 0.5f / xb;
        return m * x;
    }
    else
    {
        m = 0.5f / (1.0f - xb);
        return m * (x - xb) + 0.5f;
    }
}

float PhantomPhasor::square(float x) noexcept
{
    // slope
    float m;

    // x-breakpoints
    float xb1 = 0.49f;
    float xb2 = 0.51f;
    
    // y-intercepts
    float b1 = 0.01f;
    float b2 = 0.97959183f;

    // y-value
    float y;

    if(x < xb1)
    {
        m = 0.01f / xb1;
        y = m * x;
    }
    else if(x <= xb2)
    {
        float mx = 0.98f * (x - xb1) / (xb2 - xb1);
        y = mx + b1;
    }
    else
    {
        m = 0.01f / (1.0f - xb2);
        y = m * x + b2;
    }

    return y / 2.0f;
}

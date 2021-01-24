/*
  ==============================================================================

    PhantomFilter.cpp
    Created: 23 Jan 2021 21:22:03
    Author:  Matthew Maxwell

  ==============================================================================
*/

#include "PhantomFilter.h"

#include "PhantomUtils.h"

//==============================================================================
PhantomFilter::PhantomFilter(AudioProcessorValueTreeState& vts, PhantomEnvelopeGenerator& eg, dsp::ProcessSpec& ps)
    :   m_parameters(vts), m_eg(&eg)
{
    m_filter = new dsp::StateVariableTPTFilter<float>();
    m_filter->prepare(ps);
    m_filter->setType(m_type);
    m_filter->snapToZero();

    p_cutoff = m_parameters.getRawParameterValue(Params::_FLTR_CUTOFF_PARAM_ID);
    p_resonance = m_parameters.getRawParameterValue(Params::_FLTR_RESO_PARAM_ID);
    p_egInt = m_parameters.getRawParameterValue(Params::_FLTR_EG_INT_PARAM_ID);

    update();
}

PhantomFilter::~PhantomFilter()
{
    m_filter = nullptr;
    m_eg = nullptr;

    p_cutoff = nullptr;
    p_resonance = nullptr;
    p_egInt = nullptr;
}

//==============================================================================
void PhantomFilter::update() noexcept
{
    // NOTE: Frequency is not being set here because it is called in the update 
    // function. Discontinuous numbers could result in artifacts.
    m_filter->setResonance(*p_resonance);
}

//==============================================================================
float PhantomFilter::evaluate(float sample) noexcept
{
    float offset = k_cutoffModulationMultiplier * m_eg->getNextSample() * *p_egInt;
    float frequency = clip(*p_cutoff + offset, k_cutoffLowerBounds, k_cutoffUpperCounds);
    m_filter->setCutoffFrequency(frequency);

    return m_filter->processSample(k_channelNumber, sample);
}

float PhantomFilter::clip(float n, float lower, float upper) noexcept
{
    return std::max(lower, std::min(n, upper));
}

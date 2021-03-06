/*
  ==============================================================================

    PhantomEnvelope.cpp
    Created: 22 Jan 2021 10:41:40
    Author:  Matthew Maxwell

  ==============================================================================
*/

#include "PhantomEnvelope.h"

PhantomEnvelope::PhantomEnvelope(AudioProcessorValueTreeState& vts, EnvelopeType type) : m_parameters(vts), m_type(type)
{
    setEnvelopeType();
    setEnvelopeParameters();
}

PhantomEnvelope::~PhantomEnvelope()
{
    p_attack = nullptr;
    p_decay = nullptr;
    p_sustain = nullptr;
    p_release = nullptr;
}

void PhantomEnvelope::update(float sampleRate) noexcept
{
    setEnvelopeParameters();
    setSampleRate(sampleRate);
}

float PhantomEnvelope::evaluate() noexcept
{
    float result = (getNextSample() + m_previousSample) / 2.0f;
    m_previousSample = result;
    
    return result;
}

void PhantomEnvelope::setEnvelopeType()
{
    jassert((int) m_type != -1);

    char* atkParamId;
    char* decParamId;
    char* susParamId;
    char* relParamId;

    switch(m_type)
    {
        default:
        case EnvelopeType::AMP:
            atkParamId = Consts::_AMP_EG_ATK_PARAM_ID;
            decParamId = Consts::_AMP_EG_DEC_PARAM_ID;
            susParamId = Consts::_AMP_EG_SUS_PARAM_ID;
            relParamId = Consts::_AMP_EG_REL_PARAM_ID;
            break;

        case EnvelopeType::PHASOR:
            atkParamId = Consts::_PHASOR_EG_ATK_PARAM_ID;
            decParamId = Consts::_PHASOR_EG_DEC_PARAM_ID;
            susParamId = Consts::_PHASOR_EG_SUS_PARAM_ID;
            relParamId = Consts::_PHASOR_EG_REL_PARAM_ID;
            break;

        case EnvelopeType::FILTER:
            atkParamId = Consts::_FLTR_EG_ATK_PARAM_ID;
            decParamId = Consts::_FLTR_EG_DEC_PARAM_ID;
            susParamId = Consts::_FLTR_EG_SUS_PARAM_ID;
            relParamId = Consts::_FLTR_EG_REL_PARAM_ID;
            break;

        case EnvelopeType::MOD:
            atkParamId = Consts::_MOD_EG_ATK_PARAM_ID;
            decParamId = Consts::_MOD_EG_DEC_PARAM_ID;
            susParamId = Consts::_MOD_EG_SUS_PARAM_ID;
            relParamId = Consts::_MOD_EG_REL_PARAM_ID;
            break;
    }

    p_attack = m_parameters.getRawParameterValue(atkParamId);
    p_decay = m_parameters.getRawParameterValue(decParamId);
    p_sustain = m_parameters.getRawParameterValue(susParamId);
    p_release = m_parameters.getRawParameterValue(relParamId);
}

void PhantomEnvelope::setEnvelopeParameters() noexcept
{
    m_envelope.attack = *p_attack;
    m_envelope.decay = *p_decay;
    m_envelope.release = *p_release;

    if(m_previousSustain != *p_sustain)
    {
        float sustain = (m_previousSustain + *p_sustain) / 2.0f;
        m_previousSustain = sustain;

        float gain = powf(2.0f, sustain / 6.0f);
        m_envelope.sustain = gain; 
    }

    setParameters(m_envelope);
}

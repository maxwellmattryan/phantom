/*
  ==============================================================================

    PhantomOscillator.h
    Created: 20 Jan 2021 21:17:28
    Author:  Matthew Maxwell

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PhantomPhasor.h"
#include "PhantomWaveshaper.h"

//==============================================================================
/**
*/
class PhantomOscillator 
{
public:
    //==========================================================================
    PhantomOscillator(AudioProcessorValueTreeState&, int);
    ~PhantomOscillator();

    //==========================================================================
    void update(int midiNoteNumber, float sampleRate) noexcept;

    float evaluate(float modEgMod, float phaseEgMod, float lfoMod) noexcept;

    float midiNoteToFrequency(float midiNote) noexcept;

private:
    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhantomOscillator)

    //==========================================================================
    void initParameters();
    void initWavetable();

    void updateFrequency() noexcept;
    void updatePhaseDelta() noexcept;
    void updatePhaseDelta(float frequency) noexcept;
    
    //==========================================================================
    AudioProcessorValueTreeState& m_parameters;

    std::atomic<float>* p_oscRange;
    std::atomic<float>* p_oscCoarseTune;
    std::atomic<float>* p_oscFineTune;
    std::atomic<float>* p_modDepth;
    std::atomic<float>* p_modMode;
    std::atomic<float>* p_shapeInt;

    //==========================================================================
    Array<float> m_wavetable;
    const int k_modExpoThreshold = 5;

    float m_phase = 0.0f;
    float m_phaseDelta = 0.0f;

    int m_midiNoteNumber;
    float m_frequency;
    float m_sampleRate;

    int m_oscNumber;

    std::unique_ptr<PhantomPhasor> m_phasor;
    std::unique_ptr<PhantomWaveshaper> m_waveshaper;
};

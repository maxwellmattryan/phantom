/*
  ==============================================================================
    This file was auto-generated!
    It contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PhantomAmplifier.h"
#include "PhantomSynth.h"

/**
 * The audio processor component, containing the important JUCE boilerplate 
 * code for the plugin.
 */
class PhantomAudioProcessor : public AudioProcessor
{
public:
    PhantomAudioProcessor();
    ~PhantomAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    float getSkewFactor(float start, float end, float center);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhantomAudioProcessor)

    AudioProcessorValueTreeState m_parameters;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::unique_ptr<PhantomSynth> m_synth;
    std::unique_ptr<PhantomAmplifier> m_amp;
};

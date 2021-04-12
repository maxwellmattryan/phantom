/*
  ==============================================================================

    PhantomAnalyzer.cpp
    Created: 12 Apr 2021 16:27:57
    Author:  Matthew Maxwell

  ==============================================================================
*/

#include "PhantomAnalyzer.h"

PhantomAnalyzer::PhantomAnalyzer()
    :   m_forwardFFT(FFT_ORDER),
        m_window(FFT_SIZE, dsp::WindowingFunction<float>::hann)
{
    zeromem(m_outputData, sizeof(m_outputData));
    setSize(240, 360);
    startTimerHz(30);
}

PhantomAnalyzer::~PhantomAnalyzer()
{
    stopTimer();
}

void PhantomAnalyzer::paint(Graphics& graphics)
{
    const float width = (float) getWidth();
    const float height = (float) getHeight();

    graphics.setColour(Colours::transparentBlack);
    graphics.fillAll();

    Range<float> maxRange = FloatVectorOperations::findMinAndMax(m_outputData, OUTPUT_SIZE);
    const float scale = 1.0f / jmax((float) FFT_SIZE, maxRange.getEnd());

    graphics.setColour(m_fillStartColour);
    for(int i = 0; i < OUTPUT_SIZE; i++) 
    {
        float x = std::log10f(1.0f + 39.0f * ((i + 1.0f) / OUTPUT_SIZE)) / std::log10f(40.0f) * width;

        float magnitude = m_outputData[i] * scale;
        float decibels = Decibels::gainToDecibels(magnitude);
        float y = jmap(decibels, -90.0f, -15.0f, height, 0.0f);

        graphics.drawVerticalLine((int) x, (int) y, height);
    }
}

void PhantomAnalyzer::resized()
{

}

void PhantomAnalyzer::setColours(Colour stroke, Colour start, Colour end)
{
    m_strokeColour = stroke;
    m_fillStartColour = start;
    m_fillEndColour = end;
}

void PhantomAnalyzer::timerCallback()
{
    if(m_isNextFFTBlockReady)
    {
        m_window.multiplyWithWindowingTable(m_FFTData, FFT_SIZE);
        m_forwardFFT.performFrequencyOnlyForwardTransform(m_FFTData);

        FloatVectorOperations::max(m_outputData, m_outputData, m_FFTData, OUTPUT_SIZE);

        m_isNextFFTBlockReady = false;
    }

    for(int i = 0; i < OUTPUT_SIZE; i++)
    {
        m_outputData[i] *= 0.7f;
    }

    repaint();
}

void PhantomAnalyzer::pushBuffer(AudioSampleBuffer& buffer)
{
    if(buffer.getNumChannels() > 0)
    {
        const float* channelData = buffer.getReadPointer(0);

        for(int i = 0; i < buffer.getNumSamples(); i++)
        {
            pushNextSample(channelData[i]);
        }
    }
}

void PhantomAnalyzer::pushNextSample(float sample) 
{
    if(m_FFTQueueIndex == FFT_SIZE)
    {
        if(!m_isNextFFTBlockReady)
        {
            zeromem(m_FFTData, sizeof(m_FFTData));
            memcpy(m_FFTData, m_FFTQueue, sizeof(m_FFTQueue));

            m_isNextFFTBlockReady = true;
        }

        m_FFTQueueIndex = 0;
    }

    m_FFTQueue[m_FFTQueueIndex++] = sample;
}

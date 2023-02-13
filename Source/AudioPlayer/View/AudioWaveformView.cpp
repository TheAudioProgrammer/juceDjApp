/*
  ==============================================================================

    AudioWaveFormView.cpp
    Created: 10 Feb 2023 8:20:27pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "AudioWaveformView.h"

AudioWaveformView::AudioWaveformView (AudioPlayerState& s, juce::AudioBuffer<float>& b, juce::AudioFormatManager& manager)
: state(s)
, audioBuffer (b)
, thumbnail (1024, manager, thumbnailCache)
{
    state.addChangeListener (this);
    thumbnail.addChangeListener (this);
}

AudioWaveformView::~AudioWaveformView()
{
    state.removeAllChangeListeners();
    thumbnail.removeAllChangeListeners();
}

void AudioWaveformView::prepare (double rate)
{
    sampleRate = rate;
}

void AudioWaveformView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds());
    
    if (state.hasLoadedFile)
        drawWaveform (g);
}

void AudioWaveformView::drawWaveform (juce::Graphics& g)
{
    if (audioBuffer.getNumSamples() > 0)
    {
        g.setColour (juce::Colours::skyblue);
        thumbnail.drawChannel (g, getLocalBounds(), 0.0f, thumbnail.getTotalLength(), 0, 1.0f);
    }
}

void AudioWaveformView::resized()
{
    
}

void AudioWaveformView::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &state)
    {
        update();
    }
}

void AudioWaveformView::update()
{
    if (state.hasLoadedFile)
    {
        // You haven't called prepare!
        jassert (sampleRate > 0.0);
        thumbnail.clear();
        thumbnailCache.clear();
        thumbnail.setSource (&audioBuffer, sampleRate, 0);
        repaint();
    }
    
    else if (!state.hasLoadedFile)
    {
        thumbnail.clear();
        thumbnailCache.clear();
        repaint();
    }
}

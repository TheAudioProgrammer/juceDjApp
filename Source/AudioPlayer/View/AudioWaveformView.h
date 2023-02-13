/*
  ==============================================================================

    AudioWaveFormView.h
    Created: 10 Feb 2023 8:20:27pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../AudioPlayer/State/AudioPlayerState.h"


class AudioWaveformView : public juce::Component, public juce::ChangeListener
{
public:
    AudioWaveformView (AudioPlayerState& s, juce::AudioBuffer<float>& b, juce::AudioFormatManager& manager);
    ~AudioWaveformView();
    void prepare (double rate);
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    // Change Listener
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

private:
    void drawWaveform (juce::Graphics& g);
    void update();
    
    AudioPlayerState& state;
    juce::AudioBuffer<float>& audioBuffer;
    double sampleRate { 0.0 };
    juce::AudioThumbnailCache thumbnailCache { 1 };
    juce::AudioThumbnail thumbnail;
};

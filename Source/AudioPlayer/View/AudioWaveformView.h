/*
  ==============================================================================

    AudioWaveFormView.h
    Created: 10 Feb 2023 8:20:27pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <juce_events/juce_events.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_utils/juce_audio_utils.h>


#include "../../AudioPlayer/State/AudioPlayerState.h"


class AudioWaveformView : public juce::Component, public juce::ChangeListener
{
public:
    AudioWaveformView (AudioPlayerState& s, juce::AudioBuffer<float>& b, juce::AudioFormatManager& manager);
    ~AudioWaveformView() override;
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

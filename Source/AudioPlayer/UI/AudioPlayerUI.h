/*
  ==============================================================================

    AudioPlayerUI.h
    Created: 25 Jan 2023 4:12:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Data/AudioPlayerData.h"
#include "../../State/AudioPlayerState.h"


//==============================================================================
/*
*/
class AudioPlayerUI  : public juce::Component, public juce::ChangeListener
{
public:
    AudioPlayerUI (AudioPlayerData& p);
    ~AudioPlayerUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // From ChangeListener
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

private:
    AudioPlayerData& audioPlayerData;
    juce::TextButton loadAudioButton { "Load" };
    juce::TextButton playAudioButton { "Play" };
    juce::TextButton stopAudioButton { "Stop" };
    
    juce::Slider gainSlider { juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow };
    
    juce::Label trackNameLabel { "Song Name" };
    juce::Label artistNameLabel { "Artist Name" };
    juce::Label trackLengthLabel { "Song Length" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerUI)
};

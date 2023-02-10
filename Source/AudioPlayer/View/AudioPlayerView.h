/*
  ==============================================================================

    AudioPlayerView.h
    Created: 25 Jan 2023 4:12:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../State/AudioPlayerState.h"



//==============================================================================
/*
*/
class AudioPlayerView : public juce::Component, public juce::ChangeListener
{
public:
    AudioPlayerView (AudioPlayerState& s, AudioMetadata& m);
    ~AudioPlayerView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // From ChangeListener
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    double getGainSliderValue() const { return gainSlider.getValue(); }
    
    // Lambdas to control -- we aren't giving the view direct access to the processor.  
    std::function<void()> onPlay = [](){};
    std::function<void()> onStop = [](){};
    std::function<void()> onLoad = [](){};
    std::function<void()> onGainChange = [](){};

private:
    void update();
    
    AudioPlayerState& state;
    AudioMetadata& metadata;
    juce::TextButton loadAudioButton { "Load" };
    juce::TextButton playAudioButton { "Play" };
    juce::TextButton stopAudioButton { "Stop" };
    
    juce::Slider gainSlider { juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow };
    
    juce::Label trackNameLabel { "Song Name" };
    juce::Label artistNameLabel { "Artist Name" };
    juce::Label trackLengthLabel { "Song Length" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerView)
};

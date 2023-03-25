/*
  ==============================================================================

    AudioPlayerView.h
    Created: 25 Jan 2023 4:12:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../../LookAndFeel/StyleSheet.h"
#include "../State/AudioPlayerState.h"
#include "../../Metadata/Metadata.h"



//==============================================================================
/*
*/
class AudioPlayerView : public juce::Component,
                        public juce::DragAndDropTarget,
                        public juce::ChangeListener
{
public:
    AudioPlayerView (AudioPlayerState& s, Metadata& m);
    ~AudioPlayerView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // From ChangeListener
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    // From Drag and drop target
    void itemDragEnter (const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
    bool isInterestedInDragSource (const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
    void itemDropped (const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
    
    double getGainSliderValue() const { return gainSlider.getValue(); }
    
    // Lambdas to control -- we aren't giving the view direct access to the processor.  
    std::function<void()> onPlay = [](){};
    std::function<void()> onStop = [](){};
    std::function<void()> onLoad = [](){};
    std::function<void()> onGainChange = [](){};
    std::function<void()> onTrackDroppedFromPlayList = [](){};
    
    float percentageInTrackPlayed { 0.0f };
    juce::String pathToDroppedTrack { "" };
    
private:
    void drawDisc (juce::Graphics& g);
    void update();
    
    AudioPlayerState& state;
    Metadata& metadata;
    juce::TextButton loadAudioButton { "Load" };
    juce::TextButton playAudioButton { "Play" };
    juce::TextButton stopAudioButton { "Stop" };
    
    juce::Slider gainSlider { juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextBoxBelow };
    
    juce::Label trackNameLabel { "Song Name" };
    juce::Label artistNameLabel { "Artist Name" };
    juce::Label trackLengthLabel { "Song Length" };
    
    bool somethingIsBeingDraggedOver = false;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerView)
};

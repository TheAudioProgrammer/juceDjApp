/*
  ==============================================================================

    AudioPlayer.h
    Created: 9 Feb 2023 4:01:30pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "Data/AudioPlayerProcessor.h"
#include "View/AudioPlayerView.h"

/* Encapsulates audio processing, UI (view), and state into one object that can be easily controlled -- mutual area to pass data around between processor & view */
struct AudioPlayer : public juce::Timer
{
    AudioPlayer()
    {
        view.onPlay = [this]()
        {
            processor.play();
        };
        
        view.onStop = [this]()
        {
            processor.stop();
        };
        
        view.onLoad = [this]()
        {
            loadFile();
        };
        
        view.onGainChange = [this]()
        {
            processor.setDecibelValue (view.getGainSliderValue());
        };
        
        startTimerHz (30.0f);
    }
    
    ~AudioPlayer()
    {
        stopTimer();
    }
    
    void timerCallback()
    {
        processor.convertSamplesToTime();
        view.percentageInTrackPlayed = processor.getPercentagePlayedInTrack();
        view.repaint();
    }
    
    AudioPlayerProcessor processor;
    AudioPlayerView view { processor.getState(), processor.getMetadata() };
    
    void loadFile()
    {
        songSelector = std::make_unique<juce::FileChooser>("Select a track to play", juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDesktopDirectory), processor.getAudioFormatManager().getWildcardForAllFormats());
        
        auto songSelectorFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        songSelector->launchAsync (songSelectorFlags, [&] (const juce::FileChooser& chooser)
        {
            processor.loadTrack (chooser.getResult());
        });
    }
    
private:
    /* Window to select a track */
    std::unique_ptr<juce::FileChooser> songSelector;
    
};

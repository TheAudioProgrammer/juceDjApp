/*
  ==============================================================================

    AudioPlayer.h
    Created: 9 Feb 2023 4:01:30pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Data/AudioPlayerProcessor.h"
#include "View/AudioPlayerView.h"
#include "View/AudioWaveformView.h"


/* Encapsulates audio processing, UI (view), and state into one object that can be easily controlled -- mutual area to pass data around between processor & view */
struct AudioPlayer : public juce::Timer
{
    AudioPlayer()
    {
        playerView.onPlay = [this]()
        {
            processor.play();
        };
        
        playerView.onStop = [this]()
        {
            processor.stop();
        };
        
        playerView.onLoad = [this]()
        {
            loadFile();
        };
        
        playerView.onGainChange = [this]()
        {
            processor.setDecibelValue (static_cast<float>(playerView.getGainSliderValue()));
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
        playerView.percentageInTrackPlayed = processor.getPercentagePlayedInTrack();
        playerView.repaint();
        waveformView.repaint();
    }
    
    void loadFile()
    {
        processor.getState().setLoaded (false);
        
        songSelector = std::make_unique<juce::FileChooser>("Select a track to play", juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDesktopDirectory), processor.getAudioFormatManager().getWildcardForAllFormats());
        
        auto songSelectorFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        songSelector->launchAsync (songSelectorFlags, [&] (const juce::FileChooser& chooser)
        {
            processor.loadTrack (chooser.getResult());
        });
    }
    
    AudioPlayerProcessor processor;
    AudioPlayerView playerView { processor.getState(), processor.getMetadata() };
    AudioWaveformView waveformView { processor.getState(), processor.getTrackBuffer(), processor.getAudioFormatManager() };

private:
    /* Window to select a track */
    std::unique_ptr<juce::FileChooser> songSelector;
    
};

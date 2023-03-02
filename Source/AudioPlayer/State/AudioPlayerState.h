/*
  ==============================================================================

    AudioPlayerState.h
    Created: 25 Jan 2023 4:27:08pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once


/* Holds loaded track data */
struct AudioMetadata 
{
    juce::String artistName { "" };
    juce::String trackName { "" };
    juce::String trackCurrentTime { "" };
    juce::String trackLength { "" };
};

// Set by the processor, observed by the view
struct AudioPlayerState : public juce::ChangeBroadcaster
{
    bool isPlaying = false;
    bool hasLoadedFile = false;
    
    void setPlaying (bool playing)
    {
        isPlaying = playing;
        sendChangeMessage();
    }
    
    void setLoaded (bool loaded)
    {
        hasLoadedFile = loaded;
        sendChangeMessage();
    }    
};

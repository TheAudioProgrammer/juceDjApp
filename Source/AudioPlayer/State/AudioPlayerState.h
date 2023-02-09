/*
  ==============================================================================

    AudioPlayerState.h
    Created: 25 Jan 2023 4:27:08pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/* Holds loaded track data */
struct AudioMetaData
{
    juce::String artistName { "" };
    juce::String trackName { "" };
    juce::String trackLength { "" };
};


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
    
    AudioMetaData metadata;
};

/*
  ==============================================================================

    AudioPlayerData.h
    Created: 25 Jan 2023 4:11:19pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../State/AudioPlayerState.h"


class AudioPlayerData
{
public:
    AudioPlayerData();
    bool loadFile();
    void processAudio (const juce::AudioSourceChannelInfo& bufferToFill);
    void setGain (float newGain);
    AudioPlayerState getPlayState() const { return playState; }
    void setPlayState (AudioPlayerState newState) { playState = newState; }
    
private:
    juce::AudioFormatManager audioFormatManager;
    juce::AudioBuffer<float> audioSourceBuffer;
    int readPosition { 0 };
    bool fileIsLoaded { false };
    AudioPlayerState playState { AudioPlayerState::Stopped };
};







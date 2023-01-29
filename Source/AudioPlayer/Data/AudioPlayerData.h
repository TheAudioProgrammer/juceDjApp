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
    juce::File findFile();
    bool loadSong();
    void prepareToPlay (int numChannels, int samplesPerBlock, double sampleRate);
    void processAudio (const juce::AudioSourceChannelInfo& bufferToFill);
    void setDecibelValue (float value);
    AudioPlayerState getPlayState() const { return playState; }
    void setPlayState (AudioPlayerState newState) { playState = newState; }
    
private:
    juce::AudioFormatManager audioFormatManager;
    std::unique_ptr<juce::FileChooser> songSelector;
    
    // Holds "entire track"
    juce::AudioBuffer<float> audioSourceBuffer;
    
    // Holds mutable "temp data" where we can change gain / processing etc before being passed to main buffer
    juce::AudioBuffer<float> playerBuffer;
    
    int readPosition { 0 };
    bool fileIsLoaded { false };
    float rawGain { 1.0f };
    AudioPlayerState playState { AudioPlayerState::Stopped };
};







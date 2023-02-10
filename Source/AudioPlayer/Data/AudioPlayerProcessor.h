/*
  ==============================================================================

    AudioPlayerProcessor.h
    Created: 25 Jan 2023 4:11:19pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../State/AudioPlayerState.h"



//================================================================================

class AudioPlayerProcessor 
{
public:
    AudioPlayerProcessor();
    void loadTrack (const juce::File& musicFile);
    void prepareToPlay (int numChannels, int samplesPerBlock, double sampleRate);
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill);
    void play();
    void stop();
    void setDecibelValue (float value);
    AudioPlayerState& getState() { return state; }
    AudioMetadata& getMetadata() { return metadata; }
    juce::AudioFormatManager& getAudioFormatManager() { return audioFormatManager; }
    
private:
    void loadMetadata (juce::AudioFormatReader& reader);
    
    /* Copies player buffer data to main audio buffer */
    void processAudio (const juce::AudioSourceChannelInfo& bufferToFill);
    
    /* Necessary to register and stream audio formats */
    juce::AudioFormatManager audioFormatManager;
    
    /* Holds "entire track" in memory */
    juce::AudioBuffer<float> audioSourceBuffer;
    
    /* Holds mutable "temp data" where we can change gain / processing etc before being passed to the main output buffer */
    juce::AudioBuffer<float> playerBuffer;
    
    /* How far we are in the track */
    int readPosition { 0 };
    
    /* Ensure our file is loaded before trying to access it */
    bool fileIsLoaded { false };
    
    /* Class member that will hold our conversion from dBFS */
    float rawGain { 1.0f };
    
    /* Holds the state & metadata of our player */
    AudioPlayerState state;
    AudioMetadata metadata;
};






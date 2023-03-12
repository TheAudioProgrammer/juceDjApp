/*
  ==============================================================================

    AudioPlayerProcessor.h
    Created: 25 Jan 2023 4:11:19pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

#include "../../Metadata/Metadata.h"
#include "../../Metadata/TagReader.h"
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
    void convertSamplesToTime();
    float getPercentagePlayedInTrack();
    AudioPlayerState& getState() { return state; }
    Metadata& getMetadata() { return metadata; }
    juce::AudioFormatManager& getAudioFormatManager() { return audioFormatManager; }
    juce::AudioBuffer<float>& getTrackBuffer() { return audioSourceBuffer; }
    
private:
    void loadMetadata (const juce::File& musicFile);
    
    /* Helper to ensure time is always "00" rather than "0" until it gets to "10" */
    juce::String convertTimeToString (double time);
    
    /* Copies player buffer data to main audio buffer */
    void processAudio (const juce::AudioSourceChannelInfo& bufferToFill);
    
    /* Necessary to register and stream audio formats */
    juce::AudioFormatManager audioFormatManager;
    
    /* Holds an entire track in memory */
    juce::AudioBuffer<float> audioSourceBuffer;
    
    /* Holds mutable "temp data" where we can change gain / processing etc before being passed to the main output buffer */
    juce::AudioBuffer<float> playerBuffer;
    
    /* How far we are in the track */
    int readPosition { 0 };
    
    /* How many samples are in the track */
    juce::int64 trackNumSamples { 0 };
    
    /* Current sample rate of the soundcard */
    double currentSampleRate { 0.0 };
    
    /* Ensure our file is loaded before trying to access it */
    //bool fileIsLoaded { false };
    
    /* Class member that will hold our conversion from dBFS */
    float rawGain { 1.0f };
    
    /* Holds the state & metadata of our player */
    AudioPlayerState state;
    Metadata metadata;
};







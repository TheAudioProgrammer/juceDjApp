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


class AudioPlayerData : public juce::ChangeBroadcaster
{
public:
    AudioPlayerData();
    bool loadTrack();
    void prepareToPlay (int numChannels, int samplesPerBlock, double sampleRate);
    void processAudio (const juce::AudioSourceChannelInfo& bufferToFill);
    void setDecibelValue (float value);
    AudioPlayerState getPlayState() const { return playState; }
    void setPlayState (AudioPlayerState newState) { playState = newState; }
    juce::String getArtistName() const { return artistName; }
    juce::String getTrackName() const { return trackName; }
    juce::String getTrackLength() const { return trackLength; }
    
private:
    void loadMetadata (juce::AudioFormatReader& reader);
    
    /* Necessary to register and stream audio formats */
    juce::AudioFormatManager audioFormatManager;
    
    /* Window to select a track */
    std::unique_ptr<juce::FileChooser> songSelector;
    
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
    
    /* Holds the state of our player (for now) */
    AudioPlayerState playState { AudioPlayerState::Stopped };
    
    juce::String artistName { "" };
    juce::String trackName { "" };
    juce::String trackLength { "" };
};







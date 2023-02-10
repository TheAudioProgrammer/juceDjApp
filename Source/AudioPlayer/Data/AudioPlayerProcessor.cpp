/*
  ==============================================================================

    AudioPlayerProcessor.cpp
    Created: 25 Jan 2023 4:11:19pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "AudioPlayerProcessor.h"

AudioPlayerProcessor::AudioPlayerProcessor()
{
    audioFormatManager.registerBasicFormats();
}

void AudioPlayerProcessor::loadTrack (const juce::File& musicFile)
{
    auto* r = audioFormatManager.createReaderFor (musicFile);
    std::unique_ptr<juce::AudioFormatReader> reader (r);
    
    if (reader)
    {
        auto numSamples = static_cast<int>(reader->lengthInSamples);
        
        audioSourceBuffer.setSize (reader->numChannels, numSamples);
        jassert (numSamples > 0 && reader->numChannels > 0);
        
        // If we have metadata, load it!  Otherwise fall back to file name as track
        if (reader->metadataValues.size() > 0)
            loadMetadata (*reader);
        else
            metadata.trackName = musicFile.getFileNameWithoutExtension();
        
        //metadata.trackLength = juce::String { reader->lengthInSamples / reader->sampleRate };
        trackNumSamples = reader->lengthInSamples;
        
        bool wasLoadSuccessful = reader->read (&audioSourceBuffer, 0, numSamples, 0, true, true);
        state.setLoaded (wasLoadSuccessful);
    }
}

/* It'd be much more convenient if we could call this directly in the audio thread, however this could create problems when the message thread wants to access as well, so I'm calling this from "Audioplayer" with a timer */
void AudioPlayerProcessor::convertSamplesToTime()
{
    auto millis =  readPosition / currentSampleRate * 1000.0;
    auto minutes = millis / 60000.0;
    auto seconds = (minutes - std::floor (minutes)) * 60.0;
    auto hundreths = (seconds - std::floor (seconds)) * 100.0;
    
    metadata.trackCurrentTime = convertTimeToString (minutes) + ":" + convertTimeToString (seconds) + ":" + convertTimeToString (hundreths);
}

juce::String AudioPlayerProcessor::convertTimeToString (double time)
{
    return time < 10.0 ? "0" + juce::String (std::floor (time)) : juce::String (std::floor (time));
}

float AudioPlayerProcessor::getPercentagePlayedInTrack()
{
    return readPosition > 0.0f ? juce::jmap<float>(float(readPosition), 1.0f, float(trackNumSamples), 0.0f, 100.0f) : 0.0f;
}

void AudioPlayerProcessor::loadMetadata (juce::AudioFormatReader& reader)
{
    auto metadataValues = reader.metadataValues;
    auto metadataKeys = metadataValues.getAllKeys();
    
    for (int i = 0; i < metadataValues.size(); i++)
    {
        auto value = metadataValues.getValue (metadataKeys[i], "");
        std::cout << "Key: " << metadataKeys[i] << " Value: " << value << std::endl;
    }
}

void AudioPlayerProcessor::prepareToPlay (int numChannels, int samplesPerBlock, double sampleRate)
{
    playerBuffer.setSize (numChannels, samplesPerBlock);
    currentSampleRate = sampleRate;
}

void AudioPlayerProcessor::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (state.hasLoadedFile && state.isPlaying)
        processAudio (bufferToFill);
}

void AudioPlayerProcessor::processAudio (const juce::AudioSourceChannelInfo &bufferToFill)
{
    auto* mainBuffer = bufferToFill.buffer;
    auto samplesRemaining = audioSourceBuffer.getNumSamples() - readPosition;
    
    /* We want to ensure the amount of samples we're processing is never greater
     than the amount of samples remaining in the track (that would be bad!). For
     This reason we can't just blindly copy "buffer->getNumSamples() every time. */
    auto samplesToProcess = samplesRemaining > mainBuffer->getNumSamples() ? mainBuffer->getNumSamples() : samplesRemaining;
    
    playerBuffer.clear();
    
    // You haven't called prepareToPlay()!
    jassert (playerBuffer.getNumChannels() == mainBuffer->getNumChannels());
    jassert (playerBuffer.getNumSamples() > 0 && playerBuffer.getNumSamples() == mainBuffer->getNumSamples());
    
    for (int ch = 0; ch < mainBuffer->getNumChannels(); ch++)
    {
        playerBuffer.copyFrom (ch, 0, audioSourceBuffer, ch, readPosition, samplesToProcess);
        playerBuffer.applyGain (ch, 0, playerBuffer.getNumSamples(), rawGain);
        
        // Add samples to main buffer (Note: May want to change this later)
        mainBuffer->addFrom (ch, 0, playerBuffer, ch, 0, samplesToProcess);
    }
    
    // Move read position along...
    readPosition+=samplesToProcess;
}

void AudioPlayerProcessor::play()
{
    state.setPlaying (true);
}

void AudioPlayerProcessor::stop()
{
    state.setPlaying (false);
}

void AudioPlayerProcessor::setDecibelValue (float value)
{
    rawGain = juce::Decibels::decibelsToGain (value);
}


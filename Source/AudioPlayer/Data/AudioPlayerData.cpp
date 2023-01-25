/*
  ==============================================================================

    AudioPlayerData.cpp
    Created: 25 Jan 2023 4:11:19pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "AudioPlayerData.h"

AudioPlayerData::AudioPlayerData()
{
    audioFormatManager.registerBasicFormats();
}

bool AudioPlayerData::loadFile()
{
    DBG ("Load button clicked");
    juce::File musicFile { "/Users/theaudioprogrammer/Desktop/Alibi.wav" };
    jassert (musicFile.exists());
    
    auto* r = audioFormatManager.createReaderFor (juce::File ("/Users/theaudioprogrammer/Desktop/Alibi.wav"));
    std::unique_ptr<juce::AudioFormatReader> reader (r);
    
    jassert (reader != nullptr);
    
    auto numSamples = static_cast<int>(reader->lengthInSamples);
    
    audioSourceBuffer.setSize (reader->numChannels, numSamples);
    jassert (numSamples > 0 && reader->numChannels > 0);
    
    // Was the file load successful?
    return reader->read (&audioSourceBuffer, 0, numSamples, 0, true, true);
}

void AudioPlayerData::processAudio (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* buffer = bufferToFill.buffer;
    
    // Copy buffer size number of samples of audioSourceBuffer to main buffer
    jassert (buffer->getNumChannels() == audioSourceBuffer.getNumChannels());
    for (int ch = 0; ch < buffer->getNumChannels(); ch++)
    {
        buffer->copyFrom (ch, 0, audioSourceBuffer, ch, readPosition, buffer->getNumSamples());
    }
    
    readPosition+=buffer->getNumSamples();
}

void AudioPlayerData::setGain (float newGain)
{
    auto numSamples = audioSourceBuffer.getNumSamples();
    
    // apply temp gain to track (protect our ears)
    for (int ch = 0; ch < audioSourceBuffer.getNumChannels(); ch++)
    {
        audioSourceBuffer.applyGain(ch, 0, numSamples, newGain);
    }
}


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
    readPosition = 0;
    
    auto* r = audioFormatManager.createReaderFor (musicFile);
    std::unique_ptr<juce::AudioFormatReader> reader (r);
    
    if (reader)
    {
        auto numSamples = static_cast<int>(reader->lengthInSamples);
        
        audioSourceBuffer.setSize ((int)reader->numChannels, numSamples);
        jassert (numSamples > 0 && reader->numChannels > 0);
        
        loadMetadata (musicFile);
        
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

void AudioPlayerProcessor::loadMetadata (const juce::File& musicFile)
{
    TagLib::FileRef tagReader (musicFile.getFullPathName().toUTF8());
    
    if (! tagReader.isNull() && tagReader.tag())
    {
        TagLib::Tag* rawTag = tagReader.tag();
        auto tag = std::unique_ptr<TagLib::Tag>(rawTag);

        std::cout << "-- TAG (basic) --" << std::endl;
        std::cout << "title   - \"" << tag->title()   << "\"" << std::endl;
        std::cout << "artist  - \"" << tag->artist()  << "\"" << std::endl;
        std::cout << "album   - \"" << tag->album()   << "\"" << std::endl;
        std::cout << "year    - \"" << tag->year()    << "\"" << std::endl;
        std::cout << "comment - \"" << tag->comment() << "\"" << std::endl;
        std::cout << "track   - \"" << tag->track()   << "\"" << std::endl;
        std::cout << "genre   - \"" << tag->genre()   << "\"" << std::endl;
        
        metadata.trackName = juce::String (tag->title().toCString());
        metadata.artistName = juce::String (tag->artist().toCString());
    
        if(! tagReader.isNull() && tagReader.audioProperties())
        {
            
            TagLib::AudioProperties *properties = tagReader.audioProperties();
            
            int seconds = properties->length() % 60;
            int minutes = (properties->length() - seconds) / 60;
            
            std::cout << "-- AUDIO --" << std::endl;
            std::cout << "bitrate     - " << properties->bitrate() << std::endl;
            std::cout << "sample rate - " << properties->sampleRate() << std::endl;
            std::cout << "channels    - " << properties->channels() << std::endl;
            std::cout << "length      - " << minutes << ":" << std::setfill('0') << std::setw(2) << seconds << std::endl;
        }
    }
    else
    {
        metadata.trackName = musicFile.getFileNameWithoutExtension();
        metadata.artistName = "Unknown";
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


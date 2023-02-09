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

bool AudioPlayerProcessor::loadTrack()
{
    DBG ("Load button clicked");
    
    songSelector = std::make_unique<juce::FileChooser>("Select a track to play", juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDesktopDirectory), audioFormatManager.getWildcardForAllFormats());
    
    auto songSelectorFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    songSelector->launchAsync (songSelectorFlags, [&] (const juce::FileChooser& chooser)
    {
        juce::File musicFile = chooser.getResult();
        
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
                trackName = musicFile.getFileNameWithoutExtension();
            
            trackLength = juce::String { reader->lengthInSamples / reader->sampleRate };
            
            sendChangeMessage();
            
            // Was the file load successful?
            return reader->read (&audioSourceBuffer, 0, numSamples, 0, true, true);
        }
        
        return false;
    });
    
    return false;
}

void AudioPlayerProcessor::loadMetadata (juce::AudioFormatReader& reader)
{
    auto metadata = reader.metadataValues;
    auto metadataKeys = metadata.getAllKeys();
    
    for (int i = 0; i < metadata.size(); i++)
    {
        auto value = metadata.getValue (metadataKeys[i], "");
        std::cout << "Key: " << metadataKeys[i] << " Value: " << value << std::endl;
    }
}

void AudioPlayerProcessor::prepareToPlay (int numChannels, int samplesPerBlock, double sampleRate)
{
    playerBuffer.setSize (numChannels, samplesPerBlock);
}

void AudioPlayerProcessor::processAudio (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* mainBuffer = bufferToFill.buffer;
    
    playerBuffer.clear();
    
    // You haven't called prepareToPlay()!
    jassert (playerBuffer.getNumChannels() == mainBuffer->getNumChannels());
    jassert (playerBuffer.getNumSamples() > 0 && playerBuffer.getNumSamples() == mainBuffer->getNumSamples());
    
    for (int ch = 0; ch < mainBuffer->getNumChannels(); ch++)
    {
        playerBuffer.copyFrom (ch, 0, audioSourceBuffer, ch, readPosition, playerBuffer.getNumSamples());
        playerBuffer.applyGain (ch, 0, playerBuffer.getNumSamples(), rawGain);
        
        // Add samples to main buffer (Note: May want to change this later)
        mainBuffer->addFrom (ch, 0, playerBuffer, ch, 0, mainBuffer->getNumSamples());
    }
    
    readPosition+=mainBuffer->getNumSamples();
}

void AudioPlayerProcessor::setDecibelValue (float value)
{
    rawGain = juce::Decibels::decibelsToGain (value);
}


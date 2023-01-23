/*
  ==============================================================================

    MixerDevice.h
    Created: 22 Jan 2023 3:23:50pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


/* This is where we describe data for each mixer that we may connect to the app and bundle this data to pass to our device manager and change settings, such as:
    - audio device name
    - midi device name (which may differ from the audio device name (or be the same!)
    - Samplerate the mixer may want to function at
    - Buffersize the mixer wants to operate at
 */

class MixerDevice
{
public:
    enum class ProcessingType
    {
        Internal,
        External
    };
    
    MixerDevice (const juce::String& mixerAudioName, const juce::String& mixerMidiName, const double rate, const int buffer, const int channel, const int cc, const int channelIns, const int channelOuts)
    : audioDeviceName (mixerAudioName), midiDeviceName (mixerMidiName), sampleRate (rate), bufferSize (buffer), midiChannel (channel), midiCC (cc), inputChannels (channelIns), outputChannels (channelOuts) {}
    
    // How we update the device manager when we connect the mixer
    juce::AudioDeviceManager::AudioDeviceSetup createAudioDeviceSetup()
    {
        juce::AudioDeviceManager::AudioDeviceSetup setup;
        
        setup.inputDeviceName = audioDeviceName;
        setup.outputDeviceName = audioDeviceName;
        setup.sampleRate = sampleRate;
        setup.bufferSize = bufferSize;
        setup.inputChannels.setRange (0, inputChannels, true);
        setup.outputChannels.setRange (0, outputChannels, true);
        
        return setup;
    }
    
    // Getters & Setters
    juce::String getAudioDeviceName() const { return audioDeviceName; }
    juce::String getMidiDeviceName() const { return midiDeviceName; }
    double getSampleRate() const { return sampleRate; }
    int getBufferSize() const { return bufferSize; }
    int getMidiChannel() const { return midiChannel; }
    int getMidiCC() const { return midiCC; }
    int getNumInputChannels() const  { return inputChannels; }
    int getNumOutputChannels() const { return outputChannels; }
    
private:
    juce::String audioDeviceName     { "" };
    juce::String midiDeviceName      { "" };
    double sampleRate     { 0.0 };
    int bufferSize        { 0 };
    int midiChannel       { 0 };
    int midiCC            { 0 };
    int inputChannels     { 0 };
    int outputChannels    { 0 };
};

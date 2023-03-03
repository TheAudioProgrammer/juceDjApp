/*
  ==============================================================================

    MixerDeviceScanner.h
    Created: 22 Jan 2023 3:24:41pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "MixerDeviceList.h"

/* When a user plugs in a USB device, scans through our device list to see if it's listed.  If yes, then updates the settings */
class MixerDeviceScanner : public juce::ChangeListener
{
public:
    MixerDeviceScanner (juce::AudioDeviceManager& m) : deviceManager (m) {}
    
    void changeListenerCallback (juce::ChangeBroadcaster *source) override
    {
        if (source == dynamic_cast<juce::ChangeBroadcaster*>(&deviceManager))
        {
            const auto& deviceTypes = deviceManager.getAvailableDeviceTypes();
            
            for (auto& type : deviceTypes)
            {
                // According to docs, this must be called at least once before calling getDeviceNames()
                type->scanForDevices();
                
                for (const auto& deviceName : type->getDeviceNames())
                {
                    for (auto mixer : mixerDeviceList.deviceSetups)
                        
                        if (deviceName == mixer.getAudioDeviceName())
                        {
                            deviceManager.initialise (mixer.getNumInputChannels(), mixer.getNumOutputChannels(), nullptr, true);
                            deviceManager.setAudioDeviceSetup (mixer.createAudioDeviceSetup(), true);
                        }
                }
            }
        }
    }
    
private:
    MixerDeviceList mixerDeviceList;
    juce::AudioDeviceManager& deviceManager;
};

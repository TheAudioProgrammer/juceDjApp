#pragma once

#include <JuceHeader.h>

// Data
#include "MixerDevice/Data/MixerDeviceScanner.h"
#include "AudioPlayer/Data/AudioPlayerData.h"

// UI
#include "Settings/UI/SettingsUI.h"
#include "AudioPlayer/UI/AudioPlayerUI.h"






//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    // Checks our list of USB devices when a new device is connected
    MixerDeviceScanner deviceScanner;
    
    SettingsUI settingsUI;

    // Player UI is controlling Data
    AudioPlayerData audioPlayerData1;
    AudioPlayerUI audioPlayerUI1 { audioPlayerData1 };
    
    AudioPlayerData audioPlayerData2;
    AudioPlayerUI audioPlayerUI2 { audioPlayerData2 };
    
    bool fileIsLoaded { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

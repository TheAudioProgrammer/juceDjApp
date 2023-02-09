#pragma once

#include <JuceHeader.h>

// Data
#include "MixerDevice/Data/MixerDeviceScanner.h"
#include "AudioPlayer/Data/AudioPlayerProcessor.h"

// UI
#include "Settings/View/SettingsView.h"
#include "AudioPlayer/View/AudioPlayerView.h"






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
    
    SettingsView settingsView;

    // Player UI is controlling Data
    AudioPlayerProcessor playerProcessor1;
    AudioPlayerView playerView1 { playerProcessor1 };
    
    bool fileIsLoaded { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

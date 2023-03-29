#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

// Assets
#include "LookAndFeel/StyleSheet.h"

// Data
#include "MixerDevice/Data/MixerDeviceScanner.h"

// Encapsulated objects (UI, Data & State)
#include "AudioPlayer/AudioPlayer.h"

// Playlist
#include "Playlist/View/PlaylistView.h"

// Toolbar with settings, adding tracks etc
#include "Toolbar/View/Toolbar.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::DragAndDropContainer
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
    
    //==============================================================================

private:
    // Checks our list of USB devices when a new device is connected
    MixerDeviceScanner deviceScanner;
        
    AudioPlayer audioPlayer1;
    
    Playlist playlist;
    
    Toolbar toolbar;
        
    bool fileIsLoaded { false };
    
    juce::LookAndFeel_V4 customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

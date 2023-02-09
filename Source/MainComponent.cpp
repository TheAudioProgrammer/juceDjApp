#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : deviceScanner (deviceManager), settingsView (deviceManager)
{
    setSize (1600, 1200);

    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
    
    // Device manager broadcasts when a new device is connected
    deviceManager.addChangeListener (&deviceScanner);
    
    addAndMakeVisible (settingsView);
    addAndMakeVisible (playerView1);
    //addAndMakeVisible (audioPlayerView2);
}

MainComponent::~MainComponent()
{
    deviceManager.removeAllChangeListeners();
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Assuming this is a stereo setup for each track
    playerProcessor1.prepareToPlay (2 , samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    if (playerProcessor1.getPlayState() == AudioPlayerState::Playing)
        playerProcessor1.processAudio (bufferToFill);
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::dimgrey);
}

void MainComponent::resized()
{
    auto pad = 10;
    
    settingsView.setBounds (10, 10, 100, 50);
    playerView1.setBounds (10, settingsView.getBottom() + pad, 600, 300);
    //audioPlayerUI2.setBounds (audioPlayerUI1.getRight() + pad, settingsUI.getBottom() + pad, 350, 250);
}




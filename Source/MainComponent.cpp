#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : deviceScanner (deviceManager), settingsUI (deviceManager)
{
    setSize (800, 600);

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
    
    addAndMakeVisible (settingsUI);
    addAndMakeVisible (audioPlayerUI1);
    addAndMakeVisible (audioPlayerUI2);
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
    audioPlayerData1.prepareToPlay (2 , samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    if (audioPlayerData1.getPlayState() == AudioPlayerState::Playing)
        audioPlayerData1.processAudio (bufferToFill);
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto pad = 10;
    
    settingsUI.setBounds (10, 10, 100, 50);
    audioPlayerUI1.setBounds (10, settingsUI.getBottom() + pad, 350, 250);
    audioPlayerUI2.setBounds (audioPlayerUI1.getRight() + pad, settingsUI.getBottom() + pad, 350, 250);
}




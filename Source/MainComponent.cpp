#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : deviceScanner (deviceManager), toolbar (deviceManager, xmlPlaylist), playlist (toolbar.getTrackAddState())
{
    setSize (1400, 600);

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
    
    setLookAndFeel (&customLookAndFeel);
    
    // Device manager broadcasts when a new device is connected
    deviceManager.addChangeListener (&deviceScanner);
        
    addAndMakeVisible (audioPlayer1.playerView);
    addAndMakeVisible (audioPlayer1.waveformView);
    addAndMakeVisible (playlist.getComponent());
    addAndMakeVisible (toolbar);
}

MainComponent::~MainComponent()
{
    setLookAndFeel (nullptr);
    deviceManager.removeAllChangeListeners();
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Assuming this is a stereo setup for each track
    audioPlayer1.processor.prepareToPlay (2 , samplesPerBlockExpected, sampleRate);
    audioPlayer1.waveformView.prepare (sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    audioPlayer1.processor.getNextAudioBlock (bufferToFill);
    
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
    
    toolbar.setBounds (10, 10, getWidth() - 20, 70);

    audioPlayer1.playerView.setBounds (10, toolbar.getBottom() + pad, 600, 300);
    audioPlayer1.waveformView.setBounds (audioPlayer1.playerView.getRight() + pad, toolbar.getBottom() + pad, 600, 100);
    playlist.getComponent().setBounds (10,audioPlayer1.playerView.getBottom() + pad, 1200, 400);
}





/*
  ==============================================================================

    AudioPlayerUI.cpp
    Created: 25 Jan 2023 4:12:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioPlayerUI.h"

//==============================================================================
AudioPlayerUI::AudioPlayerUI (AudioPlayerData& p) : audioPlayerData(p)
{
    loadAudioButton.setButtonText ("Load");
    playAudioButton.setButtonText ("Play");
    stopAudioButton.setButtonText ("Stop");
    addAndMakeVisible (loadAudioButton);
    addAndMakeVisible (playAudioButton);
    addAndMakeVisible (stopAudioButton);
    
    loadAudioButton.onClick = [&]()
    {
        audioPlayerData.loadSong();
    };
    
    playAudioButton.onClick = [&]()
    {
        audioPlayerData.setPlayState (AudioPlayerState::Playing);
    };
    
    stopAudioButton.onClick = [&]()
    {
        audioPlayerData.setPlayState (AudioPlayerState::Stopped);
    };
    
    // Values set in dBFS!!
    gainSlider.setRange (-60.0f, 0.0f, 0.01f);
    gainSlider.setValue (0.0f);
    
    gainSlider.onValueChange = [&]()
    {
        audioPlayerData.setDecibelValue (gainSlider.getValue());
    };
    
    addAndMakeVisible (gainSlider);
}

AudioPlayerUI::~AudioPlayerUI()
{
}

void AudioPlayerUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AudioPlayerUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AudioPlayerUI::resized()
{
    auto w = 100;
    auto h = 50;
    auto pad = 10;
    
    loadAudioButton.setBounds (5, 5, w, h);
    playAudioButton.setBounds (loadAudioButton.getRight() + pad, 5, w, h);
    stopAudioButton.setBounds (playAudioButton.getRight() + pad, 5, w, h);
    gainSlider.setBounds (0, loadAudioButton.getBottom() + pad, 50, 150);
}

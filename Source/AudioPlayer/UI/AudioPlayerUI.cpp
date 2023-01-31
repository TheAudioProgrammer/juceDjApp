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
    
    juce::Font f { 24.0f, juce::Font::FontStyleFlags::plain };
    
    songNameLabel.setFont (f);
    songNameLabel.setText ("No Song Loaded", juce::NotificationType::dontSendNotification);
    songNameLabel.setColour (juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible (songNameLabel);
    
    artistNameLabel.setFont (f);
    artistNameLabel.setText ("No Artist", juce::NotificationType::dontSendNotification);
    artistNameLabel.setColour (juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible (artistNameLabel);
    
    songLengthLabel.setFont (f);
    songLengthLabel.setText ("00:00.0", juce::NotificationType::dontSendNotification);
    //songLengthLabel.setColour (juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible (songLengthLabel);
}

AudioPlayerUI::~AudioPlayerUI()
{
}

void AudioPlayerUI::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    
    g.setColour (juce::Colours::white);
    
    juce::Rectangle<float> discArea { 375.0f, static_cast<float>(artistNameLabel.getBottom()) + 25.0f, 180.0f, 180.0f };
    auto discCenter = discArea.getCentre();
    auto diameter { 20 };
    auto radius { diameter / 2 };
    
    g.drawEllipse (discArea, 10.0f);
    g.fillEllipse (discCenter.getX() - radius, discCenter.getY() - radius, diameter, diameter);
    
    g.drawLine (float(discCenter.getX()), float(discCenter.getY()), discArea.getX() + (discArea.getWidth() / 2.0f), discArea.getY(), 10.0f);
}

void AudioPlayerUI::resized()
{
    // Assuming component is 600 x 300
    auto w = 100;
    auto h = 50;
    auto pad = 10;
    auto x = 5;
    auto y = getHeight() - (h + pad);
    auto labelWidth = 500;
    auto labelHeight = 30;

    loadAudioButton.setBounds (x, y, w, h);
    playAudioButton.setBounds (loadAudioButton.getRight() + pad, y, w, h);
    stopAudioButton.setBounds (playAudioButton.getRight() + pad, y, w, h);
    gainSlider.setBounds (x, 70, 50, 150);
    songNameLabel.setBounds (x, 5, labelWidth, labelHeight);
    artistNameLabel.setBounds (x, songNameLabel.getBottom(), labelWidth, labelHeight);
    songLengthLabel.setBounds (275, artistNameLabel.getBottom() + 50, labelWidth / 5, labelHeight);
}

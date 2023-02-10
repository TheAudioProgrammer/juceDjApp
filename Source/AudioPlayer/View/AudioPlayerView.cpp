/*
  ==============================================================================

    AudioPlayerView.cpp
    Created: 25 Jan 2023 4:12:07pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "AudioPlayerView.h"

//==============================================================================
AudioPlayerView::AudioPlayerView (AudioPlayerState& s, AudioMetadata& m)
: state (s)
, metadata (m)
{
    loadAudioButton.setButtonText ("Load");
    playAudioButton.setButtonText ("Play");
    stopAudioButton.setButtonText ("Stop");
    addAndMakeVisible (loadAudioButton);
    addAndMakeVisible (playAudioButton);
    addAndMakeVisible (stopAudioButton);
    
    loadAudioButton.onClick = [&]()
    {
        onLoad();
    };
    
    playAudioButton.onClick = [&]()
    {
        onPlay();
    };
    
    stopAudioButton.onClick = [&]()
    {
        onStop();
    };
    
    // Values set in dBFS!!
    gainSlider.setRange (-60.0f, 0.0f, 0.01f);
    gainSlider.setValue (0.0f);
    
    gainSlider.onValueChange = [&]()
    {
        onGainChange();
    };
    
    addAndMakeVisible (gainSlider);
    
    juce::Font f { 24.0f, juce::Font::FontStyleFlags::plain };
    
    trackNameLabel.setFont (f);
    trackNameLabel.setText ("No track Loaded", juce::NotificationType::dontSendNotification);
    trackNameLabel.setColour (juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible (trackNameLabel);
    
    artistNameLabel.setFont (f);
    artistNameLabel.setText ("No Artist", juce::NotificationType::dontSendNotification);
    artistNameLabel.setColour (juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible (artistNameLabel);
    
    trackLengthLabel.setFont (f);
    trackLengthLabel.setText ("00:00.0", juce::NotificationType::dontSendNotification);
    //trackLengthLabel.setColour (juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    addAndMakeVisible (trackLengthLabel);
    
    state.addChangeListener (this);
}

AudioPlayerView::~AudioPlayerView()
{
    state.removeChangeListener (this);
}

void AudioPlayerView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    
    g.setColour (juce::Colours::white);
    
    juce::Rectangle<float> discArea { 375.0f, static_cast<float>(artistNameLabel.getBottom()) + 25.0f, 180.0f, 180.0f };
    auto discCenter = discArea.getCentre();
    auto radius { discArea.getWidth() / 2.0f };
    
    g.drawEllipse (discArea, 10.0f);
    g.fillEllipse (discCenter.getX() - 10.0f, discCenter.getY() - 10.0f, 20.0f, 20.0f);
    
    g.drawLine (float(discCenter.getX()), float(discCenter.getY()), float(discArea.getX() + radius), discArea.getY(), 10.0f);
    
    trackLengthLabel.setText (metadata.trackCurrentTime, juce::dontSendNotification);
}

void AudioPlayerView::resized()
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
    trackNameLabel.setBounds (x, 5, labelWidth, labelHeight);
    artistNameLabel.setBounds (x, trackNameLabel.getBottom(), labelWidth, labelHeight);
    trackLengthLabel.setBounds (275, artistNameLabel.getBottom() + 50, labelWidth / 5, labelHeight);
}

void AudioPlayerView::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &state)
        update();
}

void AudioPlayerView::update()
{
    trackNameLabel.setText (metadata.trackName, juce::dontSendNotification);
    trackLengthLabel.setText (metadata.trackCurrentTime, juce::dontSendNotification);
}

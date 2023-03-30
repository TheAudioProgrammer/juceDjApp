

#include "Toolbar.h"

Toolbar::Toolbar (juce::AudioDeviceManager& m, XmlPlaylist& xml) : settingsView (m), trackAddView (xml)
{
    addAndMakeVisible (settingsView);
    addAndMakeVisible (trackAddView);
}

void Toolbar::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds());
    g.fillAll (juce::Colours::black);
}

void Toolbar::resized()
{
    auto pad = 10;
    settingsView.setBounds (10, 10, 100, 50);
    trackAddView.setBounds (settingsView.getRight() + pad, 10, 100, 50);
}

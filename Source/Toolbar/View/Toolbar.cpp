

#include "Toolbar.h"

Toolbar::Toolbar (juce::AudioDeviceManager& m) : settingsView (m)
{
    audioFormatManager.registerBasicFormats();
    
    addTracksButton.onClick = [this]()
    {
        songSelector = std::make_unique<juce::FileChooser>("Select a Directory to Add Tracks", juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDesktopDirectory), audioFormatManager.getWildcardForAllFormats());
        
        auto songSelectorFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        
        songSelector->launchAsync (songSelectorFlags, [&] (const juce::FileChooser& chooser)
        {
            xmlPlaylist.addTrackData (chooser.getResult());
        });
    };
    
    addAndMakeVisible (settingsView);
    addAndMakeVisible (addTracksButton);
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
    addTracksButton.setBounds (settingsView.getRight() + pad, 10, 100, 50);
}

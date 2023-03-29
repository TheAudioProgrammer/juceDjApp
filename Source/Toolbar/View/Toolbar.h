

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Settings/View/SettingsView.h"
#include "../../Playlist/Data/XmlPlaylist.h"


class Toolbar : public juce::Component
{
public:
    Toolbar (juce::AudioDeviceManager& m);
    void paint (juce::Graphics& g) override;
    void resized() override;
    
private:
    SettingsView settingsView;
    juce::TextButton addTracksButton { "Add Tracks "};
    std::unique_ptr<juce::FileChooser> songSelector;
    XmlPlaylist xmlPlaylist;
    
    /* Necessary to register and stream audio formats */
    juce::AudioFormatManager audioFormatManager;
};

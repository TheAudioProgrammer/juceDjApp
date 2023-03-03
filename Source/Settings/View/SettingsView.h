/*
  ==============================================================================

    SettingsUI.h
    Created: 30 Jan 2023 2:38:24pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>



class SettingsView : public juce::Component
{
public:
    SettingsView (juce::AudioDeviceManager& deviceManager);
    void paint (juce::Graphics& g) override;
    void resized() override;
    
private:
    // UI
    juce::TextButton settingsButton { "Settings "};
    juce::DialogWindow settingsWindow;
};

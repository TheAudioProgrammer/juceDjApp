/*
  ==============================================================================

    SettingsUI.h
    Created: 30 Jan 2023 2:38:24pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


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

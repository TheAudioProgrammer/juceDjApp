/*
  ==============================================================================

    SettingsView.cpp
    Created: 30 Jan 2023 2:38:24pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "SettingsView.h"

SettingsView::SettingsView (juce::AudioDeviceManager& deviceManager) : settingsWindow ("Settings", juce::Colours::black, true)
{
    settingsButton.setButtonText ("Settings");
    settingsButton.setToggleable (false);
    
    // Creates a new window with our audio device selector component
    settingsButton.onClick = [&]()
    {
        juce::DialogWindow::LaunchOptions windowOptions;
        windowOptions.dialogTitle = "Settings";
        windowOptions.useNativeTitleBar = true;
        windowOptions.content.setOwned (new juce::AudioDeviceSelectorComponent (deviceManager, 2, 8, 2, 8, true, true, false, false));
        windowOptions.content->setSize (800, 600);
        windowOptions.launchAsync();
    };
    
    addAndMakeVisible (settingsButton);
}

void SettingsView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void SettingsView::resized()
{
    settingsButton.setBounds (0, 0, 100, 50);
}

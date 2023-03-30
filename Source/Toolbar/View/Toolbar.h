

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Components/Settings/View/SettingsView.h"
#include "../Components/TrackAdd/View/TrackAddView.h"

class Toolbar : public juce::Component
{
public:
    Toolbar (juce::AudioDeviceManager& m);
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    // TODO: This should probably be cleaned up
    TrackAddState& getTrackAddState() { return trackAddView.getState(); }
    
private:
    SettingsView settingsView;
    TrackAddView trackAddView;
};

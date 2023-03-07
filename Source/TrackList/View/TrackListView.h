
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


class TrackListView : public juce::Component
{
public:
    TrackListView();
    
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    
private:
    juce::TableListBox listBox;
    
};

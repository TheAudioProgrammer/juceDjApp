
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "../../../../Playlist/Data/XmlPlaylist.h"
#include "../State/TrackAddState.h"


class TrackAddView : public juce::Component
{
public:
    TrackAddView (XmlPlaylist& xml);
    void resized() override;
    
    // TODO: State should not reside in a view
    TrackAddState& getState() { return trackAddState; }
    
private:
    juce::TextButton trackAddButton { "Add Tracks "};
    std::unique_ptr<juce::FileChooser> songSelector;
    XmlPlaylist& xmlPlaylist;
    
    /* Necessary to register and stream audio formats */
    juce::AudioFormatManager audioFormatManager;
    
    TrackAddState trackAddState;
};


#pragma once

#include <juce_events/juce_events.h>

struct TrackAddState : public juce::ChangeBroadcaster
{
    bool hasAddedTracks = false;
    
    void justAddedTracks()
    {
        hasAddedTracks = true;
        sendChangeMessage();
    }
};

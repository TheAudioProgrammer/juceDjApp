
#pragma once

#include <juce_core/juce_core.h>

/* Describe all the data we want for our tracks */
struct Metadata
{
    juce::String title { "" };
    juce::String artist { "" };
    juce::String album { "" };
    juce::String year { 0 };
    juce::String genre { "" };
    juce::String comment { "" };
    juce::String currentTime { "" };
    
    // Audio Properties
    juce::String bitrate { "" };
    juce::String sampleRate { "" };
    juce::String channels { "" };
    juce::String length { "" };
    juce::String path { "" };
};

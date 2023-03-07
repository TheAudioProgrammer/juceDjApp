
#include "TrackListView.h"


TrackListView::TrackListView()
{
    listBox.getHeader().addColumn ("Track", 1, 80);
    listBox.getHeader().addColumn ("Artist", 2, 80);
    listBox.getHeader().addColumn ("Length", 3, 80);
    addAndMakeVisible (listBox);
}

void TrackListView::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void TrackListView::resized()
{
    listBox.setBounds (getLocalBounds());
}


#include "TrackListView.h"


TrackListView::TrackListView()
{
    tableHeader = std::make_unique<juce::TableHeaderComponent>();
    tableHeader->addColumn ("Track", 1, 80);
    tableHeader->addColumn ("Artist", 2, 80);
    tableHeader->addColumn ("Length", 3, 80);
    
    
    listBox.setHeader (std::move (tableHeader));
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


#include "TrackAddView.h"


TrackAddView::TrackAddView (XmlPlaylist& xml) : xmlPlaylist (xml)
{
    audioFormatManager.registerBasicFormats();
    
    trackAddButton.onClick = [this]()
    {
        songSelector = std::make_unique<juce::FileChooser>("Select a Directory to Add Tracks", juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDesktopDirectory), audioFormatManager.getWildcardForAllFormats());
        
        auto songSelectorFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        
        songSelector->launchAsync (songSelectorFlags, [&] (const juce::FileChooser& chooser)
        {
            xmlPlaylist.addTrackData (chooser.getResult());
            trackAddState.justAddedTracks();
        });
    };
    
    addAndMakeVisible (trackAddButton);
}

void TrackAddView::resized()
{
    trackAddButton.setBounds (getLocalBounds());
}

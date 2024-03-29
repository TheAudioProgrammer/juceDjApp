#pragma once

#include <juce_core/juce_core.h>
#include "../../Metadata/TagReader.h"


/* Defines header and tracklist information for the app */
class XmlPlaylist
{
public:
    XmlPlaylist();
    bool checkForUserFolderAndPlaylist();
    
    const juce::File& getPlaylistFile();
    
    void addHeaderData();
    void addTrackData (const juce::File& directoryToSearch);
    
private:
    bool checkForUserFolder() const;
    bool createNewXml();
    void addTracks (juce::XmlElement* xmlElement, const juce::File& directoryToSearchForTracks);

    juce::File userFolder { juce::File::getSpecialLocation (juce::File::SpecialLocationType::userMusicDirectory).getChildFile ("TAP DJ App") };
    juce::File playlistFile { userFolder.getChildFile ("Playlist.xml") };
    std::unique_ptr<juce::XmlElement> xml;
};

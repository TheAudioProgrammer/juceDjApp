#pragma once

#include <juce_core/juce_core.h>
#include "../../Metadata/TagReader.h"


/* Defines header and tracklist information for the app */
class XmlPlaylist
{
public:
    /* See if our XML exists in the resources folder...if not, create one!  This is just a temporary location as a proof of concept */
    void createNewXml();
    bool checkForUserFolder();
    bool checkForFile();
    const juce::File& getFile();
    
    void addHeaderData (juce::XmlElement& xml);
    
    // This function will need to be extended, otherwise it would add a new "LIBRARY" to our XML every time we called it.  Just a proof of concept for now
    void addLibraryData (juce::XmlElement& xml, juce::String directoryToSearch);
    
private:
    // Just a proof of concept for now
    juce::String playlistDirectory { "/Users/theaudioprogrammer/Desktop" };
    juce::File userFolder { juce::File::getSpecialLocation (juce::File::SpecialLocationType::userMusicDirectory).getChildFile ("TAP DJ App") };
    juce::File playlistFile { userFolder.getChildFile ("Playlist.xml") };
};

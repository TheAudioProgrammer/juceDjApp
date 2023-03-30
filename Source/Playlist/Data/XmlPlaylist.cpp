
#include "XmlPlaylist.h"


XmlPlaylist::XmlPlaylist()
{
    auto userFolderAndPlaylistExists = checkForUserFolderAndPlaylist();
    jassert (userFolderAndPlaylistExists);
}

bool XmlPlaylist::checkForUserFolderAndPlaylist() 
{
    auto userFolderExists = checkForUserFolder();
    
    jassert (userFolderExists);
    
    if (! playlistFile.existsAsFile())
        return createNewXml();
            
    return playlistFile.existsAsFile();
}

bool XmlPlaylist::createNewXml()
{
    addHeaderData();
    return playlistFile.existsAsFile();
}

bool XmlPlaylist::checkForUserFolder() const
{
    if (! userFolder.exists())
        return userFolder.createDirectory();
    
    return userFolder.isDirectory();
}

const juce::File& XmlPlaylist::getPlaylistFile()
{
    jassert (playlistFile.existsAsFile());
    return playlistFile;
}

void XmlPlaylist::addHeaderData()
{
    juce::XmlElement xmlData ("TABLE_DATA");
    
    auto* headerData = new juce::XmlElement ("HEADERS");
    
    std::vector<std::vector<juce::String>> columns = { { "Title", "400" },
                                                       { "Artist", "400" },
                                                       { "Path"  , "400" }};
    
    for (size_t i = 0; i < columns.size(); i++)
    {
        auto* columnData = new juce::XmlElement ("COLUMN");
        columnData->setAttribute ("columnId", juce::String (i + 1));
        columnData->setAttribute ("name", columns[i][0]);
        columnData->setAttribute ("width", columns[i][1]);
        
        headerData->addChildElement (columnData);        
    }
        
    xmlData.addChildElement (headerData);
    xmlData.writeTo (playlistFile);
}

void XmlPlaylist::addTrackData (const juce::File& directoryToSearch)
{
    if (! playlistFile.existsAsFile())
        checkForUserFolderAndPlaylist();
    
    jassert (checkForUserFolderAndPlaylist());
    
    const juce::String library { "LIBRARY" };
    bool hasLibraryElement { false };
    
    xml = juce::XmlDocument::parse (playlistFile);
    
    for (auto* element : xml->getChildIterator())
    {
        // We already have a library element, so we just need to add tracks to it
        if (element->hasTagName (library))
        {
            addTracks (element, directoryToSearch);
            xml->writeTo (playlistFile);
            hasLibraryElement = true;
        }
    }
    
    // This means we've checked and don't have any of the element we're looking for...
    if (! hasLibraryElement)
    {
        auto* element = new juce::XmlElement (library);
        addTracks (element, directoryToSearch);
        xml->addChildElement (element);
        xml->writeTo (playlistFile);
    }
}

void XmlPlaylist::addTracks (juce::XmlElement* xmlElement, const juce::File& directoryToSearchForTracks)
{
    // Look for all audio files in the directory we give it
    auto tracksInDirectory = directoryToSearchForTracks.findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.mp3;*.wav" );

    // Create the xml from the metadata in each audio file
    for (auto& track : tracksInDirectory)
    {
        auto* fileData = new juce::XmlElement ("AUDIOFILE");
        
        auto metadata = TagReader::getMetadataFromFile (track);
        
        fileData->setAttribute ("Title", metadata.title);
        fileData->setAttribute ("Artist", metadata.artist);
        fileData->setAttribute ("Path", metadata.path);
        
        xmlElement->addChildElement (fileData);
    }
}

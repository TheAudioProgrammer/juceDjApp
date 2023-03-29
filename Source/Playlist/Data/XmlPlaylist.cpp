
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

const juce::File& XmlPlaylist::getFile()
{
    jassert (playlistFile.existsAsFile());
    return playlistFile;
}

void XmlPlaylist::addHeaderData()
{
    juce::XmlElement xmlHeader ("TABLE_DATA");
    
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
    
    xmlHeader.addChildElement (headerData);
    
    xmlHeader.writeTo (playlistFile);
}

void XmlPlaylist::addTrackData (const juce::File& directoryToSearch)
{
    jassert (playlistFile.existsAsFile());
    
    xml = juce::XmlDocument::parse (playlistFile);
    
    auto* libData = new juce::XmlElement ("LIBRARY");
                    
    // Look for all audio files in the directory we give it
    auto tracksInDirectory = directoryToSearch.findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.mp3;*.wav" );
    
    // Create the xml from the metadata in each audio file
    for (auto& track : tracksInDirectory)
    {
        auto* fileData = new juce::XmlElement ("AUDIOFILE");
        
        auto metadata = TagReader::getMetadataFromFile (track);
        
        fileData->setAttribute ("Title", metadata.title);
        fileData->setAttribute ("Artist", metadata.artist);
        fileData->setAttribute ("Path", metadata.path);
        
        libData->addChildElement (fileData);
    }
    
    xml->addChildElement (libData);
    xml->writeTo (playlistFile);
}

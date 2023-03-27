
#include "XmlTrackList.h"

void XmlTrackList::createNewXml()
{
    auto userFolderExists = checkForUserFolder();
    auto playlistFileExists = checkForPlaylistFile();
    
    jassert (userFolderExists && playlistFileExists);
    
    
    juce::XmlElement xml ("TABLE_DATA");
    addHeaderData (xml);
    addLibraryData (xml, trackListDirectory);
    xml.writeTo (playlistFile);
}

bool XmlTrackList::checkForUserFolder()
{
    if (! userFolder.exists())
        return userFolder.createDirectory();
    
    return userFolder.isDirectory();
}

bool XmlTrackList::checkForPlaylistFile()
{
    if (! playlistFile.existsAsFile())
        return playlistFile.create().wasOk();
    
    return playlistFile.existsAsFile();
}

const juce::File& XmlTrackList::getPlaylistFile()
{
    jassert (playlistFile.existsAsFile());
    return playlistFile;
}

void XmlTrackList::addHeaderData (juce::XmlElement& xml)
{
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
    
    xml.addChildElement (headerData);
}

void XmlTrackList::addLibraryData (juce::XmlElement& xml, juce::String directoryToSearch)
{
    auto* libData = new juce::XmlElement ("LIBRARY");
                
    auto directory = juce::File (directoryToSearch);
    
    // Look for all audio files in the directory we give it
    auto tracksInDirectory = directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.mp3;*.wav" );
    
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
    
    xml.addChildElement (libData);
}

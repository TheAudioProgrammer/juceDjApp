
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Metadata/Metadata.h"
#include "../../Metadata/TagReader.h"

class TrackListView;

class XmlEditor
{
public:
    /* See if our XML exists in the resources folder...if not, create one!  This is just a temporary location as a proof of concept */
    void createNewXml()
    {
        // If we need to create a new xml...
        if (xmlDirectory.create())
        {
            juce::XmlElement xml ("TABLE_DATA");
            addHeaderData (xml);
            addLibraryData (xml, trackListDirectory);
            xml.writeTo (xmlDirectory);
        }
    }
    
    void addHeaderData (juce::XmlElement& xml)
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
    
    // This function will need to be extended, otherwise it would add a new "LIBRARY" to our XML every time we called it.  Just a proof of concept for now
    void addLibraryData (juce::XmlElement& xml, juce::String directoryToSearch)
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
    
    
    bool xmlExists()
    {
        return xmlDirectory.exists();
    }
    
    juce::File getXml()
    {
        return xmlDirectory;
    }
    
private:
    std::unique_ptr<juce::FileChooser> directorySelector;
    // Just a proof of concept for now
    juce::File xmlDirectory { "/Users/theaudioprogrammer/Development/JUCE/audioProgrammer/juceDjApp/Source/Resources/Assets/TrackList.xml" };
    juce::String trackListDirectory { "/Users/theaudioprogrammer/Desktop" };
};

class TrackListView : public juce::Component,
                      public juce::TableListBoxModel
{
public:
    TrackListView()
    {
        if (xmlDirectory.exists())
        {
            auto data = juce::XmlDocument::parse (xmlDirectory);
            dataList = data->getChildByName ("LIBRARY");
            columnList = data->getChildByName ("HEADERS");
            
            jassert (columnList);
            jassert (dataList);
                
            if (columnList)
            {
                for (auto* column : columnList->getChildIterator())
                {
                    listBox.getHeader().addColumn (column->getStringAttribute ("name"),
                                                   column->getIntAttribute ("columnId"),
                                                   column->getIntAttribute ("width"));
                }
            }
                
            if (dataList)
                numRows = dataList->getNumChildElements();
        };
        
        directoryLoadButton.onClick = [&]()
        {
            xmlEditor.createNewXml();
        };
        
        listBox.setColour (juce::ListBox::ColourIds::backgroundColourId, juce::Colours::black);
        addAndMakeVisible (listBox);
        addAndMakeVisible (directoryLoadButton);
    }
    
    void loadData (juce::File xml)
    {
        if (! xml.exists())
            return;
    }
    
    int getNumRows() override
    {
        return numRows;
    }
    
    void paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override
    {
        juce::ignoreUnused (rowNumber, width, height, rowIsSelected);
        g.fillAll (juce::Colours::black);
    }
    
    
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override
    {
        juce::ignoreUnused (columnId, rowNumber, width, height, rowIsSelected);
        g.fillAll (juce::Colours::orangered);
    }
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override
    {
        juce::ignoreUnused (isRowSelected);
        
        if (columnId == 1)
        {
            auto* selectionBox = static_cast<TrackListCell*> (existingComponentToUpdate);
    
            if (selectionBox == nullptr)
                selectionBox = new TrackListCell (*this);
    
            selectionBox->setRowAndColumn (rowNumber, columnId);
            return selectionBox;
        }
    
        if (columnId == 2)
        {
            auto* textLabel = static_cast<TrackListCell*> (existingComponentToUpdate);
    
            if (textLabel == nullptr)
                textLabel = new TrackListCell (*this);
    
            textLabel->setRowAndColumn (rowNumber, columnId);
            return textLabel;
        }
        
        if (columnId == 3)
        {
            auto* textLabel = static_cast<TrackListCell*> (existingComponentToUpdate);
    
            if (textLabel == nullptr)
                textLabel = new TrackListCell (*this);
    
            textLabel->setRowAndColumn (rowNumber, columnId);
            return textLabel;
        }
    
        jassert (existingComponentToUpdate == nullptr);
        return nullptr;
    }
    
    juce::String getAttributeNameForColumnId (const int columnId) const
    {
        for (auto* column : columnList->getChildIterator())
        {
            if (column->getIntAttribute ("columnId") == columnId)
                return column->getStringAttribute ("name");
        }
        
        return {};
    }
    
    void setText (const int columnNumber, const int rowNumber, const juce::String& newText)
    {
        const auto& columnName = listBox.getHeader().getColumnName (columnNumber);
        dataList->getChildElement (rowNumber)->setAttribute (columnName, newText);
    }
    
    juce::String getText (const int columnNumber, const int rowNumber)
    {
        if (! dataList)
            return "";
        
        auto numElements = dataList->getNumChildElements();
        
        // Check we're not trying to read into data that doesn't exist...
        if (rowNumber < numElements)
        {
            auto numAttributes = dataList->getChildElement (rowNumber)->getNumAttributes();
            
            if (columnNumber < numAttributes)
            {
                return dataList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
            }
            else
                return "";
        }
        else
            return "";
    }
    
    
    void resized() override
    {
        listBox.setBounds (getLocalBounds());
        //directoryLoadButton.setBounds (10, 40, 100, 50);
    }
    
private:
    juce::TableListBox listBox { "TrackList", this };
    // Just a proof of concept for now
    juce::File xmlDirectory { "/Users/theaudioprogrammer/Development/JUCE/audioProgrammer/juceDjApp/Source/Resources/Assets/TrackList.xml" };
    std::unique_ptr<juce::XmlElement> trackList;
    juce::XmlElement* columnList { nullptr };
    juce::XmlElement* dataList { nullptr };
    juce::TextButton directoryLoadButton;
    XmlEditor xmlEditor;
    int numRows = 0;
    
    class TrackListCell : public juce::Component
    {
    public:
        TrackListCell (TrackListView& tl) : owner (tl)
        {
            myLabel.setEditable (false, true, false);
            addAndMakeVisible (myLabel);
        }

        void setRowAndColumn (const int newRow, const int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            myLabel.setText (owner.getText (columnId, row) , juce::dontSendNotification);
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll (juce::Colours::yellowgreen);
        }

        void resized() override
        {
            myLabel.setBounds (getLocalBounds());
        }

    private:
        juce::Label myLabel { "Blah", "Blah" };
        int row = 0;
        int columnId = 0;
        TrackListView& owner;
    };
};





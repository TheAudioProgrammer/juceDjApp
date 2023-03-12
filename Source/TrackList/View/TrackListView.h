
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Metadata/Metadata.h"
#include "../../Metadata/TagReader.h"

class XmlEditor
{
public:
    /* See if our XML exists in the resources folder...if not, create one!  This is just a temporary location as a proof of concept */
    void createNewXml()
    {
        // If we need to create a new xml...
        if (xmlDirectory.create())
        {
            juce::XmlElement xml ("Library");
                        
            // For now I've set this to the desktop to prove the concept, but eventually we would want to be able to choose any directory, and be able to extend our Xml as we add new directories
            auto directory = juce::File (trackListDirectory);
            
            // Look for all audio files in the directory we give it
            auto tracksInDirectory = directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.mp3;*.wav" );
            
            // Create the xml from the metadata in each audio file
            for (auto& track : tracksInDirectory)
            {
                juce::XmlElement* fileData = new juce::XmlElement ("AudioFile");
                
                auto metadata = TagReader::getMetadataFromFile (track);
                
                fileData->setAttribute ("Title", metadata.title);
                fileData->setAttribute ("Artist", metadata.artist);
                fileData->setAttribute ("Path", metadata.path);
                
                xml.addChildElement (fileData);
            }
            
            xml.writeTo (xmlDirectory);
        }
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


class TrackListCell : public juce::Component
{
public:
    TrackListCell()
    {
        myLabel.setEditable (false, true, false);
        addAndMakeVisible (myLabel);
    }
    
    void setRowAndColumn (const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        myLabel.setText ("Yes", juce::dontSendNotification);
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
};


class TrackListView : public juce::Component,
                      public juce::TableListBoxModel
{
public:
    TrackListView()
    {
        const auto callback = [this]()
        {
            if (xmlDirectory.exists())
            {
                auto data = juce::XmlDocument::parse (xmlDirectory);
                dataList = data->getChildByName ("AudioFile");
                columnList = data->getChildByName ("Library");
                
                if (columnList)
                {
                        listBox.getHeader().addColumn ("Track", 1, 80);
                        listBox.getHeader().addColumn ("Artist", 2, 80);
                        listBox.getHeader().addColumn ("Length", 3, 80);
                }
            }
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
        //g.fillAll (juce::Colours::black);
    }
    
    
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override
    {
        g.fillAll (juce::Colours::orangered);
    }
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override
    {
        if (columnId == 1)
        {
            auto* selectionBox = static_cast<TrackListCell*> (existingComponentToUpdate);
    
            if (selectionBox == nullptr)
                selectionBox = new TrackListCell();
    
            selectionBox->setRowAndColumn (rowNumber, columnId);
            return selectionBox;
        }
    
        if (columnId == 2)
        {
            auto* textLabel = static_cast<TrackListCell*> (existingComponentToUpdate);
    
            if (textLabel == nullptr)
                textLabel = new TrackListCell();
    
            textLabel->setRowAndColumn (rowNumber, columnId);
            return textLabel;
        }
    
        jassert (existingComponentToUpdate == nullptr);
        return nullptr;
    }
    
    void paint (juce::Graphics& g) override
    {
        
    }
    
    void resized() override
    {
        listBox.setBounds (getLocalBounds());
        directoryLoadButton.setBounds (10, 40, 100, 50);
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
};





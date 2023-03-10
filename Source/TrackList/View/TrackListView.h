
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


class XmlEditor
{
public:
    void createNewXml()
    {
        // See if our XML exists in the resources folder...if not, create one!  This is just a temporary location as a proof of concept
        auto newXml = juce::File ("/Users/theaudioprogrammer/Development/JUCE/audioProgrammer/juceDjApp/Source/Resources/Assets/TrackList.xml");
        
        auto success = newXml.create();
        
        if (success)
        {
            juce::XmlElement library ("Library");
                        
            auto directory = juce::File ("/Users/theaudioprogrammer/Desktop");
            auto fileList = directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.mp3;*.wav" );
            
            for (int i = 0; i < fileList.size(); i++)
            {
                juce::XmlElement* metadata = new juce::XmlElement ("AudioFile");
                metadata->setAttribute ("Track", "Some Song");
                metadata->setAttribute ("Artist", "My Favorite DJ");
                metadata->setAttribute ("Path", fileList[i].getFullPathName());
                library.addChildElement (metadata);
            }
            
            library.writeTo (newXml);
                        
//            directorySelector = std::make_unique<juce::FileChooser>("Select a directory for tracks", juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDesktopDirectory));
//
//            auto songSelectorFlags = juce::FileBrowserComponent::canSelectDirectories;
//
//            directorySelector->launchAsync (songSelectorFlags, [&] (const juce::FileChooser& chooser)
//            {
//                auto fileList = chooser.getResult().findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.mp3;*.wav");
//
//                for (int i = 0; i < fileList.size(); i++)
//                {
//                    auto metadata = std::make_unique<juce::XmlElement>("AudioFile");
//                    metadata->setAttribute ("Track", "Some Song");
//                    metadata->setAttribute ("Artist", "My Favorite DJ");
//                    metadata->setAttribute ("Path", fileList[i].getFullPathName());
//                    metadata->setAttribute ("Type", fileList[i].getFileExtension());
//                    library.addChildElement (metadata.get());
//                }
//            });
            
            library.writeTo (newXml);
        }
    }
    
private:
    std::unique_ptr<juce::FileChooser> directorySelector;
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
        listBox.getHeader().addColumn ("Track", 1, 80);
        listBox.getHeader().addColumn ("Artist", 2, 80);
        listBox.getHeader().addColumn ("Length", 3, 80);
        
        listBox.setColour (juce::ListBox::ColourIds::backgroundColourId, juce::Colours::black);
        addAndMakeVisible (listBox);
        
        directoryLoadButton.onClick = [&]()
        {
            xmlEditor.createNewXml();
        };
        
        addAndMakeVisible (directoryLoadButton);
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
    juce::TextButton directoryLoadButton;
    XmlEditor xmlEditor;
    int numRows = 0;
};





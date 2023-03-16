
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../Data/XmlTrackList.h"
#include "../../Metadata/Metadata.h"



class TrackListView : public juce::Component,
                      public juce::TableListBoxModel
{
public:
    TrackListView();
    void loadData (juce::File xmlDir);
    
    // From TableListBoxmodel
    int getNumRows() override;
    
    // These two functions don't apply since we're using a custom component that's created by refreshComponentForCell
    void paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    // Creates new components that hold the data from our XML table
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    
    // Helper
    juce::String getAttributeNameForColumnId (const int columnId) const;
    
    // Getting & updating data
    void setText (const int columnNumber, const int rowNumber, const juce::String& newText);
    juce::String getText (const int columnNumber, const int rowNumber);
    
    
    void resized() override;
    
private:
    juce::TableListBox listBox { "TrackListBox", this };
    // Just a proof of concept for now
    juce::File xmlDirectory { "/Users/theaudioprogrammer/Development/JUCE/audioProgrammer/juceDjApp/Source/Resources/Assets/TrackList.xml" };
    std::unique_ptr<juce::XmlElement> xmlData;
    juce::XmlElement* headerList { nullptr };
    juce::XmlElement* trackList { nullptr };
    juce::TextButton directoryLoadButton;
    XmlTrackList xmlTrackList;
    int numRows = 0;
    
    class TrackListCell : public juce::Component
    {
    public:
        TrackListCell (TrackListView& tl) : owner (tl)
        {
            myLabel.setEditable (false, true, false);
            addAndMakeVisible (myLabel);
        }

        void setColumnAndRow (const int newColumn, const int newRow)
        {
            columnId = newColumn;
            row = newRow;
            myLabel.setText (owner.getText (columnId, row) , juce::dontSendNotification);
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll (juce::Colours::darkgrey);
        }

        void resized() override
        {
            myLabel.setBounds (getLocalBounds());
        }

    private:
        juce::Label myLabel { "", "" };
        int row = 0;
        int columnId = 0;
        TrackListView& owner;
    };
};





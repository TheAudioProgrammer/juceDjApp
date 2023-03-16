
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
    int getColumnAutoSizeWidth (int columnId) override;
    void paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    
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
    XmlTrackList xmlTrackList;
    int numRows = 0;
    juce::Font font { 14.0f };

    
    class TrackListCell : public juce::Label
    {
    public:
        TrackListCell (TrackListView& tl) : owner (tl)
        {
            setEditable (false, true, false);
        }
        
        void mouseDown (const juce::MouseEvent& event) override
        {
            owner.listBox.selectRowsBasedOnModifierKeys (row, event.mods, false);
            juce::Label::mouseDown (event);
        }
        
        void textWasEdited() override
        {
            owner.setText (columnId, row, getText());
        }

        void setColumnAndRow (const int newColumn, const int newRow)
        {
            columnId = newColumn;
            row = newRow;
            setText (owner.getText (columnId, row) , juce::dontSendNotification);
        }

    private:
        int row = 0;
        int columnId = 0;
        TrackListView& owner;
    };
};





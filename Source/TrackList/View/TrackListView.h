
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Data/XmlTrackList.h"
#include "../../Metadata/Metadata.h"


class SampleComponent : public juce::Component,
public juce::TextDragAndDropTarget
//public juce::FileDragAndDropTarget,
//public juce::TextDragAndDropTarget
{
//    void mouseEnter (const juce::MouseEvent& event) override
//    {
//        DBG ("Enter");
//    }
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::orange);
    }
    
//    void itemDragEnter (const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override
//    {
//        DBG ("bang");
//    }
//
//    bool isInterestedInDragSource (const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override
//    {
//        return true;
//    }
//
//    void itemDropped (const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override
//    {
//        DBG ("drop");
//    }
    
    bool isInterestedInTextDrag (const juce::String& text) override
    {
        DBG (text);
        return true;
    }
    
    void textDropped (const juce::String &text, int x, int y) override
    {
        DBG (text);
    }
};

class PlayListView : public juce::TableListBox
{
public:
    using juce::TableListBox::TableListBox;
    
    juce::var getDragSourceDescription (const juce::SparseSet<int>& selectedRows) override
    {
        // for our drag description, we'll just make a comma-separated list of the selected row
        // numbers - this will be picked up by the drag target and displayed in its box.
        juce::StringArray rows;

        for (int i = 0; i < selectedRows.size(); ++i)
        {
            rows.add (juce::String (selectedRows[i] + 1));
            DBG (juce::String (selectedRows[i] + 1));
        }

        return rows.joinIntoString (", ");
    }
    
};

class TrackListView : public juce::Component,
                      public juce::DragAndDropContainer,
                      public juce::TableListBoxModel
{
public:
    TrackListView();
    ~TrackListView() override;
    void loadData (juce::File& xmlDir);
    
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
    PlayListView listBox { "TrackListBox", this };
    // Just a proof of concept for now
    juce::File xmlDirectory { "/Users/theaudioprogrammer/Development/JUCE/audioProgrammer/juceDjApp/Source/Resources/Assets/TrackList.xml" };
    std::unique_ptr<juce::XmlElement> xmlData;
    juce::XmlElement* headerList { nullptr };
    juce::XmlElement* trackList { nullptr };
    XmlTrackList xmlTrackList;
    int numRows = 0;
    juce::Font font { 14.0f };
    SampleComponent sample;

    
    class TrackListCell : public juce::Label,
                          public juce::DragAndDropContainer
    {
    public:
        TrackListCell (TrackListView& tl) : owner (tl)
        {
            setEditable (false, true, false);
        }
        
        void mouseDown (const juce::MouseEvent& event) override
        {
            owner.listBox.selectRowsBasedOnModifierKeys (row, event.mods, false);
            path = owner.getText (3, row);
            juce::Label::mouseDown (event);
        }
        
        void mouseDrag (const juce::MouseEvent& event) override
        {
            startDragging (path, this);
            juce::Label::mouseDrag (event);
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

        juce::String path { "" };
    private:
        int row = 0;
        int columnId = 0;
        TrackListView& owner;
    };
};





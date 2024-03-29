
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Metadata/Metadata.h"
#include "../Data/XmlPlaylist.h"
#include "../../Toolbar/Components/TrackAdd/State/TrackAddState.h"


class Playlist : public juce::TableListBoxModel,
                 public juce::ChangeListener
{
public:
    Playlist (TrackAddState& t);
    ~Playlist() override;
    
    void loadData (const juce::File& xmlDir);
    
    // From TableListBoxModel
    int getNumRows() override;
    int getColumnAutoSizeWidth (int columnId) override;
    void paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    
    juce::String getAttributeNameForColumnId (const int columnId) const;
        
    // Getting & updating data
    void setText (const int columnNumber, const int rowNumber, const juce::String& newText);
    juce::String getText (const int columnNumber, const int rowNumber);
    
    juce::var getDragSourceDescription (const juce::SparseSet<int>& selectedRows) override
    {
        juce::ignoreUnused (selectedRows);
        return getText (3, currentRow);
    }
    
    juce::Component& getComponent() { return listBox; }
    
    // From Change Listener
    void changeListenerCallback (juce::ChangeBroadcaster* b) override;
        
private:
    juce::TableListBox listBox { "Playlist", this };
    std::unique_ptr<juce::XmlElement> xmlData;
    juce::XmlElement* headerList { nullptr };
    juce::XmlElement* trackList { nullptr };
    XmlPlaylist xmlPlaylist;
    int numRows = 0;
    int currentRow = 0;
    juce::Font font { 14.0f };
    TrackAddState& trackState;
    
    // TODO: This should be global, static, or otherwise
    juce::File userFolder { juce::File::getSpecialLocation (juce::File::SpecialLocationType::userMusicDirectory).getChildFile ("TAP DJ App") };
    juce::File playlistFile { userFolder.getChildFile ("Playlist.xml") };
    std::unordered_set<juce::String> headerAttributeList;


    
    class TrackListCell : public juce::Label
    {
    public:
        TrackListCell (Playlist& tl) : owner (tl)
        {
            setEditable (false, true, false);
            setInterceptsMouseClicks (false, false);
        }
        
        void mouseDown (const juce::MouseEvent& event) override
        {
            owner.listBox.selectRowsBasedOnModifierKeys (row, event.mods, false);
            juce::Label::mouseDown (event);
            //path = owner.getText (3, row);
        }
        
        void mouseDrag (const juce::MouseEvent& event) override
        {
            //owner.startDraggingComponent (row, columnId, event);
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
        Playlist& owner;
    };
};
    



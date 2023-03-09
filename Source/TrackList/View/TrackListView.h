
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


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
                      public juce::TableListBoxModel,
                      public juce::ChangeListener
{
public:
    TrackListView()
    {
        listBox.getHeader().addColumn ("Track", 1, 80);
        listBox.getHeader().addColumn ("Artist", 2, 80);
        listBox.getHeader().addColumn ("Length", 3, 80);
        
        listBox.setColour (juce::ListBox::ColourIds::backgroundColourId, juce::Colours::black);
        addAndMakeVisible (listBox);
        
    }
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        DBG ("Change Called!");
        refreshComponentForCell (1, 1, false, nullptr);
//        refreshComponentForCell (2, 1, false, nullptr);
        listBox.updateContent();
        DBG (getNumRows());
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
    }
    
    
private:
    juce::TableListBox listBox { "TrackList", this };
    int numRows = 0;
};





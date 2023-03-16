
#include "TrackListView.h"

TrackListView::TrackListView()
{
    loadData (xmlDirectory);
            
    numRows = trackList->getNumChildElements();
            
//        directoryLoadButton.onClick = [&]()
//        {
//            xmlTrackList.createNewXml();
//        };
    
    listBox.setColour (juce::ListBox::ColourIds::backgroundColourId, juce::Colours::black);
    addAndMakeVisible (listBox);
    addAndMakeVisible (directoryLoadButton);
}

void TrackListView::loadData (juce::File xmlDir)
{
    jassert (xmlDir.exists());
    xmlData = juce::XmlDocument::parse (xmlDir);
    trackList = xmlData->getChildByName ("LIBRARY");
    headerList = xmlData->getChildByName ("HEADERS");
        
    jassert (headerList);
    jassert (trackList);
        
    if (headerList)
    {
        for (auto* column : headerList->getChildIterator())
        {
            listBox.getHeader().addColumn (column->getStringAttribute ("name"),
                                           column->getIntAttribute ("columnId"),
                                           column->getIntAttribute ("width"));
        }
    }
}

int TrackListView::getNumRows()
{
    return numRows;
}

// Since we're using a custom component this doesn't apply
void TrackListView::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    juce::ignoreUnused (g, rowNumber, width, height, rowIsSelected);
    g.fillAll (juce::Colours::darkgrey);
}

void TrackListView::paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) 
{
    juce::ignoreUnused (g, columnId, rowNumber, width, height, rowIsSelected);
    g.fillAll (juce::Colours::orangered);
}

juce::Component* TrackListView::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    juce::ignoreUnused (isRowSelected);
    
    if (columnId == 1)
    {
        auto* selectionBox = static_cast<TrackListCell*> (existingComponentToUpdate);

        if (selectionBox == nullptr)
            selectionBox = new TrackListCell (*this);

        selectionBox->setColumnAndRow (columnId, rowNumber);
        return selectionBox;
    }

    if (columnId == 2)
    {
        auto* textLabel = static_cast<TrackListCell*> (existingComponentToUpdate);

        if (textLabel == nullptr)
            textLabel = new TrackListCell (*this);

        textLabel->setColumnAndRow (columnId, rowNumber);
        return textLabel;
    }

    if (columnId == 3)
    {
        auto* textLabel = static_cast<TrackListCell*> (existingComponentToUpdate);

        if (textLabel == nullptr)
            textLabel = new TrackListCell (*this);

        textLabel->setColumnAndRow (columnId, rowNumber);
        return textLabel;
    }

    jassert (existingComponentToUpdate == nullptr);
    return nullptr;
}

juce::String TrackListView::getAttributeNameForColumnId (const int columnId) const
{
    for (auto* column : headerList->getChildIterator())
    {
        if (column->getIntAttribute ("columnId") == columnId)
            return column->getStringAttribute ("name");
    }
    
    return {};
}

void TrackListView::setText (const int columnNumber, const int rowNumber, const juce::String& newText)
{
    const auto& columnName = listBox.getHeader().getColumnName (columnNumber);
    trackList->getChildElement (rowNumber)->setAttribute (columnName, newText);
}

juce::String TrackListView::getText (const int columnNumber, const int rowNumber)
{
    return trackList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
}

void TrackListView::resized() 
{
    listBox.setBounds (getLocalBounds());
    //directoryLoadButton.setBounds (10, 40, 100, 50);
}

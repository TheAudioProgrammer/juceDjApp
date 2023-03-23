
#include "TrackListView.h"

TrackListView::TrackListView()
{
    if (! xmlDirectory.exists())
        xmlTrackList.createNewXml();
    
    loadData (xmlDirectory);
            
    numRows = trackList->getNumChildElements();
    listBox.setColour (juce::ListBox::ColourIds::backgroundColourId, juce::Colours::black);
    addAndMakeVisible (listBox);
    addAndMakeVisible (sample);
}

TrackListView::~TrackListView()
{
    listBox.setModel (nullptr);
}

void TrackListView::loadData (juce::File& xmlDir)
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

int TrackListView::getColumnAutoSizeWidth (int columnId)
{
    if (columnId == 9)
        return 50;

    int widest = 32;

    for (auto i = getNumRows(); --i >= 0;)
    {
        if (auto* rowElement = trackList->getChildElement (i))
        {
            auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));

            widest = juce::jmax (widest, font.getStringWidth (text));
        }
    }

    return widest + 8;
}

void TrackListView::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    juce::ignoreUnused (width, height);
    
    auto alternateColour = getLookAndFeel().findColour (juce::ListBox::backgroundColourId)
                                           .interpolatedWith (getLookAndFeel().findColour (juce::ListBox::textColourId), 0.03f);
    if (rowIsSelected)
        g.fillAll (juce::Colours::dimgrey);
    else if (rowNumber % 2)
        g.fillAll (alternateColour);
    
    g.setColour (juce::Colours::white.withAlpha (0.2f));
    g.drawRect (getLocalBounds());
}

void TrackListView::paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) 
{
    g.setColour (rowIsSelected ? juce::Colours::darkblue : getLookAndFeel().findColour (juce::ListBox::textColourId));
    g.setFont (font);

    if (auto* rowElement = trackList->getChildElement (rowNumber))
    {
        auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
        g.drawText (text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }

    g.setColour (getLookAndFeel().findColour (juce::ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
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
    sample.setBounds (10, 100, 100, 50);
}

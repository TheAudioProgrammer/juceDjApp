
#include "PlaylistView.h"

Playlist::Playlist()
{
    // TODO: XML Tracklist should be changed to PlaylistXml
    xmlPlaylist.createNewXml();
    loadData (xmlPlaylist.getFile());
}

Playlist::~Playlist()
{
    listBox.setModel (nullptr);
}

void Playlist::loadData (const juce::File& xmlDir)
{
    jassert (xmlDir.exists());
    
    xmlData = juce::XmlDocument::parse (xmlDir);
    trackList = xmlData->getChildByName ("LIBRARY");
    headerList = xmlData->getChildByName ("HEADERS");
    numRows = trackList->getNumChildElements();
        
    if (headerList)
    {
        for (auto* column : headerList->getChildIterator())
        {
            listBox.getHeader().addColumn (column->getStringAttribute ("name"),
                                           column->getIntAttribute ("columnId"),
                                           column->getIntAttribute ("width"));
        }
    }
        
    jassert (headerList);
    jassert (trackList);
}

int Playlist::getNumRows()
{
    return numRows;
}

int Playlist::getColumnAutoSizeWidth (int columnId)
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

void Playlist::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    juce::ignoreUnused (width, height);

    if (rowIsSelected)
        g.fillAll (juce::Colours::dimgrey);
    else if (rowNumber % 2)
        g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white.withAlpha (0.2f));
    g.fillRect (width - 1, 0, 1, height);
}

void Playlist::paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour (rowIsSelected ? juce::Colours::darkblue : juce::Colours::white);
    g.setFont (font);

    if (auto* rowElement = trackList->getChildElement (rowNumber))
    {
        auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
        g.drawText (text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }

    g.setColour (juce::Colours::black);
    g.fillRect (width - 1, 0, 1, height);
}

juce::Component* Playlist::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    if (isRowSelected == true)
        currentRow = rowNumber;
    
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

juce::String Playlist::getAttributeNameForColumnId (const int columnId) const
{
    for (auto* column : headerList->getChildIterator())
    {
        if (column->getIntAttribute ("columnId") == columnId)
            return column->getStringAttribute ("name");
    }
    
    return {};
}

void Playlist::setText (const int columnNumber, const int rowNumber, const juce::String& newText)
{
    const auto& columnName = listBox.getHeader().getColumnName (columnNumber);
    trackList->getChildElement (rowNumber)->setAttribute (columnName, newText);
}

juce::String Playlist::getText (const int columnNumber, const int rowNumber)
{
    return trackList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
}



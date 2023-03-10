
#pragma once

#include <juce_core/juce_core.h>
#include "Metadata.h"
#include <fileref.h>
#include <tag.h>


/* Read the metadata of an audio file and store it in a Metadata struct */
class TagReader
{
public:
    static Metadata getMetadataFromFile (const juce::File& musicFile)
    {
        Metadata metadata;
        TagLib::FileRef tagReader (musicFile.getFullPathName().toUTF8());
        
        if (! tagReader.isNull() && tagReader.tag())
        {
            TagLib::Tag* tag = tagReader.tag();
            
            metadata.title = juce::String (tag->title().toCString());
            metadata.artist = juce::String (tag->artist().toCString());
            metadata.album = juce::String (tag->album().toCString());
            metadata.year = juce::String (tag->year());
            metadata.genre = juce::String (tag->genre().toCString());
            metadata.comment = juce::String (tag->comment().toCString());
            
            if(tagReader.audioProperties())
            {
                TagLib::AudioProperties *properties = tagReader.audioProperties();
                
                metadata.bitrate = juce::String (properties->bitrate());
                metadata.sampleRate = juce::String (properties->sampleRate());
                metadata.channels = juce::String (properties->channels());
                
                auto seconds = juce::String (properties->length() % 60);
                auto minutes = juce::String ((properties->length() - seconds.getIntValue()) / 60);
                
                metadata.length = minutes + ":" + seconds;
                
                metadata.path = musicFile.getFullPathName();
            }
        }
        else
        {
            metadata.title = musicFile.getFileNameWithoutExtension();
            metadata.artist = "Unknown";
        }
        
        return metadata;
    }
};

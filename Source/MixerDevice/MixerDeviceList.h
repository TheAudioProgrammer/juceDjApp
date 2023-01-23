/*
  ==============================================================================

    MixerDeviceList.h
    Created: 22 Jan 2023 3:24:30pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MixerDevice.h"


/* Holds our list of mixers */
struct MixerDeviceList
{
    MixerDeviceList()
    {
        deviceSetups.push_back (pioneerDJMS9);
    }
    
    std::vector<MixerDevice> deviceSetups;
    MixerDevice pioneerDJMS9 { "DJM-S9", "Pioneer DJ DJM-S9", 48000.0, 64, 7, 31, 4, 8 };
};

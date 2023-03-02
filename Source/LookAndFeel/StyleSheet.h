/*
  ==============================================================================

    StyleSheet.h
    Created: 26 Feb 2023 1:35:30pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../Resources/Resources.h"

namespace StyleSheet
{

static juce::Typeface::Ptr boldFont  { juce::Typeface::createSystemTypefaceFor (Resources::WorkSansSemiBold_ttf, Resources::WorkSansSemiBold_ttfSize) };
static juce::Typeface::Ptr plainFont { juce::Typeface::createSystemTypefaceFor (Resources::WorkSansRegular_ttf, Resources::WorkSansRegular_ttfSize) };

};

class customLookandFeel : public juce::LookAndFeel_V4
{
    
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Bilifans.h"

//==============================================================================
/**
*/
class FansAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FansAudioProcessorEditor (FansAudioProcessor&);
    ~FansAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    void setLabel(int mid);
    void closeButtonPressed();
    void Setting(bool native);
    bool keyPressed(const juce::KeyPress& key);
    //==============================================================================
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FansAudioProcessor& audioProcessor;
    juce::Label name{ {},"Name:" };
    juce::Label fans{ {},"Fans:" };
    juce::StretchableLayoutManager stretchableManager;
    juce::TextButton button{ "..." };
    juce::LookAndFeel_V4 look;
    juce::Font font;
    juce::TextEditor input{};
    juce::Image image;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FansAudioProcessorEditor)
};

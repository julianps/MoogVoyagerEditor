#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Utils.h"

class MoogVoyagerAudioProcessorEditor  : public AudioProcessorEditor,
                                         private MoogVoyagerAudioProcessor::SliderListener,
                                         public Timer
{
private:
    struct midiData {
        int MSB;
        int LSB;
        int decimal;
    } cutoff, lfoRate;

public:
    MoogVoyagerAudioProcessorEditor (MoogVoyagerAudioProcessor&);
    ~MoogVoyagerAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    Utils utils;
    int lfoRateSliderValue, cutoffSliderValue;
    int midiInChannel = 0, midiOutChannel = 0;
    bool isUpdating;

    Slider lfoRateSlider, cutoffFrequencySlider;
    ComboBox midiInChooser, midiOutChooser;
    Label lfoRateLabel, cutoffFrequencyLabel, midiInChooserLabel, midiOutChooserLabel;
    
    void initializeMidiChannelChooser();
    void initializeLfoRateSlider();
    void initializeCutoffFrequencySlider();
    
    void handleNewSliderValue (int, int) override;
    void timerCallback() override;

    MoogVoyagerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogVoyagerAudioProcessorEditor)
};

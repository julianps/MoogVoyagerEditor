#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyParameters.h"

class MoogVoyagerAudioProcessorEditor  : public AudioProcessorEditor,
                                         private MoogVoyagerAudioProcessor::SliderListener,
                                         public Timer
{
private:
    struct MidiData {
        int MSB;
        int LSB;
        int decimal;
    };

    MoogVoyagerAudioProcessor& processor;

    MidiData cutoff, lfoRate, resonance;
    
    MyParameters &parameters;
public:
    MoogVoyagerAudioProcessorEditor (MoogVoyagerAudioProcessor&);
    ~MoogVoyagerAudioProcessorEditor();
    void paint (Graphics&) override;
    void resized() override;
private:
    int midiInChannel = 0, midiOutChannel = 0;
    ComboBox midiInChooser, midiOutChooser;
    
    TextEditor logBox;
    void addMessageToList(const String);

    const StringArray MIDI_CHANNEL_LIST = StringArray::fromTokens("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16", false);

    int lfoRateSliderValue, cutoffSliderValue, resonanceSliderValue;
    bool isUpdating;

    Slider lfoRateSlider, cutoffFrequencySlider, resonanceSlider;
    Label lfoRateLabel, cutoffFrequencyLabel, midiInChooserLabel, midiOutChooserLabel, resonanceLabel;
    
    void initializeMidiChannelChooser();
    void initializeLfoRateSlider();
    void initializeResonanceSlider();
    void initializeCutoffFrequencySlider();
    
    void sendCutoffToDevice(int);
    void sendLfoRateToDevice(int);
    void sendResonanceToDevice(int);
    
    void handleNewSliderValue (int, int) override;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogVoyagerAudioProcessorEditor)
};

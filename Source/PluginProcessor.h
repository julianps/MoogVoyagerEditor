#pragma once

#include <JuceHeader.h>
#include "MyParameters.h"
#include "MoogVoyagerConstants.h"

class MoogVoyagerAudioProcessor  : public AudioProcessor
{
private:
    int midiInChannel = 1;
    int midiOutChannel = 1;
    double startTime;
    double sampleRate = 44100.0;
    int previousSampleNumber = 0;

public:

    MoogVoyagerAudioProcessor();
    ~MoogVoyagerAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    struct SliderListener
    {
        virtual ~SliderListener() {}
        virtual void handleNewSliderValue (int, int) = 0;
    };
    void addSliderListener    (SliderListener& listener) { sliderListeners.add    (&listener); }
    void removeSliderListener (SliderListener& listener) { sliderListeners.remove (&listener); }

    struct ParameterListener
    {
        virtual ~ParameterListener() {}
        virtual void handleNewParameterValue (int, int) = 0;
    };
    void addParameterListener    (ParameterListener& listener) { parameterListeners.add    (&listener); }
    void removeParameterListener (ParameterListener& listener) { parameterListeners.remove (&listener); }

    
    void sendMidiCCMessage(int controllerNumber, int value);
    void addMessageToBuffer (const MidiMessage& message);
    
    void setMidiOutChannel(int channelNumber);
    void setMidiInChannel(int channelNumber);
    
private:
    ListenerList<SliderListener> sliderListeners;
    ListenerList<ParameterListener> parameterListeners;

    MidiBuffer midiOutputMessages;
    
    
public:
    // Plugin's AudioProcessorValueTreeState, and its associated UndoManager
    AudioProcessorValueTreeState valueTreeState;
    UndoManager undoManager;
    MyParameters parameters;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoogVoyagerAudioProcessor)
};

#include "PluginProcessor.h"
#include "PluginEditor.h"

MoogVoyagerAudioProcessor::MoogVoyagerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), startTime (Time::getMillisecondCounterHiRes() * 0.001)
#endif
{
}

MoogVoyagerAudioProcessor::~MoogVoyagerAudioProcessor()
{
}

const String MoogVoyagerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MoogVoyagerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MoogVoyagerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MoogVoyagerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MoogVoyagerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MoogVoyagerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MoogVoyagerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MoogVoyagerAudioProcessor::setCurrentProgram (int index)
{
}

const String MoogVoyagerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MoogVoyagerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MoogVoyagerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void MoogVoyagerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MoogVoyagerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MoogVoyagerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    buffer.clear();
    int time;
    MidiMessage m;
    MidiBuffer processedMidi;
    
    // Handles new INCOMING midi messages.
    for (MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if(m.isController()){
            sliderListeners.call ([=] (SliderListener& l){
                l.handleNewSliderValue(m.getControllerNumber(), m.getControllerValue());
            });
        }
    }
    
    //Handles new OUTGOING midi messages.
    if(!midiOutputMessages.isEmpty())
    {
        midiMessages.swapWith(midiOutputMessages);
        midiOutputMessages.clear();
    }
}

bool MoogVoyagerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MoogVoyagerAudioProcessor::createEditor()
{
    return new MoogVoyagerAudioProcessorEditor (*this);
}

void MoogVoyagerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void MoogVoyagerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoogVoyagerAudioProcessor();
}

void MoogVoyagerAudioProcessor::sendMidiCCMessage(int controllerNumber, int value)
{
    auto message = MidiMessage::controllerEvent(midiOutChannel, controllerNumber, value);
    addMessageToBuffer(message);
}

void MoogVoyagerAudioProcessor::setMidiInChannel(int channelNumber)
{
    this->midiInChannel = channelNumber;
}

void MoogVoyagerAudioProcessor::setMidiOutChannel(int channelNumber)
{
    this->midiOutChannel = channelNumber;
}

void MoogVoyagerAudioProcessor::addMessageToBuffer (const MidiMessage& message)
{
    auto timestamp = message.getTimeStamp();
    auto sampleNumber =  (int) (timestamp * sampleRate);
    midiOutputMessages.addEvent (message, sampleNumber);
}



#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MoogVoyagerConstants.h"
#include "Utils.h"

MoogVoyagerAudioProcessorEditor::MoogVoyagerAudioProcessorEditor (MoogVoyagerAudioProcessor &p)
    : AudioProcessorEditor (&p),
    processor (p),
    parameters(p.parameters)
{
    startTimer(100);
    setSize (1000, 600);
   
    parameters.attachControls(lfoRateSlider);

    processor.addSliderListener(*this);
    initializeResonanceSlider();
    initializeLfoRateSlider();
    initializeCutoffFrequencySlider();
    initializeMidiChannelChooser();

//    addAndMakeVisible (logBox);
//    logBox.setMultiLine (true);
//    logBox.setReturnKeyStartsNewLine (true);
//    logBox.setReadOnly (true);
//    logBox.setScrollbarsShown (true);
//    logBox.setCaretVisible (false);
//    logBox.setPopupMenuEnabled (true);
//    logBox.setColour (TextEditor::backgroundColourId, Colour (0x32ffffff));
//    logBox.setColour (TextEditor::outlineColourId, Colour (0x1c000000));
//    logBox.setColour (TextEditor::shadowColourId, Colour (0x16000000));
}

MoogVoyagerAudioProcessorEditor::~MoogVoyagerAudioProcessorEditor()
{
    parameters.detachControls();
    processor.removeSliderListener(*this);
}

void MoogVoyagerAudioProcessorEditor::timerCallback(){
    if(!isUpdating){
        if(lfoRateSlider.getValue() != lfoRateSliderValue){
            lfoRateSlider.setValue(lfoRateSliderValue);
        }
        if(cutoffFrequencySlider.getValue() != cutoffSliderValue){
            cutoffFrequencySlider.setValue(cutoffSliderValue);
        }
        if(resonanceSlider.getValue() != resonanceSliderValue){
            resonanceSlider.setValue(resonanceSliderValue);
        }
    }
}

void MoogVoyagerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MoogVoyagerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    midiInChooser.setBounds(area.withWidth(80).withHeight(30).withX(70).withY(0));
    midiOutChooser.setBounds(area.withWidth(80).withHeight(30).withX(250).withY(0));
   
//    logBox.setBounds(area.withWidth(500).withHeight(getHeight()-250).withX(400).withY(10));

    lfoRateSlider.setBounds(area.withWidth(80).withHeight(getHeight()-130).withX(10).withY(100));
    cutoffFrequencySlider.setBounds(area.withWidth(80).withHeight(getHeight()-130).withX(100).withY(100));
    resonanceSlider.setBounds(area.withWidth(80).withHeight(getHeight()-130).withX(190).withY(100));
}

void MoogVoyagerAudioProcessorEditor::initializeMidiChannelChooser()
{
    midiInChooserLabel.setText ("Midi-IN", dontSendNotification);
    midiOutChooserLabel.setText ("Midi-OUT", dontSendNotification);
    midiInChooserLabel.attachToComponent(&midiInChooser, true);
    midiOutChooserLabel.attachToComponent(&midiOutChooser, true);
    midiInChooser.addItemList(MIDI_CHANNEL_LIST, 1);
    midiInChooser.setSelectedId(5);
    addAndMakeVisible(midiInChooser);
    midiOutChooser.addItemList(MIDI_CHANNEL_LIST, 1);
    midiOutChooser.setSelectedId(5);
    addAndMakeVisible(midiOutChooser);
    midiInChooser.onChange = [this]
    {
        if(!isUpdating){
            processor.setMidiInChannel(midiInChooser.getSelectedId());
        }
    };
    midiOutChooser.onChange = [this]
    {
        if(!isUpdating){
            processor.setMidiOutChannel(midiOutChooser.getSelectedId());
        }
    };
}

void MoogVoyagerAudioProcessorEditor::initializeLfoRateSlider()
{
    lfoRateLabel.setText (MyParameters::lfoRate_Name, dontSendNotification);
    lfoRateLabel.attachToComponent (&lfoRateSlider, false );
    lfoRateSlider.setSliderStyle(Slider::LinearBarVertical);
    lfoRateSlider.setColour(Slider::backgroundColourId, Colours::yellow);
    lfoRateSlider.setRange(Utils::MIDI_CC_MIN_VALUE, Utils::MIDI_CC_MAX_VALUE);
    lfoRateSlider.setTextBoxStyle(Slider::TextBoxRight, true, 50,50);
    lfoRateSlider.setTextBoxIsEditable(false);
    lfoRateSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(&lfoRateSlider);
    cutoffFrequencySlider.setValue(1);
    lfoRateSlider.onValueChange = [this]
    {
        if(!isUpdating) {
            lfoRateSliderValue = lfoRateSlider.getValue();
            sendLfoRateToDevice(lfoRateSliderValue);
        }
    };
}


void MoogVoyagerAudioProcessorEditor::initializeCutoffFrequencySlider()
{
    cutoffFrequencyLabel.setText ("Cutoff", dontSendNotification);
    cutoffFrequencyLabel.attachToComponent (&cutoffFrequencySlider, false );
    cutoffFrequencySlider.setSliderStyle(Slider::LinearBarVertical);
    cutoffFrequencySlider.setColour(Slider::backgroundColourId, Colours::yellow);
    cutoffFrequencySlider.setRange(Utils::MIDI_CC_MIN_VALUE, Utils::MIDI_CC_MAX_VALUE);
    cutoffFrequencySlider.setTextBoxStyle(Slider::TextBoxRight, true, 50,50);
    cutoffFrequencySlider.setTextBoxIsEditable(false);
    cutoffFrequencySlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(&cutoffFrequencySlider);
    cutoffFrequencySlider.setValue(1);
    cutoffFrequencySlider.onValueChange = [this]
    {
        if(!isUpdating) {
            cutoffSliderValue = cutoffFrequencySlider.getValue();
            sendCutoffToDevice(cutoffSliderValue);
        }
    };
}

void MoogVoyagerAudioProcessorEditor::initializeResonanceSlider()
{
    resonanceLabel.setText("Resonance", dontSendNotification);
    resonanceLabel.attachToComponent(&resonanceSlider, false);
    resonanceSlider.setSliderStyle(Slider::LinearBarVertical);
    resonanceSlider.setColour(Slider::backgroundColourId, Colours::yellow);
    resonanceSlider.setRange(0, 127);
    resonanceSlider.setTextBoxStyle(Slider::TextBoxRight, true, 50, 50);
    resonanceSlider.setTextBoxIsEditable(true);
    resonanceSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(&resonanceSlider);
    resonanceSlider.setValue(0);
    resonanceSlider.onValueChange = [this]
    {
        if(!isUpdating)
        {
            resonanceSliderValue = resonanceSlider.getValue();
            sendResonanceToDevice(resonanceSliderValue);
        }
    };
}

// -----------------------------
// Incomming Midi Processing
// - The processor calls all Slider-Listener with an updated value.
// -----------------------------

void MoogVoyagerAudioProcessorEditor::handleNewSliderValue(int controlNumber, int controlValue)
{

    isUpdating = true;
    switch(controlNumber){
        case MoogVoyagerConstants::CC_CUTOFF_MSB :
            cutoff.MSB = controlValue;
            cutoff.decimal = cutoff.MSB * 128 + cutoff.LSB;
            cutoffSliderValue = cutoff.decimal;
            break;
        case MoogVoyagerConstants::CC_CUTOFF_LSB :
            cutoff.LSB = controlValue;
            cutoff.decimal = cutoff.MSB * Utils::DEFAULT_BYTE_SIZE + cutoff.LSB;
            cutoffSliderValue = cutoff.decimal;
            break;
        case MoogVoyagerConstants::CC_LFO_RATE_MSB :
            lfoRate.MSB = controlValue;
            lfoRate.decimal = lfoRate.MSB *Utils::DEFAULT_BYTE_SIZE + lfoRate.LSB;
            lfoRateSliderValue = lfoRate.decimal;
            break;
        case MoogVoyagerConstants::CC_LFO_RATE_LSB :
            lfoRate.LSB = controlValue;
            lfoRate.decimal = lfoRate.MSB *Utils::DEFAULT_BYTE_SIZE + lfoRate.LSB;
            lfoRateSliderValue = lfoRate.decimal;
            break;
        case MoogVoyagerConstants::CC_RESONANCE_MSB :
            resonance.MSB = controlValue;
            resonance.decimal = controlValue;
            resonanceSliderValue = resonance.decimal;
            break;
        case MoogVoyagerConstants::CC_RESONANCE_LSB :
            resonance.LSB = controlValue;
            resonance.decimal = controlValue;
            resonanceSliderValue = resonance.decimal;
            break;

    }
    isUpdating = false;
}


// ---------------------------
// Outgoing Midi Processing
// - send Midi Messages to the Processor, to let him take care of sending it to the device
// ---------------------------

void MoogVoyagerAudioProcessorEditor::sendLfoRateToDevice(int value)
{
    lfoRate.MSB = Utils::toMsb(value);
    lfoRate.LSB = Utils::toLsb(value);
    lfoRate.decimal = lfoRateSliderValue;
    processor.sendMidiCCMessage(MoogVoyagerConstants::CC_LFO_RATE_MSB, lfoRate.MSB);
    processor.sendMidiCCMessage(MoogVoyagerConstants::CC_LFO_RATE_LSB, lfoRate.LSB);
}

void MoogVoyagerAudioProcessorEditor::sendCutoffToDevice(int value){
    cutoff.decimal = cutoffSliderValue;
    cutoff.MSB = Utils::toMsb(value);
    cutoff.LSB = Utils::toLsb(value);
    processor.sendMidiCCMessage(MoogVoyagerConstants::CC_CUTOFF_MSB, cutoff.MSB);
    processor.sendMidiCCMessage(MoogVoyagerConstants::CC_CUTOFF_LSB, cutoff.LSB);
}

void MoogVoyagerAudioProcessorEditor::sendResonanceToDevice(int value)
{
    resonance.decimal = resonanceSliderValue;
    resonance.MSB = Utils::toMsb(value);
    resonance.LSB = Utils::toLsb(value);
    processor.sendMidiCCMessage(MoogVoyagerConstants::CC_RESONANCE_MSB, resonance.MSB);
    processor.sendMidiCCMessage(MoogVoyagerConstants::CC_RESONANCE_LSB, resonance.LSB);
}

// Logging to internal Text Fields
void MoogVoyagerAudioProcessorEditor::addMessageToList (const String m)
{
    logBox.moveCaretToEnd();
    logBox.insertTextAtCaret (m + newLine);
}

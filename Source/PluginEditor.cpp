#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.h"
#include "MoogVoyagerConstants.h"

MoogVoyagerAudioProcessorEditor::MoogVoyagerAudioProcessorEditor (MoogVoyagerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    startTimer(100);
    setSize (1000, 600);
    processor.addSliderListener(*this);
    initializeLfoRateSlider();
    initializeCutoffFrequencySlider();
    initializeMidiChannelChooser();
}

MoogVoyagerAudioProcessorEditor::~MoogVoyagerAudioProcessorEditor()
{
    processor.removeSliderListener(*this);
}

void MoogVoyagerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MoogVoyagerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    lfoRateSlider.setBounds(area
                              .withWidth(80)
                              .withHeight(getHeight()-130)
                              .withX(10)
                              .withY(100));
    
    cutoffFrequencySlider.setBounds(area
                                    .withWidth(80)
                                    .withHeight(getHeight()-130)
                                    .withX(100)
                                    .withY(100));
    midiInChooser.setBounds(area
                            .withWidth(80)
                            .withHeight(30)
                            .withX(70)
                            .withY(0));
    midiOutChooser.setBounds(area
                            .withWidth(80)
                            .withHeight(30)
                            .withX(250)
                            .withY(0));

}

void MoogVoyagerAudioProcessorEditor::initializeMidiChannelChooser()
{
    midiInChooserLabel.setText ("Midi-IN", dontSendNotification);
    midiInChooserLabel.attachToComponent(&midiInChooser, true);
    midiOutChooserLabel.setText ("Midi-OUT", dontSendNotification);
    midiOutChooserLabel.attachToComponent(&midiOutChooser, true);
    midiInChooser.addItemList(utils.MIDI_CHANNEL_LIST, 1);
    midiInChooser.setSelectedId(5);
    addAndMakeVisible(midiInChooser);
    midiOutChooser.addItemList(utils.MIDI_CHANNEL_LIST, 1);
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
    lfoRateLabel.setText ("LFO-Rate", dontSendNotification);
    lfoRateLabel.attachToComponent (&lfoRateSlider, false );
    lfoRateSlider.setSliderStyle(Slider::LinearBarVertical);
    lfoRateSlider.setColour(Slider::backgroundColourId, Colours::yellow);
    lfoRateSlider.setRange(utils.midiCCMinValue, utils.midiCCMaxValue);
    lfoRateSlider.setTextBoxStyle(Slider::TextBoxRight, true, 50,50);
    lfoRateSlider.setTextBoxIsEditable(false);
    lfoRateSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(&lfoRateSlider);
    lfoRateSlider.setValue(1);
    lfoRateSlider.onValueChange = [this]
    {
        if(!isUpdating) {
            
            lfoRateSliderValue = lfoRateSlider.getValue();
            lfoRate.MSB = lfoRateSliderValue / 128;
            lfoRate.LSB = lfoRateSliderValue % 128;
            lfoRate.decimal = lfoRateSliderValue;
            processor.sendMidiCCMessage(MoogVoyagerConstants::CC_LFO_RATE_MSB, lfoRate.MSB);
            processor.sendMidiCCMessage(MoogVoyagerConstants::CC_LFO_RATE_LSB, lfoRate.LSB);
        }
    };
}

void MoogVoyagerAudioProcessorEditor::initializeCutoffFrequencySlider()
{
    cutoffFrequencyLabel.setText ("Cutoff", dontSendNotification);
    cutoffFrequencyLabel.attachToComponent (&cutoffFrequencySlider, false );
    cutoffFrequencySlider.setSliderStyle(Slider::LinearBarVertical);
    cutoffFrequencySlider.setColour(Slider::backgroundColourId, Colours::yellow);
    cutoffFrequencySlider.setRange(utils.midiCCMinValue, utils.midiCCMaxValue);
    cutoffFrequencySlider.setTextBoxStyle(Slider::TextBoxRight, true, 50,50);
    cutoffFrequencySlider.setTextBoxIsEditable(false);
    cutoffFrequencySlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(&cutoffFrequencySlider);
    cutoffFrequencySlider.setValue(1);
    cutoffFrequencySlider.onValueChange = [this]
    {
        if(!isUpdating) {
            cutoffSliderValue = cutoffFrequencySlider.getValue();
            cutoff.MSB = cutoffSliderValue / 128;
            cutoff.LSB = cutoffSliderValue % 128;
            cutoff.decimal = cutoffSliderValue;
            processor.sendMidiCCMessage(MoogVoyagerConstants::CC_CUTOFF_MSB, cutoff.MSB);
            processor.sendMidiCCMessage(MoogVoyagerConstants::CC_CUTOFF_LSB, cutoff.LSB);
            //        Logger::getCurrentLogger()->outputDebugString("------------------------------------------");
            //        Logger::getCurrentLogger()->outputDebugString("Slider: " + std::to_string(cutoffSliderValue));
            //        Logger::getCurrentLogger()->outputDebugString("struct.VALUE: " + std::to_string(cutoff_decimal));
            //        Logger::getCurrentLogger()->outputDebugString("struct.MSB: " + std::to_string(cutoff_MSB));
            //        Logger::getCurrentLogger()->outputDebugString("struct.LSB: " + std::to_string(cutoff_LSB));
        }
    };
}


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
            cutoff.decimal = cutoff.MSB * 128 + cutoff.LSB;
            cutoffSliderValue = cutoff.decimal;
            break;
        case MoogVoyagerConstants::CC_LFO_RATE_MSB :
            lfoRate.MSB = controlValue;
            lfoRate.decimal = lfoRate.MSB * 128 + lfoRate.LSB;
            lfoRateSliderValue = lfoRate.decimal;
            break;
        case MoogVoyagerConstants::CC_LFO_RATE_LSB :
            lfoRate.LSB = controlValue;
            lfoRate.decimal = lfoRate.MSB * 128 + lfoRate.LSB;
            lfoRateSliderValue = lfoRate.decimal;
            break;
    }
    isUpdating = false;
//    Logger::getCurrentLogger()->outputDebugString("MSB: " + std::to_string(cutoff_MSB));
//    Logger::getCurrentLogger()->outputDebugString("LSB: " + std::to_string(cutoff_LSB));
//    Logger::getCurrentLogger()->outputDebugString("value: " + std::to_string(cutoff_decimal));

}


void MoogVoyagerAudioProcessorEditor::timerCallback(){
    if(!isUpdating){
        if(lfoRateSlider.getValue() != lfoRateSliderValue){
            lfoRateSlider.setValue(lfoRateSliderValue);
        }
        if(cutoffFrequencySlider.getValue() != cutoffSliderValue){
            cutoffFrequencySlider.setValue(cutoffSliderValue);
        }
    }
}


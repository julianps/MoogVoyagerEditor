#pragma once

#ifndef Parameters_h
#define Parameters_h

#include <JuceHeader.h>
#include "MoogVoyagerConstants.h"
#include <stdio.h>

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

struct MyParameters
{
public:
    static const String lfoRate_Id, lfoRate_Name, lfoRate_Label;
    
    MyParameters(AudioProcessorValueTreeState& vts);
   
    void createAllParameters();
    void detachControls();
    void attachControls(Slider& lfoRateSlider);
    
    // Actual working parameter values
    int lfoRate;

private:
    AudioProcessorValueTreeState& valueTreeState;
    SliderAttachment* lfoRateAttachment;
    
    struct IntegerListener : public AudioProcessorValueTreeState::Listener
    {
        int& workingValue;
        
        IntegerListener(int& wv) : AudioProcessorValueTreeState::Listener(), workingValue(wv) {}
        void parameterChanged(const String&, float newValue) override
        {
            workingValue = (int)newValue;
        }
    };
    
    IntegerListener lfoRateListener;
};

#endif

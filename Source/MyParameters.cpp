#include "MyParameters.h"
#include "MoogVoyagerConstants.h"

const String MyParameters::lfoRate_Id = "lfoRate";
const String MyParameters::lfoRate_Name = "LFO Rate";


MyParameters::MyParameters(AudioProcessorValueTreeState& vts)
    : valueTreeState(vts),
    lfoRateAttachment(nullptr),
    lfoRateListener(lfoRate)
{
    lfoRate = 0;
}

void MyParameters::createAllParameters()
{
    valueTreeState.addParameterListener(lfoRate_Id, &lfoRateListener);
}

void MyParameters::detachControls()
{
    if (lfoRateAttachment != nullptr)
    {
        delete lfoRateAttachment;
        lfoRateAttachment = nullptr;
    }
}

void MyParameters::attachControls(Slider& lfoRateSlider)
{
    lfoRateAttachment = new SliderAttachment(valueTreeState, lfoRate_Id, lfoRateSlider);
}

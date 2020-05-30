//
//  Utils.h
//  Moog Voyager
//
//  Created by Julian Stücker on 22.05.20.
#include <JuceHeader.h>

//  Copyright © 2020 jps. All rights reserved.
//

#ifndef Utils_h
#define Utils_h


class Utils
{
public:
    static const int midiCCMinValue = 0;
    static const int midiCCMaxValue = 16384;
    Utils():MIDI_CHANNEL_LIST(StringArray::fromTokens("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16", false)){}
    
    const StringArray MIDI_CHANNEL_LIST;

    uint8 toMsb(int value)
    {
        Logger::getCurrentLogger()->outputDebugString("Incomming: " + std::to_string(value));
        if(value <= 0){
            return 0;
        }
        return value / 128;
    };
    
    uint8 toLsb(int value)
    {
        if(value <= 0){
            return 0;
        }
        return value % 128;
    };
};


#endif /* Utils_h */

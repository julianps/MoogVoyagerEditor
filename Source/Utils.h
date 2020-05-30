//  Utils.h
//  Created by Julian Stücker on 22.05.20.
//  Copyright © 2020 jps. All rights reserved.
#include <JuceHeader.h>

#ifndef Utils_h
#define Utils_h

class Utils
{
public:
    // Const values ------
    static const int DEFAULT_BYTE_SIZE = 128;
    static const int MIDI_CC_MIN_VALUE = 0;
    static const int MIDI_CC_MAX_VALUE = 16384;

    // helper functions -----
    static uint8 toMsb(int value)
    {
        if(value <= 0)
        {
            return 0;
        }
        else
        {
            return value / 128;
        }
    };
    
    static uint8 toLsb(int value)
    {
        if(value <= 0)
        {
            return 0;
        }
        else
        {
            return value % 128;
        }
    };
};

#endif /* Utils_h */

#pragma once

#ifndef Utils_h
#define Utils_h

struct Utils
{
public:
// Const values ------
    static const int DEFAULT_BYTE_SIZE = 128;
    static const int MIDI_CC_MIN_VALUE = 0;
    static const int MIDI_CC_MAX_VALUE = 16384;
    static const int MIDI_CC_DEFAULT_STEP_SIZE = 1;

// midi helper functions -----
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
    
    static uint8 toDecimal(int MSB, int LSB)
    {
            return MSB * DEFAULT_BYTE_SIZE + LSB;
    };
};

#endif

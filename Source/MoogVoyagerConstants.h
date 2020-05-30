//
//  MoogVoyagerConstants.h
//  Moog Voyager
//
//  Created by Julian Stücker on 22.05.20.
//  Copyright © 2020 jps. All rights reserved.
//

#ifndef MoogVoyagerConstants_h
#define MoogVoyagerConstants_h

class MoogVoyagerConstants
{
public:
    // Cutoff
    static const int CC_CUTOFF_MSB = 19;
    static const int CC_CUTOFF_LSB = 51;

    // Filter Resonance
    static const int CC_RESONANCE_MSB = 21;
    static const int CC_RESONANCE_LSB = 53;
    
    // LFO Rate
    static const int CC_LFO_RATE_MSB = 3;
    static const int CC_LFO_RATE_LSB = 35;
    
    // MOD Wheel
    static const int CC_MOD_WHEEL_MSB = 1;
    static const int CC_MOD_WHEEL_LSB = 33;
};

#endif /* MoogVoyagerConstants_h */

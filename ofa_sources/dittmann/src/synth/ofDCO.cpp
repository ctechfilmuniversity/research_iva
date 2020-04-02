//
//  ofDCO.cpp
//  audio_04_osc
//
//  Created by abrennec on 31.10.19.
//

#include "ofMathConstants.h"

#include "synth/ofDCO.h"


ofDCO::ofDCO()
{
    updatePhaseIncrement();
};

void ofDCO::setup(const float& smplRate, const float& freq, const float& ampl) {
    
    sampleRate = smplRate;
    frequency = freq;
    amplitude = ampl;
    
    updatePhaseIncrement();
}

void ofDCO::updatePhaseIncrement() {
    
    phaseInc = frequency * TWO_PI / sampleRate;
}

void ofDCO::updatePhase() {
    
    phase += phaseInc;
    
    if (phase >= TWO_PI) {
        
        phase -= TWO_PI;
    }
}


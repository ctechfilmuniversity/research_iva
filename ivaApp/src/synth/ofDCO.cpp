//
//  ofDCO.cpp
//  audio_04_osc
//
//  Created by abrennec on 31.10.19.
//

#include "ofMathConstants.h"

#include "synth/ofDCO.h"


ofDCO::ofDCO(OscillatorType type)
: dcoType{type}
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

float ofDCO::generate() {
    
    float currentSample{};
    
    switch (dcoType) {
            
        case SINE:
            currentSample = generateSine();
            break;
        case SAWTOOTH:
            currentSample = generateSawTooth();
            break;
        case TRIANGLE:
            currentSample = generateTriangle();
            break;
        case PULSE:
            currentSample = generatePulse();
            break;
    }
    
    return currentSample;
}


float ofDCO::generateSine() {
    
    auto currentSample = amplitude * sin(phase * phaseMod);
    
    //phase += phaseInc;
    
    updatePhase();
    
    return currentSample;
}

float ofDCO::generateTriangle() {
    
    // val = ( 2.0 * (phase * (1 / TWO_PI) ) - 1.0;
    // if (val <= 0.0)
    //      val = -val;
    // val = 2.0 * (val - 0.5);
    
    auto phaseTmp = phase * phaseMod;
    
    auto currentSample = ( 2.0 * (phaseTmp * (1.0 / TWO_PI) ) ) - 1.0;
    
    if (currentSample <= 0.0)
        currentSample *= -1.0;
    
    currentSample = 2.0 * (currentSample - 0.5);
    
    currentSample *= amplitude;
    
   
    updatePhase();
    
    return currentSample;
}


float ofDCO::generateSawTooth() {
    
    // val = ( 2.0 * (phase * (1 / TWO_PI) ) - 1.0;
    
    auto phaseTmp = phase * phaseMod;
    
    auto currentSample = ( 2.0 * (phaseTmp * (1.0 / TWO_PI) ) ) - 1.0;
    currentSample *= amplitude;
    
    //phase += phaseInc;
    
    updatePhase();
    
    return currentSample;
}


float ofDCO::generatePulse() {
    
    auto currentSample = sin(phase * phaseMod) > 0 ? 1. : -1.;
    currentSample *= amplitude;
    
    //phase += phaseInc;
    
    updatePhase();
    
    return currentSample;
}

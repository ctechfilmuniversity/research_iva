//
//  ofDCOBaseTypes.cpp
//  audio_04_osc
//
//  Created by abrennec on 31.10.19.
//

#include "ofDCOBaseTypes.h"
#include "ofMathConstants.h"

ofDCOSine::ofDCOSine()
: ofDCO()
{

}

float ofDCOSine::generate() {
    
    auto currentSample = amplitude * sin(phase * phaseMod);
    
    updatePhase();
    
    return currentSample;
}


// -----------------

ofDCOTriangle::ofDCOTriangle()
: ofDCO()
{

}

float ofDCOTriangle::generate() {
    
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


// -----------------

ofDCOSawTooth::ofDCOSawTooth()
: ofDCO()
{
    updatePhaseIncrement();
}

float ofDCOSawTooth::generate() {
    
    // val = ( 2.0 * (phase * (1 / TWO_PI) ) - 1.0;
    
    auto phaseTmp = phase * phaseMod;
    
    auto currentSample = ( 2.0 * (phaseTmp * (1.0 / TWO_PI) ) ) - 1.0;
    currentSample *= amplitude;
    
    //phase += phaseInc;
    
    updatePhase();
    
    return currentSample;
}


// -----------------

ofDCOPulse::ofDCOPulse()
: ofDCO()
{
}

float ofDCOPulse::generate() {
    
    auto currentSample = sin(phase * phaseMod) > 0 ? 1. : -1.;
    currentSample *= amplitude;
    
    //phase += phaseInc;
    
    updatePhase();
    
    return currentSample;
}



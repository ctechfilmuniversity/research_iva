//
//  ofDCO.h
//  audio_04_osc
//
//  Created by abrennec on 31.10.19.
//
//  Reference: Aaron Anderson's ofxATK addon
//

#pragma once

#include <iostream>

// Digitally controlled oscillator (DCO)

class ofDCO {

    
public:
    
    ofDCO();
    virtual ~ofDCO(){};
    
        
    void setup(const float& smplRate, const float& freq, const float& ampl);
    void setFrequency(const float& freq);
    void setAmplitude(const float& amp);
    void setPhase(const float& phs);
    void setSampleRate(const float& rate);

    float getFrequency() const;
    float getAmplitude() const;
    float getPhase() const;
    float getSampleRate() const;
    
protected:
    
    void updatePhaseIncrement();
    void updatePhase();

    float frequency{220.0};
    float amplitude{1.0};
    float phase{0.0};
    float phaseInc{0.0};
    float sampleRate{44100.0};
    
};

inline void ofDCO::setFrequency(const float &freq) {
    
    frequency = freq;
    updatePhaseIncrement();
}

inline void ofDCO::setAmplitude(const float &amp) {
    amplitude = amp;
}

inline void ofDCO::setPhase(const float &phs) {
    phase = phs;
}

inline void ofDCO::setSampleRate(const float &rate) {
    
    sampleRate = rate;
    updatePhaseIncrement();
}

inline float ofDCO::getFrequency() const {
    return frequency;
}

inline float ofDCO::getAmplitude() const {
    return amplitude;
}

inline float ofDCO::getPhase() const {
    return phase;
}

inline float ofDCO::getSampleRate() const {
    return sampleRate;
}

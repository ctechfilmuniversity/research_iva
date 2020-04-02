//
//  ofSynth.h
//  audio_03_synth
//
//  Created by abrennec on 16.10.19.
//

#pragma once

#include "ofMain.h"
#include "synth/ofDCOBaseTypes.h"

class ofSoundBuffer;

class ofSynth {
    
public:
    
    ofSynth(int numOsc);
    ~ofSynth(){};
    
    void updateSoundBuffer(ofSoundBuffer& buffer);

    float getFrequency(bool isFundamental = false) const;
    void setFrequency(int oscNum, const float& freq);
    void setAmplitude(int oscNum, const float& amp);
    float getAmplitude(int oscNum) const;
    
    void setSampleRate(const float& smplRate);
    float getSampleRate() const;
    
private:
  
    void setupOscillators(int numOsc);
    void updateOscillatorFreq(const float& freq);
    
    const float fundamentalFreq{172.5};
    float currentFreq{172.5};
    float sampleRate{44100.0};
    float currentAmp{0.5};
    
    // TODO: refactor the management of the oscillators
    // Use a vector of type "shared_ptr<ofDCO>" to manage the different
    // oscillator types and access them through an iterator
    // - std::vector<shared_ptr<ofDCO>> oscillatorsVec;
    // Adapt the functions setAmplitude and getAmplitude accordingly
    // as well as all functions that use the individual oscillators
    
    vector<ofDCOSine> sineOsc;
    
    ofDCOSine sineWaveOscill{};
    ofDCOSawTooth sawToothOscill{};
    ofDCOTriangle triangleOscill{};
    
    ofDCOPulse sineWavePulse{};
};


// inline can improve performance but this is not ensured
// more info at https://isocpp.org/wiki/faq/inline-functions
inline
float ofSynth::getFrequency(bool isFundamental) const {
    
    return ( isFundamental ? fundamentalFreq : currentFreq );
}

inline
float ofSynth::getAmplitude(int oscNum) const {
    
    return sineOsc[oscNum].getAmplitude();
}

inline
float ofSynth::getSampleRate() const {
    
    return sampleRate;
}

inline
void ofSynth::setSampleRate(const float& smplRate) {
    
    sampleRate = smplRate;
}

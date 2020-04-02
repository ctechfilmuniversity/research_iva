//
//  ofSynth.h
//  audio_03_synth
//
//  Created by abrennec on 16.10.19.
//

#pragma once

#include "synth/ofDCOBaseTypes.h"

class ofSoundBuffer;

class ofSynth {
    
public:
    
    ofSynth();
    ~ofSynth(){};
    
    void fillSoundBuffer(ofSoundBuffer& buffer);

    float getFrequency(bool isFundamental = false) const;
    void setFrequency(const float& freq);
    void increaseFrequency();
    void decreaseFrequency();
    
    void setSampleRate(const float& smplRate);
    float getSampleRate() const;
    
private:
  
    void setupOscillators();
    void updateOscillatorFreq(const float& freq);
    
    
    //float rms{};
    const float fundamentalFreq{172.5};
    float currentFreq{172.5};
    float sampleRate{44100.0};
    
    //std::shared_ptr<ofDCO> oscillator{};
    ofDCOSine sineWaveOscillator1{};
    ofDCOSine sineWaveOscillator2{};
    ofDCOSine sineWaveOscillator3{};
    ofDCOSine sineWaveOscillator4{};
    ofDCOPulse sineWavePulse1{};
    ofDCOSine sineWavePulse2{};
    ofDCOSawTooth sawToothOscill{};
    ofDCOTriangle triangleOscill{};
};


// inline can improve performance but this is not ensured
// more info at https://isocpp.org/wiki/faq/inline-functions
inline
float ofSynth::getFrequency(bool isFundamental) const {
    
    return ( isFundamental ? fundamentalFreq : currentFreq );
}

inline
float ofSynth::getSampleRate() const {
    
    return sampleRate;
}

inline
void ofSynth::setSampleRate(const float& smplRate) {
    
    sampleRate = smplRate;
}

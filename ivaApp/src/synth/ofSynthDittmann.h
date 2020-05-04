//
//  ofSynth.h
//  audio_03_synth
//
//  Created by abrennec on 16.10.19.
//

#pragma once

#include "synth/ofDCOBaseTypes.h"
#include <vector>

class ofSoundBuffer;

class ofSynthDittmann
{
  public:
    ofSynthDittmann();
    ~ofSynthDittmann(){};

    void updateSoundBuffer(ofSoundBuffer& buffer);

    void setSampleRate(const float& smplRate);
    float getSampleRate() const;

    std::vector<ofDCOSine> oscillators;

  private:
    void setupOscillators();
    void updateOscillatorFreq(const float& freq);
    float sampleRate{44100.0};
};

inline float ofSynthDittmann::getSampleRate() const
{
    return sampleRate;
}

inline void ofSynthDittmann::setSampleRate(const float& smplRate)
{
    sampleRate = smplRate;
}

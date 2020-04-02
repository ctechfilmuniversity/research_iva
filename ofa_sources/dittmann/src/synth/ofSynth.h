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

class ofSynth
{
  public:
    ofSynth();
    ~ofSynth(){};

    void updateSoundBuffer(ofSoundBuffer& buffer);

    void setSampleRate(const float& smplRate);
    float getSampleRate() const;

    std::vector<ofDCOSine> oscillators;

  private:
    void setupOscillators();
    void updateOscillatorFreq(const float& freq);
    float sampleRate{44100.0};
};

inline float ofSynth::getSampleRate() const
{
    return sampleRate;
}

inline void ofSynth::setSampleRate(const float& smplRate)
{
    sampleRate = smplRate;
}

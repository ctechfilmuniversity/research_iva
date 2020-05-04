//
//  ofSynthDittmann.cpp
//  audio_03_synth
//
//  Created by abrennec on 16.10.19.
//

#include "ofSynthDittmann.h"
#include "ofMath.h"
#include "ofMathConstants.h"
#include "ofSoundBuffer.h"
#include <array>

ofSynthDittmann::ofSynthDittmann()
{
    setupOscillators();
}

void ofSynthDittmann::updateSoundBuffer(ofSoundBuffer& outBuffer)
{
    // this loop builds a buffer of audio containing 3 sine waves at different
    // frequencies, and pulses the volume of each sine wave individually. In
    // other words, 4 oscillators and 2 LFOs (frequency < 20hz).

    for (auto i = 0; i < outBuffer.getNumFrames(); i++)
    {
        float sum = 0;
        for (auto& oscillator : oscillators)
        {
            sum += oscillator.generate();
        }

        float sampleFull = ofClamp(sum, -.9f, .9f);

        // write the computed sample to the left and right channels
        outBuffer.getSample(i, 0) = sampleFull;
        outBuffer.getSample(i, 1) = sampleFull;
    }
}

const std::array<float, 20> frequencies = {
    130.81, 164.81, 185.00, 196.00, 246.94, 261.63, 329.63, 369.99, 392.00, 493.88, 523.25, 659.25, 739.99, 783.99, 987.77, 1046.50, 1318.51, 1479.98, 1567.98, 1975.53};

void ofSynthDittmann::setupOscillators()
{
    for (auto frequency : frequencies)
    {
        auto oscillator = ofDCOSine();
        oscillator.setup(sampleRate, frequency, 1);
        oscillators.push_back(oscillator);
    }
}

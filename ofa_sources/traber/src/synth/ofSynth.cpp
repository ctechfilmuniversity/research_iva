//
//  ofSynth.cpp
//  audio_03_synth
//
//  Created by abrennec on 16.10.19.
//

#include "ofSoundBuffer.h"
#include "ofMath.h"
#include "ofMathConstants.h"
#include "synth/ofSynth.h"


ofSynth::ofSynth()
{
    assert(currentFreq == fundamentalFreq);
    setupOscillators();
}

void ofSynth::updateSoundBuffer(ofSoundBuffer& outBuffer)
{
    // this loop builds a buffer of audio containing 3 sine waves at different
    // frequencies, and pulses the volume of each sine wave individually. In
    // other words, 4 oscillators and 2 LFOs (frequency < 20hz).

    for (auto i = 0; i < outBuffer.getNumFrames(); i++) {
        auto sampleFull = getSample(i);
        
        // write the computed sample to the left and right channels
        outBuffer.getSample(i, 0) += sampleFull;
        outBuffer.getSample(i, 1) += sampleFull;
    }
}

float ofSynth::getSample(int frame) {
    // build up a chord out of sine waves at 3 different frequencies
    auto sampleSine = sineWaveOscill.generate();
    auto sampleSaw = sawToothOscill.generate();
    auto sampleTri = triangleOscill.generate();

    auto sampleFull = (sampleSine + sampleSaw + sampleTri);

    // reduce the full sample's volume so it doesn't exceed 1
    sampleFull *= 0.3;
    sampleFull = ofClamp(sampleFull, -.9f, .9f);
    
    return sampleFull;
}

void ofSynth::setFrequency(const float& freq) {
    
    currentFreq = freq;
    updateOscillatorFreq(freq);
}

//void ofSynth::setAmplitude(int oscNum, const float& amp) {
//    
//    if(oscNum == 1) {
//        
//        sineWaveOscill.setAmplitude(amp);
//    }
//    else if(oscNum == 2) {
//        
//        sawToothOscill.setAmplitude(amp);
//    }
//    else if(oscNum == 3) {
//        
//        triangleOscill.setAmplitude(amp);
//    }
//}

void ofSynth::setOverallAmplitude(const float& amp) {
//    sineWaveOscill.setAmplitude(amp * 0.3);
//    sawToothOscill.setAmplitude(amp * 0.75);
//    triangleOscill.setAmplitude(amp * 0.75);
    sineWaveOscill.setAmplitude(amp * 0.7);
    sawToothOscill.setAmplitude(amp * 1);
    triangleOscill.setAmplitude(amp * 1);
}

void ofSynth::setupOscillators() {

//    sineWaveOscill.setup(sampleRate, fundamentalFreq, 0.3);
//    sawToothOscill.setup(sampleRate, fundamentalFreq, 0.75);
//    triangleOscill.setup(sampleRate, fundamentalFreq, 0.75);

    sineWaveOscill.setup(sampleRate, fundamentalFreq, 0);
    sawToothOscill.setup(sampleRate, fundamentalFreq, 0);
    triangleOscill.setup(sampleRate, fundamentalFreq, 0);
    
    sineWavePulse.setup(sampleRate, 2.5, 1.0);
}

void ofSynth::updateOscillatorFreq(const float& freq) {
    
    sineWaveOscill.setFrequency(freq*0.5);
    sawToothOscill.setFrequency(freq*0.25);
    triangleOscill.setFrequency(freq);
}

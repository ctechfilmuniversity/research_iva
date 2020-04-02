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


ofSynth::ofSynth(int numOsc)
{
    assert(currentFreq == fundamentalFreq);
    setupOscillators(numOsc);
}

void ofSynth::updateSoundBuffer(ofSoundBuffer& outBuffer)
{    
    // this loop builds a buffer of audio containing 3 sine waves at different
    // frequencies, and pulses the volume of each sine wave individually. In
    // other words, 4 oscillators and 2 LFOs (frequency < 20hz).
    
    for (auto i = 0; i < outBuffer.getNumFrames(); i++) {
        
        // build up a chord out of sine waves at 3 different frequencies
        
        float sampleFull;
        
        for (int i = 0; i < sineOsc.size(); i++) {
            sampleFull += sineOsc[i].generate();
        }
        
//        auto sampleSine = sineWaveOscill.generate();
//        auto sampleSaw = sawToothOscill.generate();
//        auto sampleTri = triangleOscill.generate();

//        auto sampleFull = (sampleSine + sampleSaw + sampleTri);
        
        // reduce the full sample's volume so it doesn't exceed 1
        sampleFull /= (sineOsc.size() * 0.5);
        sampleFull = ofClamp(sampleFull, -.9f, .9f);
        
        // write the computed sample to the left and right channels
        outBuffer.getSample(i, 0) = sampleFull;
        outBuffer.getSample(i, 1) = sampleFull;
    }
}


void ofSynth::setFrequency(int oscNum, const float& freq) {
    
    sineOsc[oscNum].setFrequency(freq);
    
//    currentFreq = freq;
//    updateOscillatorFreq(freq);
}

void ofSynth::setAmplitude(int oscNum, const float& amp) {
    
    sineOsc[oscNum].setAmplitude(amp);
        
//        sineWaveOscill.setAmplitude(amp);

}

void ofSynth::setupOscillators(int numOsc) {
    
    for (int i = 0; i < numOsc; i++) {
        sineOsc.push_back(ofDCOSine());
        sineOsc[i].setup(sampleRate, fundamentalFreq, 0.0);
        
    }

//    sineWaveOscill.setup(sampleRate, fundamentalFreq, 0.3);
//    sawToothOscill.setup(sampleRate, fundamentalFreq, 0.75);
//    triangleOscill.setup(sampleRate, fundamentalFreq, 0.75);
//
//    sineWavePulse.setup(sampleRate, 2.5, 1.0);
}

void ofSynth::updateOscillatorFreq(const float& freq) {
    
    sineWaveOscill.setFrequency(freq*0.5);
    sawToothOscill.setFrequency(freq*0.25);
    triangleOscill.setFrequency(freq);
}

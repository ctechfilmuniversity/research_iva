//
//  ofSynth2.cpp
//  ivaApp
//
//  Created by Markus Traber on 06.05.20.
//

#include "ofSynth2.hpp"

//
// Constructors
//

//
// Private functions
//
void ofSynth2::changeFrequencyBy(const int& id, const float& factor) {
    oscillators.at(id).setFrequency(oscillators.at(id).getFrequency() + factor);
}

//
// Public functions
//

void ofSynth2::fillSoundBuffer(ofSoundBuffer& outBuffer) {
    auto sampleRate = outBuffer.getSampleRate();
       
    for(auto i = 0; i < outBuffer.getNumFrames(); i++) {
        auto sampleFull = 0;
        
        for (auto &osc : oscillators) {
            sampleFull += osc.generate();
        }
        
        // reduce the full sample's volume so it doesn't exceed 1
        //sampleFull *= 0.4;
        //sampleFull = ofClamp(sampleFull, -.9f, .9f);
        
        // write the computed sample to the left and right channels
        outBuffer.getSample(i, 0) = sampleFull;
        outBuffer.getSample(i, 1) = sampleFull;
    }
}

void ofSynth2::setFrequency(const int& id, const float& freq) {
    //    if (oscillators.size() < id) {
    //        return;
    //    }
    oscillators.at(id).setFrequency(freq);
}

float ofSynth2::getFrequency(const int& id) {
    //    if (oscillators.size() < id) {
    //        return -1;
    //    }
    return oscillators.at(id).getFrequency();
}

void ofSynth2::increaseFrequency(const int& id) {
    changeFrequencyBy(id, FREQ_INCREASE_FACTOR);
}

void ofSynth2::decreaseFrequency(const int& id) {
    changeFrequencyBy(id, FREQ_DECREASE_FACTOR);
}

void ofSynth2::setSampleRate(const int& id, const float& sampleRate){
    oscillators.at(id).setSampleRate(sampleRate);
}

float ofSynth2::getSampleRate(const int& id){
    return oscillators.at(id).getSampleRate();
}

void ofSynth2::setAmplitude(const int& id, const float& rate) {
    oscillators.at(id).setSampleRate(rate);
}

float ofSynth2::getAmplitude(const int& id){
    return oscillators.at(id).getAmplitude();
}

int ofSynth2::addOscillator(const ofDCO::OscillatorType oscillatorType) {
    addOscillator(oscillatorType, SAMPLE_RATE, FUNDAMENTAL_FREQ, AMPLITUDE);
}

int ofSynth2::addOscillator(const ofDCO::OscillatorType oscillatorType, const float sampleRate, const float frequency, const float amplitude) {
    ofDCO dco{oscillatorType};
    dco.setup(sampleRate, frequency, amplitude);
    oscillators.push_back(dco);
    return oscillators.size() - 1;
}


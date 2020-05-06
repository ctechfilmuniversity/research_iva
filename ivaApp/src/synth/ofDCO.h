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
    
    enum OscillatorType {
        SINE,
        PULSE,
        TRIANGLE,
        SAWTOOTH
    };
    

    ofDCO(OscillatorType type = SINE);
    virtual ~ofDCO(){};
    
    
    float generate();
    
    void setup(const float& smplRate, const float& freq, const float& ampl);
    void setFrequency(const float& freq);
    void setAmplitude(const float& amp);
    void setPhase(const float& phs);
    void setPhaseModifier(const float& phsMod);
    void setSampleRate(const float& rate);

    float getFrequency() const;
    float getAmplitude() const;
    float getPhase() const;
    float getPhaseModifier() const;
    float getSampleRate() const;
    
protected:
    
    void updatePhaseIncrement();
    void updatePhase();
    
    float generateSine();
    float generatePulse();
    float generateTriangle();
    float generateSawTooth();
    
    float frequency{220.0};
    float amplitude{1.0};
    float phase{0.0};
    float phaseInc{0.0};
    float phaseMod{1.0}; // frequency multiple
    float sampleRate{44100.0};
    
    OscillatorType dcoType{};
    

    
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

inline void ofDCO::setPhaseModifier(const float &phsMod) {
    phaseMod = phsMod;
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

inline float ofDCO::getPhaseModifier() const {
    return phaseMod;
}

inline float ofDCO::getSampleRate() const {
    return sampleRate;
}

/*
 
 //
 //  BaseOscillator.hpp
 //  libraryBuilding
 //
 //  Created by Aaron Anderson on 8/1/17.
 //
 //

 #ifndef BaseOscillator_hpp
 #define BaseOscillator_hpp

 #include <stdio.h>
 #include "ATKSettings.hpp"
 #include "ofMain.h"

 class BaseOscillator{
     
 public:
     
     BaseOscillator(){
         frequency = 440;
         amplitude = 1.0;
         phase = 0;
         setFreq(frequency);
     }
     
     BaseOscillator(float initFreq){
         frequency = initFreq;
         amplitude = 1.0;
         phase = 0;
         setFreq(frequency);
     }
     
     inline void setFreq(float newFreq){
         frequency = newFreq;
         phaseInc = frequency/ATKSettings::sampleRate;
     }
     void setAmp(float newAmp){
         amplitude = newAmp;
     }
     
     void setPhase(float newPhase){
         phase = newPhase;
     }
     
     void setup(float freq, float amp, double newPhase){
         frequency = freq;
         amplitude = amp;
         phase = newPhase;
     }
     
     float getSample(){
         return currentSample;
     }
     
     virtual void process(){};
     
     float getFreq(){
         return frequency;
     }
     float getAmp(){
         return amplitude;
     }
     
 protected:
     
     float frequency, amplitude;
     double phase, phaseInc;
     float currentSample;
     
 };

 #endif

 
 //
 //  TrivialOscillators.hpp
 //  libraryBuilding
 //
 //  Created by Aaron Anderson on 7/17/17.
 //
 //

 #ifndef TrivialOscillators_hpp
 #define TrivialOscillators_hpp

 #include <stdio.h>
 #include "BaseOscillator.hpp"

 #endif


 class TSine : public BaseOscillator{
     
 public:
     
     TSine(){
         frequency = 440;
         phase = 0;
         amplitude = 1.0;
         setFreq(frequency);
     }
     
     
     TSine(float initFreq) : BaseOscillator(initFreq){
         phase = 0;
         amplitude = 1.0;
         setFreq(initFreq);
     }
     
     inline void process(){
         currentSample = sin(phase)*amplitude;
         phase += phaseInc;
     }
     
     void setFreq(float newFreq){
         frequency = newFreq;
         phaseInc = (TWO_PI*frequency)/ATKSettings::sampleRate;
     }
     
 };

 class TSawtooth : public BaseOscillator{
     
 public:
     TSawtooth(float initFreq) : BaseOscillator(initFreq){
         phase = 0;
         amplitude = 1.0;
         setFreq(initFreq);//this sets the phaseInc
         currentSample = 0;
     }
     
     void process(){
         phase += phaseInc;
         while(phase > 1.0){//use while in case frequency is insanely highh (above SR)
             phase -= 1.0;
         }
         while(phase < 0.0){//to ensure that negative frequencies will work
             phase += 1.0;
         }
         
         currentSample = ((phase*2.f)-1.f) * amplitude;
     }

 };

 class TPhasor : public TSawtooth{//same thing as sawtooth, but unipolar
     
 public:

     TPhasor(float initFreq) : TSawtooth(initFreq){
         phase = 0;
         amplitude = 1.0;
         setFreq(initFreq);//this sets the phaseInc
         currentSample = 0;
     }
     void process(){
         phase += phaseInc;
         while(phase > 1.0){//use while in case frequency is insanely highh (above SR)
             phase -= 1.0;
         }
         while(phase < 0.0){//to ensure that negative frequencies will work
             phase += 1.0;
         }
         currentSample = phase * amplitude;
     }
     
 };

 class TTriangle : public TSawtooth{
     
 public:
     
     TTriangle(float initFreq) : TSawtooth(initFreq){
         phase = 0;
         amplitude = 1.0;
         setFreq(initFreq);
     }
     
     inline float getSample(){
         float tri = 2 * fabs(currentSample) - 1;//manipulate sawtooth to triangle
         return tri*amplitude;
     }
 };

 class TSquare : public BaseOscillator{
     
 public:
     
     TSquare(float initFreq) : BaseOscillator(initFreq){
         phase = 0;
         amplitude = 1.0;
         setFreq(initFreq);
         dutyCycle = 0.5;
     }
     
     inline void process(){
         phase += phaseInc;
         while(phase > 1.0){
             phase -= 1;
         }
         while(phase < 0){
             phase += 1.0;
         }
         
         if(phase > dutyCycle){
             currentSample = 1.0 * amplitude;
         }else{
             currentSample = -1.0 * amplitude;
         }
         
     }
     
     void setDuty(float newDutyCycle){
         dutyCycle = newDutyCycle;
     }
     
 private:
     float dutyCycle;
 };



 */

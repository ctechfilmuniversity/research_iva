//
//  ofDCOBaseTypes.h
//  audio_04_osc
//
//  Created by abrennec on 31.10.19.
//
//  Reference: Aaron Anderson's ofxATK addon
//  Also nice to read: http://www.martin-finke.de/blog/articles/audio-plugins-008-synthesizing-waveforms/
//

#pragma once

#include <synth/ofDCO.h>


class ofDCOSine : public ofDCO {
    
    
public:
    
    ofDCOSine();
    ~ofDCOSine(){};
        
    virtual float generate();
    
    
protected:
     
};



class ofDCOTriangle : public ofDCO {
    
public:
    
    ofDCOTriangle();
    ~ofDCOTriangle(){};
        
    virtual float generate();
    
    
protected:
     
};



class ofDCOSawTooth : public ofDCO {
    
public:
    
    ofDCOSawTooth();
    ~ofDCOSawTooth(){};
        
    virtual float generate();
    
    
protected:
     
};



class ofDCOPulse : public ofDCO {

public:
    
    ofDCOPulse();
    ~ofDCOPulse(){};
        
    virtual float generate();
    
    
protected:
    
    
private:
    
    float dutyCycle{0.5};
 
};


//returns a sample given an input phase and desired
//waveform type
//eg. sine, square, sawtooth, triangle
//float ofApp::generateSample(float phase, int waveType){
//    switch (waveType) {
//        case 1://sine
//            return sin(phase*TWO_PI);
//            break;
//        case 2://square
//            return sin(phase*TWO_PI)>0?1:-1;
//        case 3://sawtooth
//            return fmod(phase,TWO_PI);
//        case 4://triangle
//            return abs(sin(phase*TWO_PI));
//        default:
//            break;
//    }
//}

/*


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

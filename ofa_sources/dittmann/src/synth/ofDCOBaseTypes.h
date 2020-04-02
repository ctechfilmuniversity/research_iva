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

 
};

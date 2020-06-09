
#pragma once

//#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofSynth2.hpp"
#include "ivaApp.h"

class stimberg : public ivaApp {

public:
    void setup();
    void update();
    void draw();
    
    void audioOut(ofSoundBuffer &outBuffer);
    
    ofVideoGrabber cam;
    ofxCv::FlowFarneback flow;
//    ofMesh mesh;

    int stepSize, xSteps, ySteps;
    
    vector<ofVec2f> flowAmount;
    ofxCvGrayscaleImage grayImg;
    
    void shutdownApp();
    
private:
    // synth stuff
    //ofSynthStimberg synth{8};
    ofSynth2 synth2{};

    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    std::mutex audioMutex{};
    
    void setupAudio();
    void setupSynth();
};

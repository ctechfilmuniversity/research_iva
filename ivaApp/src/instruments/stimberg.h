
#pragma once

#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ivaApp.h"

class stimberg : public ivaApp {

public:
    void setup();
    void update();
    void draw();
    void shutdownApp();

private:
    int stepSize, xSteps, ySteps;

    ofVideoGrabber cam;
    ofxCv::FlowFarneback flow;
    ofxCvGrayscaleImage grayImg;
    ofxCvColorImage colorImg;

    vector<ofVec2f> flowAmount;

    vector<pdsp::ParameterAmp> amps;
    vector<pdsp::VAOscillator> oscs;

    void setupAudio();
};

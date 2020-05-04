
#pragma once

//#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofSynthStimberg.h"
#include "ivaApp.h"

class stimberg : public ivaApp
{

public:
	void setup();
	void update();
	void draw();
    
    void audioOut(ofSoundBuffer &outBuffer);
	
	ofVideoGrabber cam;
	ofxCv::FlowFarneback flow;
//	ofMesh mesh;

    int stepSize, xSteps, ySteps;
    
    vector<ofVec2f> flowAmount;
    ofxCvGrayscaleImage grayImg;
    
    void shutdownApp();
    
private:
    // synth stuff
    ofSynthStimberg synth{8};
    int oscNums;

    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    std::mutex audioMutex{};
    
    void setupAudio();
    
//    float ampAry[4]{0, 0, 0, 0};
    
    
};

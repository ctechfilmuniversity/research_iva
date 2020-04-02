
#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "synth/ofSynth.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
    
    void audioOut(ofSoundBuffer &outBuffer);
	
	ofVideoGrabber cam;
	ofxCv::FlowFarneback flow;
	ofMesh mesh;

    int stepSize, xSteps, ySteps;
    
    vector<ofVec2f> flowAmount;
    ofxCvGrayscaleImage grayImg;
    
private:
    // synth stuff
    ofSynth synth{8};
    int oscNums;

    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    std::mutex audioMutex{};
    
    void setupAudio();
    
//    float ampAry[4]{0, 0, 0, 0};
    
    
};

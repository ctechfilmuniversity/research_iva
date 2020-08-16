    // Source:
    // https://github.com/wearenocomputer/workshop


#pragma once

//#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ivaApp.h"

//class clausen : public ofBaseApp{
class clausen : public ivaApp {
public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);    
    void shutdownApp();
    void audioOut(ofSoundBuffer &outBuffer);

private:
    int fboSize, index;
    
    ofVideoGrabber grabber;
    ofFbo fbo;
    
    void debugDraw();
    
    ofTexture tex;
    ofxCvColorImage imageCv;
    ofxCvGrayscaleImage imageGray;
    
    bool captured = false;
    bool filtered = false;
    
    unsigned char * pixelBuffer;
    
    ofSoundStream soundStream;
    int sampleCount;
    
    int width;
    int height;
    
    
    void resetData();

};

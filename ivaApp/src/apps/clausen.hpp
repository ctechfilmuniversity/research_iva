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
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
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
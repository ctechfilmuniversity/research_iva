    // Source:
    // https://github.com/wearenocomputer/workshop


#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "synth/ofSynth.h"

class ofApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();
    
    void audioOut(ofSoundBuffer &outBuffer);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    
    //----------------------------------------------------------
private:
    
    int fboSize, index;
    int grabberHeight{240};
    int grabberWidth{320};
    
    ofVideoGrabber grabber;
    ofFbo fbo;
    ofxCvGrayscaleImage grayImage;    //Grayscaled original image
    
    void debugDraw();


    void setupAudio();
    void setupGui();
    
    // function bound to gui element
    // therefore its scope is defined by ofEvent
    // see ofEventUtils for further information
    void updateGui(float& value);
    
    //ofSynth synth{};
    
    std::vector<ofSynth> synths;
    std::vector<int> synthTones {-5, -2, 0, 3, 5, 7, 10, 12, 15};
    
    bool changeAmp{false};
    
    void playPitch(int pitchnum);
    
    int numOscillators{6};
    
    int oscNum{1};
    int initialMouseDistY{};
    
    // more info on using mutex and locks at
    // https://medium.com/swlh/c-mutex-write-your-first-concurrent-code-69ac8b332288
    std::mutex audioMutex{};
    
    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    
    
};

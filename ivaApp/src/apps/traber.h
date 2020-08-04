    // Source:
    // https://github.com/wearenocomputer/workshop


#pragma once

//#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofSynth2.hpp"
#include "ivaApp.h"

class traber : public ivaApp {
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
    
    void shutdownApp();

    
    //----------------------------------------------------------
private:
    bool drawUI = false;
    bool recordMode = false;
    bool recorded = false;
    bool running = true;
    int fboSize, index, currentToneIndex;
    int grabberHeight{240};
    int grabberWidth{320};
    
    ofVideoGrabber grabber;
    ofFbo fbo, fboTonesPlayed;
    ofxCvGrayscaleImage grayImage;    //Grayscaled original image
    
    ofImage image;
    
    ofColor whiteAlphaLine{255, 255, 255, 1};
    
        
    void debugDraw();
    void drawToneLines();
    void drawPositionLine();
    void drawToneNames();
    void drawPositionLineCurrentTone();
    void drawTextInfo();


    void setupAudio();
    void setupGui();
    
    // function bound to gui element
    // therefore its scope is defined by ofEvent
    // see ofEventUtils for further information
    void updateGui(float& value);
    
    ofSynth2 synth{};
    
    //inline static int numTones = 7;
    // -5 E3, -2 G3, 0 A3, 3 C4, 5 D4, 7 E4, 10 G4, 12 A4, 15 C5
    // std::vector<int> synthTones {-5, -2, 0, 3, 5, 7, 10, 12, 15};
    std::vector<int> synthTones {-5, -2, 0, 3, 5, 7};
    std::vector<string> toneNames {"E3", "G3", "A3", "C4", "D4", "E4"};
    
    void updateToneAndIndex();
    void updateFramebuffer();
    void updateFramebufferTonesPlayed();
    void calculateToneIndex();
    
    void changeRecordMode(const bool mode);
    void changeRunning(const bool run);
    
    void resetToStart();
        
    // more info on using mutex and locks at
    // https://medium.com/swlh/c-mutex-write-your-first-concurrent-code-69ac8b332288
    std::mutex audioMutex{};
    
    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    
    ofPolyline toneLines{};
};

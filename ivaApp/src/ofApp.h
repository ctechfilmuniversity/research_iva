#pragma once

#include "ofMain.h"
#include "synth/ofSynth.h"
#include "instruments/brennecke.hpp"
#include "instruments/clausen.hpp"
#include "instruments/traber.h"
#include "instruments/stimberg.h"
#include "instruments/dittmann.h"
#include "instruments/objectContours.hpp"

// you possibly need to adapt this number for your hardware
// look for your index in the console output when starting
#define AUDIO_DEVICE 1

class ofApp : public ofBaseApp {

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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    int appIndex = 0;
    std::vector<ivaApp*> apps;
    
    unique_ptr<pdsp::Engine> enginePtr;
    void switchApp(int key);
    void resetPDSPengine();
};

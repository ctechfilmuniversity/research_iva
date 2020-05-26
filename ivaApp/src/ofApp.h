#pragma once

#include "ofMain.h"
#include "synth/ofSynth.h"
#include "apps/brennecke.hpp"
#include "apps/clausen.hpp"
#include "apps/traber.h"
#include "apps/stimberg.h"
#include "apps/dittmann.h"


class ofApp : public ofBaseApp {

public:
    
    void setup();
    void update();
    void draw();

    //void audioOut(ofSoundBuffer &outBuffer);

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
    ivaApp *currentApp;
    
    brennecke brenneckeApp;
    clausen clausenApp;
    traber traberApp;
    stimberg stimbergApp;
    dittmann dittmannApp;
    
    
    void switchApp(int key);
};

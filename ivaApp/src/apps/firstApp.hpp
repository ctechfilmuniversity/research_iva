//
//  firstApp.hpp
//  ivaApp
//
//  Created by Markus Traber on 24.04.20.
//

#ifndef firstApp_hpp
#define firstApp_hpp

#include <stdio.h>

#endif /* firstApp_hpp */


#pragma once

//#include "ofMain.h"
#include "synth/ofSynth.h"
#include "ivaApp.h"

class firstApp : public ivaApp {

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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void shutdownApp();
private:

    // Member functions

    void drawHelpText();
    void setupAudio();
    
    // function bound to gui element
    // therefore its scope is defined by ofEvent
    // see ofEventUtils for further information
    void updateFrequency(float& value);

    
    // Member variables
    float rms{};
    //float defaultFrequency{172.5};
    ofSynth synth{};

    bool changeFreq{false};
    int initialMouseDistY{};

    std::mutex audioMutex{};
    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    ofPolyline waveform{};
    
};

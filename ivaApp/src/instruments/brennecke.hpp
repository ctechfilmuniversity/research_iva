#ifndef brennecke_hpp
#define brennecke_hpp

#include <stdio.h>

#endif /* brennecke_hpp */


#pragma once

//#include "ofMain.h"
#include "ofSynth2.hpp"
#include "ivaApp.h"


class brennecke : public ivaApp {

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

    void shutdownApp();
private:
    static constexpr float FUNDAMENTAL_FREQ = 172.5;
    
    // Member functions
    void drawHelpText();
    void setupAudio();
    
    // function bound to gui element
    // therefore its scope is defined by ofEvent
    // see ofEventUtils for further information
    void updateFrequency(const float& value);

    
    // Member variables
    float rms{};

    bool changeFreq{false};
    int initialMouseDistY{};

    ofPolyline waveform{};
        
    pdsp::VAOscillator      osc_1, osc_2, osc_3, osc_4;
    pdsp::ValueControl      pitch_ctrl_1, pitch_ctrl_2, pitch_ctrl_3, pitch_ctrl_4;
    pdsp::ParameterAmp      osc_amp;
    
    pdsp::Scope scopeTest;
    
};

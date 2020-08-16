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

    void audioOut(ofSoundBuffer &outBuffer);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );

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
    ofSynth2 synth{};

    bool changeFreq{false};
    int initialMouseDistY{};

    std::mutex audioMutex{};
    ofSoundBuffer lastBuffer{};
    ofPolyline waveform{};
    
    int id_sine1, id_sine2, id_sine3, id_sine4, id_pulse1, id_pulse2, id_sawtooth, id_triangle;
    
};

#pragma once

#include "ofMain.h"
#include "synth/ofSynth.h"

class ofApp : public ofBaseApp {

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

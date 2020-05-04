#pragma once

#include "ivaApp.h"
#include "ofxGui.h"
#include "ofxMidi.h"
#include "ofSynthDittmann.h"

class dittmann : public ivaApp, public ofxMidiListener
{

  public:
    void setup();
    void update();
    void draw();

    void audioOut(ofSoundBuffer& outBuffer);
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
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
    void setupGui();
    void setupMidi();

    // function bound to gui element
    // therefore its scope is defined by ofEvent
    // see ofEventUtils for further information
    void updateGui(float& value);

    // Member variables
    float rms{};
    ofSynthDittmann synth{};

    // TODO: Correct the variable name
    bool changeAmp{false};

    int oscNum{1};
    int initialMouseDistY{};

    // more info on using mutex and locks at
    // https://medium.com/swlh/c-mutex-write-your-first-concurrent-code-69ac8b332288
    std::mutex audioMutex{};

    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    ofPolyline waveform{};

    ofVideoGrabber grabber;

    const int grabWidth{640};
    const int grabHeight{480};

    // gui
    ofxPanel gui{};
    ofxFloatSlider frequencySlider{};
    //    ofxFloatSlider amplitudeSlider{};

    // MIDI
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    void newMidiMessage(ofxMidiMessage& eventArgs);
};

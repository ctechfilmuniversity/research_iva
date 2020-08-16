//
//  objectContours.hpp
//  ivaApp
//
//  Created by Markus Traber on 10.08.20.
//

#ifndef objectContours_hpp
#define objectContours_hpp

#include <stdio.h>

#include "ofxOpenCv.h"
#include "ofSynth2.hpp"
#include "ivaApp.h"

enum objectContoursMode {
    TONE_MODE = 0,
    FREQ_HEIGHT_MODE = 1,
    FREQ_AREA_MODE = 2,
};

class objectContours : public ivaApp {
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
    // more info on using mutex and locks at
    // https://medium.com/swlh/c-mutex-write-your-first-concurrent-code-69ac8b332288
    std::mutex audioMutex{};
    //ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    ofSynth2 synth{};
    
    ofVideoGrabber         vidGrabber;
    
    ofxCvColorImage            colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvGrayscaleImage     grayBg;
    ofxCvGrayscaleImage     grayDiff;

    ofxCvContourFinder     contourFinder;

    int                 threshold;
    bool                bLearnBakground;
    
    int mode = FREQ_AREA_MODE;
    int numSynths = 10;
    int maxAreaSize;
    
    std::vector<int> synthTones {-5, -2, 0, 3, 5, 7, 10, 12, 15, 17};
    
    void updateSynth(const int index);
    void muteRemainingSynths();
    void setupAudio();
    void toggleMode();
};


#endif /* objectContours_hpp */

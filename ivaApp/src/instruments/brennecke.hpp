#ifndef brennecke_hpp
#define brennecke_hpp

#include <stdio.h>

#endif /* brennecke_hpp */


#pragma once

#include "ivaApp.h"


class brennecke : public ivaApp {

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );

    void shutdownApp();

private:
    static constexpr float FUNDAMENTAL_FREQ = 172.5;
    //static constexpr float SAMPLE_RATE = 44100.0;
    //static constexpr float AMPLITUDE = 0.5;
    
    // Member functions
    void drawHelpText();
    void setupAudio();
    
    // We just alter the pitch of the first osc.
    // Therefore we dont need a parameter to choose which osc right now.
    void updateFrequency(const float& value);
    void increaseFrequency();
    void decreaseFrequency();

    
    // Member variables
    //float rms{};

    bool changeFreq{false};
    int initialMouseDistY{};

    pdsp::ValueControl      pitch_ctrl_1, pitch_ctrl_2;
    pdsp::ParameterAmp      amp_ctrl_1, amp_ctrl_2;

    pdsp::SineFB            sine_1, sine_2;
    pdsp::PMPhasor          phasor_1, phasor_2;
    
    pdsp::Scope             scope;
};

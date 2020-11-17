#include "brennecke.hpp"


//--------------------------------------------------------------
void brennecke::setup(){
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    setupAudio();
}

//--------------------------------------------------------------
void brennecke::update(){
    
    // "lastBuffer" is shared between update() and audioOut(), which are called
    // on two different threads. This lock makes sure we don't use lastBuffer
    // from both threads simultaneously (see the corresponding lock in audioOut())

    // this loop is building up a polyline representing the audio contained in
    // the left channel of the buffer
    
    // the x coordinates are evenly spaced on a grid from 0 to the window's width
    // the y coordinates map each audio sample's range (-1 to 1) to the height
    // of the window
    pdsp::SampleBuffer          sampleData;
    sampleData.
//    waveform.clear();
//    for(size_t i = 0; i < lastBuffer.getNumFrames(); i++) {
//        float sample = lastBuffer.getSample(i, 0);
//        float x = ofMap(i, 0, lastBuffer.getNumFrames(), 0, ofGetWidth());
//        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
//        waveform.addVertex(x, y);
//    }
    
//    rms = lastBuffer.getRMSAmplitude();
}

//--------------------------------------------------------------
void brennecke::draw() {
    
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);
    ofSetLineWidth(1 + (rms * 30.));
    //waveform.draw();
    
    scopeTest.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    drawHelpText();
}

//--------------------------------------------------------------
void brennecke::keyPressed(int key){
    
    if (key == 'f' && !changeFreq) {
        
        changeFreq = true;
        initialMouseDistY = ofGetMouseY() - ofGetWindowPositionY();
    }
    else if (key == 'r') {
        updateFrequency( FUNDAMENTAL_FREQ );
    }
    else if (key == '1') {
        
        // play back sine
    }
    else if (key == '2') {
        
        // play back pulse
    }
    else if (key == '3') {
        
        // play back saw
    }
    else if (key == '4') {
        
        // play back triangle
    }

}

//--------------------------------------------------------------
void brennecke::keyReleased(int key){
    if (key == 'f') {
        changeFreq = false;
        std::cout << "<<<<<  Freq: " << pitch_ctrl_1.get() << std::endl;
        std::cout << "<<<<<  FrameRate: " << ofGetFrameRate() << std::endl;
        std::cout << "<<<<<  FrameNum: " << ofGetFrameNum() << std::endl;
        std::cout << "<<<<<  ElapsedTime: " << ofGetElapsedTimef() << std::endl;
    }
}

//--------------------------------------------------------------
void brennecke::mouseMoved(int x, int y ){

    if (changeFreq) {
        
        int currentMouseDistY = y - ofGetWindowPositionY();
        
        // only if we have actually moved the mouse
        if (currentMouseDistY != initialMouseDistY) {
            if (currentMouseDistY > initialMouseDistY) synth.increaseFrequency(id_sine1);
            if (currentMouseDistY < initialMouseDistY) synth.decreaseFrequency(id_sine1);
            
            float frequency = ofClamp(synth.getFrequency(id_sine1), 0.0, 20000.0);
            updateFrequency(frequency);
            
            initialMouseDistY = currentMouseDistY;
        }
    }
}

//--------------------------------------------------------------
void brennecke::drawHelpText() {
 
    std::string freqString = std::to_string(synth.getFrequency(id_sine1));
    ofDrawBitmapString("Press 'f' and move the mouse in y-direction.", 25, 25);
    ofDrawBitmapString("Press 'r' to reset the frequency.", 25, 40);
    ofDrawBitmapString("Current frequency: " + freqString, 25, 75);
}

//--------------------------------------------------------------
void brennecke::setupAudio() {
    pitch_ctrl_1 >> osc_1.in_pitch();
    pitch_ctrl_2 >> osc_2.in_pitch();
    pitch_ctrl_3 >> osc_3.in_pitch();
    pitch_ctrl_4 >> osc_4.in_pitch();
    osc_1.out_sine() >> osc_amp >> enginePtr->audio_out(0);
    osc_1.out_sine() >> osc_amp >> enginePtr->audio_out(1);
    osc_2.out_sine() >> osc_amp >> enginePtr->audio_out(0);
    osc_2.out_sine() >> osc_amp >> enginePtr->audio_out(1);
    osc_3.out_sine() >> osc_amp >> enginePtr->audio_out(0);
    osc_3.out_sine() >> osc_amp >> enginePtr->audio_out(1);
    osc_4.out_sine() >> osc_amp >> enginePtr->audio_out(0);
    osc_4.out_sine() >> osc_amp >> enginePtr->audio_out(1);
    
    osc_1.out_sine() >> osc_amp >> scopeTest >> enginePtr->blackhole();
    
    
    //pitch_ctrl.enableSmoothing(50.0f);
}

//--------------------------------------------------------------
void brennecke::updateFrequency(float& value) {
    
    synth.setFrequency(id_sine1, value);
};

void brennecke::shutdownApp() {
    soundStream.close();
    lastBuffer.clear();
    synth.reset();
}


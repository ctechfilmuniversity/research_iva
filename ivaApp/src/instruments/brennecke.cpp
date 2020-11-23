#include "brennecke.hpp"

constexpr float brennecke::FUNDAMENTAL_FREQ;

//--------------------------------------------------------------
void brennecke::setup(){
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    setupAudio();
}

//--------------------------------------------------------------
void brennecke::update(){
}

//--------------------------------------------------------------
void brennecke::draw() {
    
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);

    // before using pdsp, the thickness of the line was defined by using the following algorithm
    // 1 + (rms * 30.)
    // since the line width is hard coded into scope, this is not working right now.
    // maybe a new class could be implemented based on scope in order to enable this functionality
    scope.draw(0, 0, ofGetWidth(), ofGetHeight());
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
            if (currentMouseDistY > initialMouseDistY) increaseFrequency();
            if (currentMouseDistY < initialMouseDistY) decreaseFrequency();
            
            float frequency = ofClamp(pitch_ctrl_1.get(), 0.0, 20000.0);
            updateFrequency(frequency);
            
            initialMouseDistY = currentMouseDistY;
        }
    }
}

//--------------------------------------------------------------
void brennecke::drawHelpText() {
 
    std::string freqString = std::to_string(pitch_ctrl_1.get());
    ofDrawBitmapString("Press 'f' and move the mouse in y-direction.", 25, 25);
    ofDrawBitmapString("Press 'r' to reset the frequency.", 25, 40);
    ofDrawBitmapString("Current frequency: " + freqString, 25, 75);
}

//--------------------------------------------------------------
void brennecke::setupAudio() {
    // Defining start values
    pitch_ctrl_1.set(brennecke::FUNDAMENTAL_FREQ);
    pitch_ctrl_2.set(200);

    amp_ctrl_1.set(.3f);
    amp_ctrl_2.set(.5f);

    //pitch_ctrl.enableSmoothing(50.0f);


    // Patching everything
    pitch_ctrl_1 >> phasor_1.in_freq();
    pitch_ctrl_2 >> phasor_2.in_freq();

    phasor_1 >> sine_1 >> amp_ctrl_1 >> enginePtr->audio_out(0);
    phasor_1 >> sine_1 >> amp_ctrl_1 >> enginePtr->audio_out(1);
    phasor_2 >> sine_2 >> amp_ctrl_2 >> enginePtr->audio_out(0);
    phasor_2 >> sine_2 >> amp_ctrl_2 >> enginePtr->audio_out(1);

    // Patching the output of one channel to scope, a utility class to monitor the signal
    enginePtr->audio_out(0) >> scope >> enginePtr->blackhole();
}

//--------------------------------------------------------------
void brennecke::updateFrequency(const float& value) {
    pitch_ctrl_1.set(value);
}

void brennecke::increaseFrequency() {
    updateFrequency(pitch_ctrl_1.get() + 10);
}

void brennecke::decreaseFrequency() {
    updateFrequency(pitch_ctrl_1.get() - 10);
}

void brennecke::shutdownApp() {
//    soundStream.close();
//    lastBuffer.clear();
//    synth.reset();
}


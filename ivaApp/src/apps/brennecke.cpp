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
    unique_lock<mutex> lock(audioMutex);

    // this loop is building up a polyline representing the audio contained in
    // the left channel of the buffer
    
    // the x coordinates are evenly spaced on a grid from 0 to the window's width
    // the y coordinates map each audio sample's range (-1 to 1) to the height
    // of the window
    
    waveform.clear();
    for(size_t i = 0; i < lastBuffer.getNumFrames(); i++) {
        float sample = lastBuffer.getSample(i, 0);
        float x = ofMap(i, 0, lastBuffer.getNumFrames(), 0, ofGetWidth());
        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
        waveform.addVertex(x, y);
    }
    
    rms = lastBuffer.getRMSAmplitude();
}

//--------------------------------------------------------------
void brennecke::draw() {
    
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);
    ofSetLineWidth(1 + (rms * 30.));
    waveform.draw();
    
    drawHelpText();
}

//--------------------------------------------------------------
void brennecke::audioOut(ofSoundBuffer &outBuffer) {
    
    synth.fillSoundBuffer(outBuffer);
    
    unique_lock<mutex> lock(audioMutex);
    lastBuffer = outBuffer;
}

//--------------------------------------------------------------
void brennecke::keyPressed(int key){
    
    if (key == 'f' && !changeFreq) {
        
        changeFreq = true;
        initialMouseDistY = ofGetMouseY() - ofGetWindowPositionY();
    }
    else if (key == 'r') {
        auto freq = synth.FUNDAMENTAL_FREQ; // get fundamental freq
        updateFrequency( freq );
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
        std::cout << "<<<<<  Freq: " << synth.getFrequency(id_sine1) << std::endl;
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
void brennecke::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void brennecke::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void brennecke::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void brennecke::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void brennecke::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void brennecke::windowResized(int w, int h){

}

//--------------------------------------------------------------
void brennecke::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void brennecke::dragEvent(ofDragInfo dragInfo){

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
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = synth.SAMPLE_RATE;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    
    // the following setup function initiates the whole audio connection
    // it invokes the underlying RTAudioSoundStream to
    // - create an RtAudio object
    // - connect the object to the RtAudioCallback function
    // - start the stream and hence have a continious connection to audio in & out
    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
    
    // Setup all oscillators
    id_sine1 = synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.3);
    id_sine2 = synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.0);
    id_sine3 = synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, 200, 0.5);
    id_sine4 = synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.0);
    
//    id_sawtooth = synth.addOscillator(ofDCO::SAWTOOTH, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.75);
//    id_triangle = synth.addOscillator(ofDCO::TRIANGLE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.3);
//
//    id_pulse1 = synth.addOscillator(ofDCO::PULSE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.3);
//    id_pulse2 = synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0.3);
//
//    synth.setPhaseModifier(id_pulse1, 0.98);
//    synth.setPhaseModifier(id_pulse2, 3.98);
    
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


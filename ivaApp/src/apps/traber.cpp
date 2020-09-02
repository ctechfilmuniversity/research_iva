// Based on slitscan example from: https://github.com/wearenocomputer/workshop

#include "traber.h"

//--------------------------------------------------------------
void traber::setup(){
    ofBackground(33);
    ofSetWindowShape(512, 384);
    
    // Init grabber
    grabberWidth = 640;
    grabberHeight = 480;
        
    grabber.initGrabber(grabberWidth, grabberHeight);
    
    // Init FBO
    // FBO sizes need to be a power of 2
    // Otherwise openFrameworks will scale
    // our FBO, which results in incorrect
    // mapping of texture coordinates
    fboSize = ofGetWidth();
    index = 0;
    currentToneIndex = 0;
        
    fbo.allocate(fboSize, fboSize, GL_RGBA);
    fboTonesPlayed.allocate(fboSize, fboSize, GL_RGBA);
    grayImage.allocate(ofGetWidth(), ofGetHeight());
    setupAudio();
}

//--------------------------------------------------------------
void traber::update(){
    // "lastBuffer" is shared between update() and audioOut(), which are called
    // on two different threads. This lock makes sure we don't use lastBuffer
    // from both threads simultaneously (see the corresponding lock in audioOut())
    //    std::unique_lock<std::mutex> lock_name(audioMutex);
    
    if (!running) {
        return;
    }
    
    // Grab the camera feed
    // and check for new frames
    grabber.update();
    if(grabber.isFrameNew()){
        
        // this part is inside the grabber.isFrameNew() if-clause, since the speed for a recording would be different outside.
        if (recorded && recordMode) {
            updateToneAndIndex();
            return;
        }
        
        if (recordMode && index == fboSize) {
            recorded = true;
        }
        
        updateToneAndIndex();
        
        // And draw a slice of the
        // new frame to the FBO
        updateFramebuffer();
        updateFramebufferTonesPlayed();
    }
}

void traber::updateToneAndIndex() {
    calculateToneIndex();
    pitch_ctrl.set(57 + synthTones.at(currentToneIndex));
    
    //index < fboSize ? index++ : index=0;
    if (index < fboSize) {
        index++;
    } else {
        index = 0;
        if (recordMode) {
            recorded = true;
        }
    }
}

//--------------------------------------------------------------
void traber::updateFramebuffer() {
    float slice = grabber.getWidth() / float(fboSize);
    float offset = grabber.getWidth() * 0.5;
    
    //grayImage.blurGaussian( 9 );
    //grayImage.threshold(150);
    
    fbo.begin();
        grabber.getTexture().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
    fbo.end();
}

void traber::updateFramebufferTonesPlayed() {
    fboTonesPlayed.begin();
        if (index == 0 && !recordMode) {
            ofClear(255,255,255, 0); // Clear canvas when starting from 0
        }
    
        ofPolyline line{};
        line.addVertex(index, fboSize / synthTones.size() * currentToneIndex);
        line.addVertex(index, fboSize / synthTones.size() * (currentToneIndex + 1));
    
        // Problems with alpha when switching between ui on and off
        //ofColor greenAlpha(13, 255, 100, 100);
        //ofSetColor(greenAlpha);
        ofSetColor(ofColor::lightSeaGreen);
        line.draw();
        ofSetColor(ofColor::white);
    fboTonesPlayed.end();
}

//--------------------------------------------------------------
void traber::calculateToneIndex() {
    auto stepSize{grabberHeight / 6};
    
    // TODO: use brightness to adjust amplitude?
    
    // now using the fbo instead of webcam image in order to enable recording function
    ofPixels pixels;
    fbo.getTexture().readToPixels(pixels);
    pixels.setImageType(OF_IMAGE_GRAYSCALE);
    grayImage.setFromPixels(pixels);
    
    float currentMeanBrightness = 0;
    int currentToneIndex = 0;
    for (int i = 0; i < ofGetHeight(); i += stepSize) {
        
        float accumulatedBrightness = 0;
        for (int in = i; in < i+stepSize; in++) {
            accumulatedBrightness += grayImage.getPixels().getColor(index, in).getBrightness();
        }
        float meanBrightness = accumulatedBrightness / stepSize;
        
        //cout << "index " << i << " accumulatedBrightness " << accumulatedBrightness << " meanBrightness " << meanBrightness << "\n";
        
        if (meanBrightness > currentMeanBrightness) {
            currentMeanBrightness = meanBrightness;
            //currentAmp = ofMap(meanBrightness, 0, limit, 0, 1);
            currentToneIndex = i / stepSize;
        }
    }
    this->currentToneIndex = currentToneIndex;
}

//--------------------------------------------------------------
void traber::draw(){
    // debugDraw();

    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    if (drawUI) {
        fboTonesPlayed.draw(0, 0, ofGetWidth(), ofGetHeight());
        drawToneLines();
        drawPositionLine();
        drawToneNames();
        drawPositionLineCurrentTone();
        drawTextInfo();
    } else {
        ofDrawBitmapString("Press space to toggle UI", 10, 20);
    }
}

void traber::drawToneLines() {
    ofSetColor(whiteAlphaLine);
    ofSetLineWidth(1);
    
    for (int i=1; i < synthTones.size(); i++) {
        float x1 = i % 2 != 0 ? -1 : ofGetWidth() + 1;
        float x2 = i % 2 != 0 ? ofGetWidth() + 1 : -1;
        float y = ofGetHeight() / synthTones.size() * i;
        toneLines.addVertex(x1, y);
        toneLines.addVertex(x2, y);
        
        if (i == synthTones.size() - 1 && i % 2 != 0) {
            toneLines.addVertex( ofGetWidth() + 1, ofGetHeight() + 1);
            toneLines.addVertex( -1,               ofGetHeight() + 1);
        }
    }

    toneLines.draw();
    
    ofSetColor(ofColor::white);
}

void traber::drawPositionLine() {
    ofPolyline line{};
    line.addVertex(index, 0);
    line.addVertex(index, ofGetHeight());
    ofSetColor(ofColor::red);
    line.draw();
    ofSetColor(ofColor::white);
}

void traber::drawToneNames() {
    for (int i=0; i < toneNames.size(); i++) {
        int y = ofGetHeight() / toneNames.size() * i + ofGetHeight() / toneNames.size() / 2;
        ofDrawBitmapString(toneNames[i], index + 10, y);
    }
}

void traber::drawPositionLineCurrentTone() {
    ofPolyline line{};
    line.addVertex(index, ofGetHeight() / synthTones.size() * currentToneIndex);
    line.addVertex(index, ofGetHeight() / synthTones.size() * (currentToneIndex + 1));
    ofSetColor(ofColor::green);
    line.draw();
    ofSetColor(ofColor::white);
}

void traber::drawTextInfo() {
    string runningText = "Running:      ";
    runningText.append(running ? "yes" : "no ");
    runningText.append("   Press 's' to toggle.");
    
    string recordModeText = "Record mode:  ";
    recordModeText.append(recordMode ? "yes" : "no ");
    recordModeText.append("   Press 'r' to toggle.");
    
    ofDrawBitmapString(runningText, 10, 20);
    ofDrawBitmapString(recordModeText, 10, 35);
}

//--------------------------------------------------------------
void traber::debugDraw(){
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);

    //grabber.draw(0, 20, grabberWidth, grabberHeight);
    //grayImage.draw(0, 250, grabberWidth, grabberHeight);
    grayImage.draw(0, 20, grayImage.getWidth() / 2, grayImage.getHeight() / 2);

    ofPopMatrix();
}


//--------------------------------------------------------------
// void traber::audioOut(ofSoundBuffer &outBuffer) {
//     synth.fillSoundBuffer(outBuffer);
//     
//     // THREAD INFO
//     // lock_name is the "var" name of the lock guard, kind of
//     // a variable that is being constructed with a mutex (audioMutex),
//     // locks the mutex at its construction and unlocks the mutex
//     // when it is being destroyed, i.e., at the end of the scope
// 
//     std::unique_lock<std::mutex> lock_name(audioMutex);
//     lastBuffer = outBuffer;
// }

//--------------------------------------------------------------
void traber::setupAudio() {
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
//    ofSoundStreamSettings settings;
//    settings.numOutputChannels = 2;
//    settings.sampleRate = 44100;
//    settings.bufferSize = 1024;
//    settings.numBuffers = 4;
//    settings.setOutListener(this);
    
    // the following setup function initiates the whole audio connection
    // it invokes the underlying RTAudioSoundStream to
    // - create an RtAudio object
    // - connect the object to the RtAudioCallback function
    // - start the stream and hence have a continious connection to audio in & out
//    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
 
//    synth.addOscillator(ofDCO::SINE);
//    synth.setSampleRate(0, settings.sampleRate);
    
    pitch_ctrl >> osc.in_pitch();
    
//    osc.out_sine() >> osc_amp >> engine.audio_out(0); // connect to left output channel
//    osc.out_sine() >> osc_amp >> engine.audio_out(1); // connect to right right channel
    
    osc.out_sine() >> osc_amp >> enginePtr->audio_out(0); // connect to left output channel
    osc.out_sine() >> osc_amp >> enginePtr->audio_out(1); // connect to right right channel
    
    
    //pitch_ctrl.enableSmoothing(50.0f); // 50ms smoothing — quick changes to pitch seem to work fine with pdsp without smoothing enabled
    osc_amp.enableSmoothing(50.0f); // 50ms smoothing — solves click sound when stopping this instrument
    osc_amp.set(.5f);
    
//    engine.listDevices();
//    engine.setDeviceID(1);
//    engine.setup( 44100, 512, 3);
    
    
    enginePtr->listDevices();
    enginePtr->setDeviceID(1);
    enginePtr->setup( 44100, 512, 3);
}

void traber::keyPressed(int key){
}

//--------------------------------------------------------------
void traber::keyReleased(int key){
    if (key == 32) { // space key
        drawUI = !drawUI;
    } else if (key == 'r') {
        changeRecordMode(!recordMode);
    } else if (key == 's') {
        changeRunning(!running);
    }
}

void traber::changeRecordMode(const bool mode) {
    recordMode = mode;
    recorded = false;
    resetToStart();
    if (recordMode) {
        changeRunning(false);
    }
}

void traber::changeRunning(const bool run) {
    running = run;
    if (running) {
        osc_amp.set(.5f);
    } else {
        osc_amp.set(.0f);
    }
}

void traber::resetToStart() {
    fboTonesPlayed.begin();
        ofClear(255,255,255, 0); // Clear canvas when starting from 0
    fboTonesPlayed.end();
    fbo.begin();
        ofClear(255,255,255, 0); // Clear canvas when starting from 0
    fbo.end();
    index = 0;
    currentToneIndex = 0;
}

//--------------------------------------------------------------
void traber::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void traber::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void traber::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void traber::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void traber::windowResized(int w, int h){

}

//--------------------------------------------------------------
void traber::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void traber::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void traber::shutdownApp(){
//    soundStream.close();
//    lastBuffer.clear();
    grabber.close();
    fbo.clear();
    grayImage.clear();
//    audioMutex.unlock();
//    synth.reset();
}

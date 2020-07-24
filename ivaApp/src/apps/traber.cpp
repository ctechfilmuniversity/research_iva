// Based on slitscan example from: https://github.com/wearenocomputer/workshop

#include "traber.h"

//--------------------------------------------------------------
void traber::setup(){
    
    //ofSetWindowTitle("Slitscan example");
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
    
    fbo.allocate(fboSize, fboSize);
    fboTonesPlayed.allocate(fboSize, fboSize);
    grayImage.allocate(grabberWidth, grabberHeight);
    setupAudio();
}

// TODO: This function is too large, break it down into readable packages.
//--------------------------------------------------------------
void traber::update(){
    // "lastBuffer" is shared between update() and audioOut(), which are called
    // on two different threads. This lock makes sure we don't use lastBuffer
    // from both threads simultaneously (see the corresponding lock in audioOut())
    std::unique_lock<std::mutex> lock_name(audioMutex);
    
    // Grab the camera feed
    // and check for new frames
    grabber.update();
    if(grabber.isFrameNew()){
        
        // And draw a slice of the
        // new frame to the FBO
        updateFramebuffer();
        updateFramebufferTonesPlayed();
        updateFrequency();
        
        // Increase index or
        // x-offset position
        index < fboSize ? index++ : index=0;
    }
}

//--------------------------------------------------------------
void traber::updateFramebuffer() {
    float slice = grabber.getWidth() / float(fboSize);
    float offset = grabber.getWidth() * 0.5;
    
    
    ofPixels pixels;
    
    grabber.getTexture().readToPixels(pixels);
    pixels.setImageType(OF_IMAGE_GRAYSCALE);
    grayImage.setFromPixels(pixels);
    
    
    //grayImage.blurGaussian( 9 );
    //grayImage.threshold(150);
    
    fbo.begin();
        grabber.getTexture().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
    fbo.end();
}

void traber::updateFramebufferTonesPlayed() {
    fboTonesPlayed.begin();
        ofPolyline line{};
        line.addVertex(index, fboSize / synthTones.size() * calculateTone());
        line.addVertex(index, fboSize / synthTones.size() * (calculateTone() + 1));
    
        // Problems with alpha when switching between ui on and off
        //ofColor greenAlpha(13, 255, 100, 100);
        //ofSetColor(greenAlpha);
        //ofSetLineWidth(1);
        ofSetColor(ofColor::lightSeaGreen);
        line.draw();
        //ofSetLineWidth(1);
        ofSetColor(ofColor::white);
    fboTonesPlayed.end();
}

//--------------------------------------------------------------
void traber::updateFrequency() {
    int tone = calculateTone();
    synth.setFrequency(0, 220 * pow(2,(synthTones.at(tone)/12.f)));
};


//--------------------------------------------------------------
int traber::calculateTone() {
    // float limit = grayImage.getPixels().getColor(index, 0).limit(); // not used right now
    //cout << limit << " limit \n";
    
    // introduced to avoid dependencies
    auto stepSize{grabberHeight / 6};
    
    // TODO: use brightness to adjust amplitude?
    float currentMeanBrightness = 0;
    int currentToneIndex = 0;
    for (int i = 0; i < grabberHeight; i += stepSize) {
        
        float accumulatedBrightness = 0;
        for (int in = i; in < i+stepSize; in++) {
            accumulatedBrightness += grayImage.getPixels().getColor(grabber.getWidth() * 0.5, in).getBrightness();
            //cout << "brightness: " << grayImage.getPixels().getColor(index, in).getBrightness() << "\n";
        }
        float meanBrightness = accumulatedBrightness / stepSize;
        
        //cout << "index " << i << " accumulatedBrightness " << accumulatedBrightness << " meanBrightness " << meanBrightness << "\n";
        
        if (meanBrightness > currentMeanBrightness) {
            currentMeanBrightness = meanBrightness;
            //currentAmp = ofMap(meanBrightness, 0, limit, 0, 1);
            currentToneIndex = i / stepSize;
        }
    }
    return currentToneIndex;
}

//--------------------------------------------------------------
void traber::draw(){
    // debugDraw();

    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    if (drawUI) {
        fboTonesPlayed.draw(0, 0, ofGetWidth(), ofGetHeight());
        drawToneLines();
        drawPositionLine();
        drawPositionLineCurrentTone();
    }
}

void traber::drawToneLines() {
    ofColor whiteAlphaLine(255, 255, 255, 1);
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

void traber::drawPositionLineCurrentTone() {
    ofPolyline line{};
    line.addVertex(index, ofGetHeight() / synthTones.size() * calculateTone());
    line.addVertex(index, ofGetHeight() / synthTones.size() * (calculateTone() + 1));
    ofSetColor(ofColor::green);
    line.draw();
    ofSetColor(ofColor::white);
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
void traber::audioOut(ofSoundBuffer &outBuffer) {
    synth.fillSoundBuffer(outBuffer);
    
    // THREAD INFO
    // lock_name is the "var" name of the lock guard, kind of
    // a variable that is being constructed with a mutex (audioMutex),
    // locks the mutex at its construction and unlocks the mutex
    // when it is being destroyed, i.e., at the end of the scope
    std::unique_lock<std::mutex> lock_name(audioMutex);
    lastBuffer = outBuffer;
}

//--------------------------------------------------------------
void traber::setupAudio() {
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 1024; // increased buffer size seems necessary here
    settings.numBuffers = 4;
    settings.setOutListener(this);
    
    // the following setup function initiates the whole audio connection
    // it invokes the underlying RTAudioSoundStream to
    // - create an RtAudio object
    // - connect the object to the RtAudioCallback function
    // - start the stream and hence have a continious connection to audio in & out
    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
    
    synth.addOscillator(ofDCO::SINE);
    synth.setSampleRate(0, settings.sampleRate);
}

void traber::keyPressed(int key){
}

//--------------------------------------------------------------
void traber::keyReleased(int key){
    if (key == 32) { // space key
        drawUI = !drawUI;
    }
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
    soundStream.close();
    lastBuffer.clear();
    grabber.close();
    fbo.clear();
    grayImage.clear();
    audioMutex.unlock();
    synth.reset();
}

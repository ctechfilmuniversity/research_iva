    // Source:
    // https://github.com/wearenocomputer/workshop


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Slitscan example");
    ofBackground(33);
    
    // Init grabber
    grabberWidth = 640;
    grabberHeight = 480;
    
    grabber.initGrabber(grabberWidth, grabberHeight);
    
    // Init FBO
    // FBO sizes need to be a power of 2
    // Otherwise openFrameworks will scale
    // our FBO, which results in incorrect
    // mapping of texture coordinates
    fboSize = 512;
    index = 0;
    
    fbo.allocate(fboSize, fboSize);
    
    setupAudio();
}

//--------------------------------------------------------------
void ofApp::update(){
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
        
        float slice = grabber.getWidth() / float(fboSize);
        float offset = grabber.getWidth() * 0.5;
        
        ofPixels pixels = grabber.getPixels();
        pixels.setImageType(OF_IMAGE_GRAYSCALE);
        
        //grabber.getTexture().readToPixels(pixels);
        
        
        grayImage.setFromPixels(pixels);
        grayImage.blurGaussian( 9 );
        grayImage.threshold(96);
        //grayImage.draw(0, 300);
        
        fbo.begin();
        //grabber.getTexture().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());

        grayImage.getTexture().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
        fbo.end();
        
        float limit = grayImage.getPixels().getColor(index, 0).limit();
        //cout << limit << " limit \n";
    
        // introduced to avoid dependencies
        auto stepSize{grabberHeight / 6};
        
        for (int i = 0; i < grabberHeight; i += stepSize) {
            
            float accumulatedBrightness = 0;
            for (int in = i; in < i+stepSize; in++) {
                accumulatedBrightness += grayImage.getPixels().getColor(index, in).getBrightness();
                //cout << "brightness: " << grayImage.getPixels().getColor(index, in).getBrightness() << "\n";
            }
            float meanBrightness = accumulatedBrightness / stepSize;
            
            //cout << meanBrightness << "\n";
            
            float amp = ofMap(meanBrightness, 0, limit, 0, 1);
            //cout << amp << " amp \n";
            int synthIndex = i / stepSize;
            
            cout << "synth " << synthIndex << " amp " << amp << "\n";
            
            synths.at(synthIndex).setOverallAmplitude(amp);
        }
        
        
        // Increase index or
        // x-offset position
        index < fboSize ? index++ : index=0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    debugDraw();
//
    ofPushMatrix();
    ofTranslate(360, 50);

    fbo.draw(0, 0, 768, grabberHeight);

    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
//    ofPushMatrix();
//    ofTranslate(20, 30);
//    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
//    
//    grabber.draw(0, 20, grabberWidth, grabberHeight);
//    grayImage.draw(0, 250, grabberWidth, grabberHeight);
//    
//    ofPopMatrix();
    
}


//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    
    //synth.updateSoundBuffer(outBuffer);
    
//    for (int i=0; i<numOscillators; i++) {
//        synths.at(i).updateSoundBuffer(outBuffer);
//    }
    
    for (auto i = 0; i < outBuffer.getNumFrames(); i++) {
        auto sampleFull = synths.at(0).getSample(i);

        for (int osc=1; osc<numOscillators; osc++) {
            sampleFull += synths.at(osc).getSample(i);
        }

        // write the computed sample to the left and right channels
        outBuffer.getSample(i, 0) = sampleFull;
        outBuffer.getSample(i, 1) = sampleFull;
    }
    
    
    // THREAD INFO
    // lock_name is the "var" name of the lock guard, kind of
    // a variable that is being constructed with a mutex (audioMutex),
    // locks the mutex at its construction and unlocks the mutex
    // when it is being destroyed, i.e., at the end of the scope
    std::unique_lock<std::mutex> lock_name(audioMutex);
    lastBuffer = outBuffer;
}

//--------------------------------------------------------------
void ofApp::setupAudio() {
    
    
    
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    
    // the following setup function initiates the whole audio connection
    // it invokes the underlying RTAudioSoundStream to
    // - create an RtAudio object
    // - connect the object to the RtAudioCallback function
    // - start the stream and hence have a continious connection to audio in & out
    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
    
    //synth.setSampleRate(settings.sampleRate);
    
    
    // abrennec: Potential risk in a loop: numOscillators is 6, synthTones.size is >6
    for (int i=0; i<numOscillators; i++) {
        ofSynth currSynth = ofSynth();
        currSynth.setSampleRate(settings.sampleRate);
        currSynth.setFrequency(220 * pow(2,(synthTones.at(i)/12.f)));
        
        synths.insert(synths.end(),currSynth);
    }
}

void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

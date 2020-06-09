    // Source:
    // https://github.com/wearenocomputer/workshop


#include "traber.h"

//--------------------------------------------------------------
void traber::setup(){
    
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
    
    //        for (int i = 0; i < grabberHeight; i += stepSize) {
    //
    //            float accumulatedBrightness = 0;
    //            for (int in = i; in < i+stepSize; in++) {
    //                accumulatedBrightness += grayImage.getPixels().getColor(index, in).getBrightness();
    //                //cout << "brightness: " << grayImage.getPixels().getColor(index, in).getBrightness() << "\n";
    //            }
    //            float meanBrightness = accumulatedBrightness / stepSize;
    //
    //            //cout << meanBrightness << "\n";
    //
    //            float amp = ofMap(meanBrightness, 0, limit, 0, 1);
    //            //cout << amp << " amp \n";
    //            int synthIndex = i / stepSize;
    //
    //            cout << "synth " << synthIndex << " amp " << amp << "\n";
    //
    //            synths.at(synthIndex).setOverallAmplitude(amp);
    //        }
    
    float currentMeanBrightness = 0;
    //float currentAmp = 0;
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
//    debugDraw();
//    ofPushMatrix();
//    ofTranslate(360, 50);
//    fbo.draw(0,0);
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
//    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());

//    ofPopMatrix();
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
    
    //synth.updateSoundBuffer(outBuffer);
    
//    for (int i=0; i<numOscillators; i++) {
//        synths.at(i).updateSoundBuffer(outBuffer);
//    }
    
//    for (auto i = 0; i < outBuffer.getNumFrames(); i++) {
//        auto sampleFull = synths.at(0).getSample(i);
//
//        for (int osc=1; osc<numOscillators; osc++) {
//            sampleFull += synths.at(osc).getSample(i);
//        }
//
//        // write the computed sample to the left and right channels
//        outBuffer.getSample(i, 0) = sampleFull;
//        outBuffer.getSample(i, 1) = sampleFull;
//    }
    
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
    // TODO: The dropouts are likely to be caused by the buffer size being too small. Try again with 1024.
    // I guess that the dropout is caused by the buffer not being filled fast enough. When playback / audio out reads from the buffer, there are empty spots, i.e., sample values jump from 0.75 (or whatever) to 0.0. This causes the dropout. It might as well be an amplitude value > 1.0/-1.0 but since this does not happen and would potentially sound slightly different, the reason might be a small buffer size. To investigate this further, please try to identify a recreation scenario and file that one as a bug report. Also, increase the buffer size and see whether this fixes the issue. Latency might be a negative sideeffect .. ; -) But let's see.
    
    // MT: Increased buffer size as suggested by Angela solved the problem for this app
    
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 1024;
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
//    for (int i=0; i<numOscillators; i++) {
//        ofSynth currSynth = ofSynth();
//        currSynth.setSampleRate(settings.sampleRate);
//        currSynth.setFrequency(220 * pow(2,(synthTones.at(i)/12.f)));
//
//        synths.insert(synths.end(),currSynth);
//    }
    synth.addOscillator(ofDCO::SINE);
    synth.setSampleRate(0, settings.sampleRate);
}

void traber::keyPressed(int key){
}

//--------------------------------------------------------------
void traber::keyReleased(int key){
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

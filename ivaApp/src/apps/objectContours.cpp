//
//  objectContours.cpp
//  ivaApp
//
//  Created by Markus Traber on 10.08.20.
//

#include "objectContours.hpp"

#include "objectContours.hpp"

//--------------------------------------------------------------
void objectContours::setup(){
    ofSetWindowShape(1024, 786);
    ofBackground(33);
    ofSetLineWidth(1);
    
    vidGrabber.setVerbose(true);
    vidGrabber.setup(320,240);
    
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);

    bLearnBakground = true;
    threshold = 80;
    
    maxAreaSize = 320 * 240; // video size
    
    setupAudio();
}

//--------------------------------------------------------------
void objectContours::update(){
    // "lastBuffer" is shared between update() and audioOut(), which are called
    // on two different threads. This lock makes sure we don't use lastBuffer
    // from both threads simultaneously (see the corresponding lock in audioOut())
    std::unique_lock<std::mutex> lock_name(audioMutex);
    
    ofBackground(100,100,100);

    bool bNewFrame = false;

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame){

        colorImg.setFromPixels(vidGrabber.getPixels());

        grayImage = colorImg;
        if (bLearnBakground == true){
            grayBg = grayImage;        // the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }
        
        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);

        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        // limited to max 10 contours -> 10 synths
        contourFinder.findContours(grayDiff, 20, (340*240)/3, numSynths, false);
    }
}

//--------------------------------------------------------------
void objectContours::draw(){
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    colorImg.draw(20,20);
    grayImage.draw(360,20);
    grayBg.draw(20,280);
    grayDiff.draw(360,280);

    // then draw the contours:

    ofFill();
    ofSetHexColor(0x333333);
    ofDrawRectangle(360,540,320,240);
    ofSetHexColor(0xffffff);

    // we could draw the whole contour finder
    //contourFinder.draw(360,540);

    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
        
//        cout << contourFinder.blobs[i].area << endl;
//        cout << contourFinder.blobs[i].length << endl;
//        cout << contourFinder.blobs[i].nPts << endl;
        
        // draw over the centroid if the blob is a hole
        ofSetColor(255);
//        if(contourFinder.blobs[i].hole){
//            ofDrawBitmapString("hole",
//                contourFinder.blobs[i].boundingRect.getCenter().x + 360,
//                contourFinder.blobs[i].boundingRect.getCenter().y + 540);
//        }
        
        
//        ofDrawBitmapString(contourFinder.blobs[i].area,
//            contourFinder.blobs[i].boundingRect.getCenter().x + 360,
//            contourFinder.blobs[i].boundingRect.getCenter().y + 540);
//        ofDrawBitmapString(i,
//            contourFinder.blobs[i].boundingRect.getCenter().x + 360,
//            contourFinder.blobs[i].boundingRect.getCenter().y + 540);
        
        
        updateSynth(i);
            
    }
    
    // see updateSynth
//    if (contourFinder.nBlobs < 10) {
//        for (int i = contourFinder.nBlobs; i < 10; i++){
//            synth.setAmplitude(i, 0);
//        }
//    }

    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << "threshold " << threshold << " (press: +/-)" << endl
              << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, 600);
}

//--------------------------------------------------------------
void objectContours::updateSynth(const int i){
    switch (mode) {
        case TONE_MODE: {
            break;
        }
        case FREQ_HEIGHT_MODE: {
            float freq = ofMap(contourFinder.blobs[i].boundingRect.getCenter().y, 0, 240, 20, 8000);
            float amp = ofMap(contourFinder.blobs[i].area, 1, maxAreaSize, 0, .9);
            synth.setAmplitude(i,amp);
            synth.setFrequency(i, freq);
            ofDrawBitmapString(freq,
                contourFinder.blobs[i].boundingRect.getCenter().x + 360,
                contourFinder.blobs[i].boundingRect.getCenter().y + 540);
            break;
        }
        case FREQ_AREA_MODE: {
            float freq = ofMap(contourFinder.blobs[i].area, 1, maxAreaSize, 20, 20000);
            if (synth.getAmplitude(i) != 0.5) {
                synth.setAmplitude(i, 0.5);
            }
            synth.setFrequency(i, freq);
            ofDrawBitmapString(freq,
                contourFinder.blobs[i].boundingRect.getCenter().x + 360,
                contourFinder.blobs[i].boundingRect.getCenter().y + 540);
            break;
        }
        default: {
            cout << "Unknown mode is set: " << mode << endl;
            break;
        }
    }
    if (i+1 == contourFinder.nBlobs && contourFinder.nBlobs < 10) {
        for (int i = contourFinder.nBlobs; i < 10; i++){
            synth.setAmplitude(i, 0);
        }
    }
}


//--------------------------------------------------------------
void objectContours::audioOut(ofSoundBuffer &outBuffer) {
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
void objectContours::setupAudio() {
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512; // increased buffer size seems necessary here
    settings.numBuffers = 4;
    settings.setOutListener(this);
    
    // the following setup function initiates the whole audio connection
    // it invokes the underlying RTAudioSoundStream to
    // - create an RtAudio object
    // - connect the object to the RtAudioCallback function
    // - start the stream and hence have a continious connection to audio in & out
    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
    
    for (int i=0; i<numSynths; i++) {
        synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, synth.FUNDAMENTAL_FREQ, 0);
    }
}

void objectContours::keyPressed(int key){
    switch (key){
        case ' ':
            bLearnBakground = true;
            break;
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
    }
}

//--------------------------------------------------------------
void objectContours::keyReleased(int key){
}

//--------------------------------------------------------------
void objectContours::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void objectContours::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void objectContours::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void objectContours::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void objectContours::windowResized(int w, int h){

}

//--------------------------------------------------------------
void objectContours::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void objectContours::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void objectContours::shutdownApp(){
    audioMutex.unlock();
    synth.reset();
}

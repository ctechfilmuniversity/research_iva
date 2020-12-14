//
//  objectContours.cpp
//  ivaApp
//
//  Created by Markus Traber on 10.08.20.
//

#include "objectContours.hpp"

//--------------------------------------------------------------
void objectContours::setup(){
    ofSetWindowShape(1024, 786);
    ofBackground(33);
    ofSetLineWidth(1);

    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(0);
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
    contourFinder.draw(360,540);

    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        //contourFinder.blobs[i].draw(360,540);

        //        cout << contourFinder.blobs[i].area << endl;
        //        cout << contourFinder.blobs[i].length << endl;
        //        cout << contourFinder.blobs[i].nPts << endl;

        // draw over the centroid if the blob is a hole

        //        if(contourFinder.blobs[i].hole){
        //            ofDrawBitmapString("hole",
        //                contourFinder.blobs[i].boundingRect.getCenter().x + 360,
        //                contourFinder.blobs[i].boundingRect.getCenter().y + 540);
        //        }
        updateSynth(i);
    }
    muteRemainingSynths();

    string modeStr = "Mode: Frequency Area Mode";
    switch (mode) {
        case TONE_MODE: {
            modeStr = "Mode: Tone Mode";
            break;
        }
        case FREQ_HEIGHT_MODE: {
            modeStr = "Mode: Frequency Height Mode";
            break;
        }
        case FREQ_AREA_MODE:
        default: {
            modeStr = "Mode: Frequency Area Mode";
            break;
        }
    }

    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << "threshold " << threshold << " (press: +/-)" << endl
              << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate() << endl
              << modeStr << endl
              << "Press 'm' to toggle mode";
    ofDrawBitmapString(reportStr.str(), 20, 600);
}

//--------------------------------------------------------------
void objectContours::updateSynth(const int i){
    ofSetColor(255);
    switch (mode) {
        case TONE_MODE: {
            float amp = ofMap(contourFinder.blobs[i].area, 1, maxAreaSize, .2, .9);
            float freq = 220 * pow(2,(synthTones.at(i)/12.f));
            amp_ctrls[i].set(amp);
            pitch_ctrls[i].set(freq);
            ofDrawBitmapString(freq,
                contourFinder.blobs[i].boundingRect.getCenter().x + 360,
                contourFinder.blobs[i].boundingRect.getCenter().y + 540);
            break;
        }
        case FREQ_HEIGHT_MODE: {
            float freq = ofMap(contourFinder.blobs[i].boundingRect.getCenter().y, 240, 0, 20, 8000);
            float amp = ofMap(contourFinder.blobs[i].area, 1, maxAreaSize, .2, .9);
            amp_ctrls[i].set(amp);
            pitch_ctrls[i].set(freq);
            ofDrawBitmapString(freq,
                contourFinder.blobs[i].boundingRect.getCenter().x + 360,
                contourFinder.blobs[i].boundingRect.getCenter().y + 540);
            break;
        }
        case FREQ_AREA_MODE: {
            float freq = ofMap(contourFinder.blobs[i].area, 1, maxAreaSize, 20, 20000);
            for (int i = 0; i < numSynths; i++){
                amp_ctrls[i].set(.5f);
            }
            pitch_ctrls[i].set(freq);
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
}

//--------------------------------------------------------------
void objectContours::muteRemainingSynths(){
    if (contourFinder.nBlobs < 10) {
        for (int i = contourFinder.nBlobs; i < 10; i++){
            amp_ctrls[i].set(.0f);
        }
    }
}

//--------------------------------------------------------------
void objectContours::setupAudio() {
    pitch_ctrls.resize(numSynths);
    amp_ctrls.resize(numSynths);
    sine_oscs.resize(numSynths);
    phasors.resize(numSynths);

    for (int i=0; i<numSynths; i++) {
        pitch_ctrls[i].set(172.5f);
        amp_ctrls[i].set(.5f); // set initially to a value for initial mode

        pitch_ctrls[i] >> phasors[i].in_freq();
        phasors[i].out_sync() >> sine_oscs[i].in_sync();

        sine_oscs[i].out_sine() >> amp_ctrls[i] >> enginePtr->audio_out(0);
        sine_oscs[i].out_sine() >> amp_ctrls[i] >> enginePtr->audio_out(1);
    }
}

//--------------------------------------------------------------
void objectContours::toggleMode() {
    switch (mode) {
        case TONE_MODE:
            mode = FREQ_HEIGHT_MODE;
            break;
        case FREQ_HEIGHT_MODE:
            mode = FREQ_AREA_MODE;
            break;
        case FREQ_AREA_MODE:
        default:
            mode = TONE_MODE;
            break;
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
        case 'm':
            toggleMode();
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
}

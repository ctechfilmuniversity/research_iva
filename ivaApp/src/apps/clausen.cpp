    // Source:
    // https://github.com/wearenocomputer/workshop
    // https://github.com/ofZach/avsys2012/tree/master/week1_audioAndSoundBufferTogether


#include "clausen.hpp"

//--------------------------------------------------------------
void clausen::setup(){
    
    width = 512;
    height = 512;
    
    ofBackground(33);
    
    // Init grabber
    
    grabber.initGrabber(320, 240);
    
    // Init FBO
    // FBO sizes need to be a power of 2
    // Otherwise openFrameworks will scale
    // our FBO, which results in incorrect
    // mapping of texture coordinates
    
    fboSize = width;
    
    resetData();
    
    fbo.allocate(fboSize, fboSize);

    imageCv.allocate(512, 512);
    imageGray.allocate(512, 512);
    tex.allocate(512, 512, GL_RGB);

    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    
    soundStream.setup(settings);
    
    sampleCount = 0;
}

//--------------------------------------------------------------
void clausen::update(){
    
    // Grab the camera feed
    // and check for new frames
    
    if (!captured) {
        grabber.update();
        if(grabber.isFrameNew() && index <= fboSize){
        
            // And draw a slice of the
            // new frame to the FBO
        
            float slice = grabber.getWidth() / float(fboSize);
            float offset = grabber.getWidth() * 0.5;
        
            fbo.begin();
            grabber.getTexture().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
            fbo.end();
        
            // Increase index or
            // x-offset position
        
            if (index < fboSize) {
                index++;
            } else {
                captured = true;
            }
        }
    }
    
    
    if (captured && !filtered) {
        tex = fbo.getTexture();
        ofPixels pixels;
        tex.readToPixels(pixels);
        pixels.setImageType(OF_IMAGE_COLOR);
        imageCv.setFromPixels(pixels);
        
        imageGray = imageCv;
        //imageGray.threshold( 128 );
        imageGray.blurHeavily();
        
        
        pixelBuffer = new unsigned char[ width * height ];
        
        /*
        for (int i = 0; i < width*height; i++) {
            pixelBuffer[i] = imageGray.getPixels()[i];
        }
        */
        
        int fillBuffer = 0;
        for (int i = 0; i < width; i++) {
            for (int j = i; j < width*height; j+=width){
                pixelBuffer[fillBuffer] = imageGray.getPixels()[j];
                fillBuffer++;
            }
        }
        
        filtered = true;
    }
    
    
    if(captured & filtered) {
        tex.loadData(pixelBuffer, width, height, GL_LUMINANCE);
    }

}

//--------------------------------------------------------------
void clausen::draw(){
    
    //debugDraw();
    
    ofPushMatrix();
    //ofTranslate(360, 50);
    
    if (!filtered) {
        fbo.draw(0, 0);
    } else {
        imageGray.draw(0, 0);
        //tex.draw(0,0);
    }
    
    if (filtered) {
        for (int i = 0; i < 512; i++){
            int samples = sampleCount - i;
            if (samples < 0) samples += width*height;
            int x = samples / width;
            int y = samples % width;
        
            ofPushStyle();
            ofSetColor(255,0,0);
            ofDrawRectangle(x,y, 2,2);
            ofPopStyle();
        }
    }
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void clausen::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    grabber.draw(0, 0, width/4, height/4);
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void clausen::keyPressed(int key){
    if (key == 'n') {
        resetData();
    }
    
    /*
    if (key == 'a'){
        float val = ofMap(mouseX, 0, ofGetWidth(), 1, 1000.0, true);
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                int index = i * width + j;
                    pixelBuffer[index] = ofSignedNoise(i/val, j/val, ofGetElapsedTimef()*10.0) * 127 + 127;
            }
        }
    } else if (key == 'b'){
        for (int i = 0; i < width*height; i++){
            if (i % max(mouseX,1) == 0){
                pixelBuffer[i] = 255;
            } else {
                pixelBuffer[i] = 0;
            }
        }
    } else if (key == 'c'){
        float val = ofRandom(0.01, 0.9999);


        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                int index = i * width + j;
                if ( j % (int)(val*100) == 0 || i % (int)(val*100) == 0){
                    pixelBuffer[index] = ofRandom(0,255);
                } else {
                    pixelBuffer[index] = 0;
                }
            }
        }
    }
    */

}


void clausen::audioOut(ofSoundBuffer &outBuffer){
    if(filtered) {
        for (auto i = 0; i < outBuffer.getNumFrames(); i++) {
            float val = ofMap(pixelBuffer[sampleCount],0,255,-1,1);
            
            // output[i*nChannels    ] = val * 0.1;
            // output[i*nChannels + 1] = val * 0.1;
            
            outBuffer.getSample(i, 0) = val * 0.1;
            outBuffer.getSample(i, 1) = val * 0.1;
            
            sampleCount++;
            
            sampleCount %= width*height;
        }
    }
    
    return;
}

void clausen::shutdownApp() {
    soundStream.close();
    grabber.close();
    resetData();
}

void clausen::resetData() {
    index = 0;
    captured = false;
    filtered = false;
}

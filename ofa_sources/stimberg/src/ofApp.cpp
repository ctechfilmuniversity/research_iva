// Based on: ofxCV addon example

#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup()
{
	ofSetVerticalSync(true);
//    ofSetFrameRate(120);
	cam.setup(640, 480);
	
//	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	stepSize = 10;
	ySteps = cam.getHeight() / stepSize;
	xSteps = cam.getWidth() / stepSize;
    
    
	for (int y = 0; y < ySteps; y++)
    {
		for (int x = 0; x < xSteps; x++)
        {
            flowAmount.push_back(ofVec2f(0,0));
		}
	}
    
//    synth = ofSynth(4);
    
    
    
    setupAudio();
    
    ofHideCursor();
    
    
    int oscNums = 4;
    
    float freq1 = 110;
    synth.setFrequency(1, freq1);
    
    float freq2 = 220 * pow(2,(3/12.f));
    synth.setFrequency(2, freq2);
    
    float freq3 = 220 * pow(2,(10/12.f));
    synth.setFrequency(3, freq3);
    
    float freq4 = 220 * pow(2,(7/12.f));
    synth.setFrequency(4, freq4);
   

    float freq5 = 55;
    synth.setFrequency(5, freq5);

    float freq6 = 220 * pow(2,(19/12.f));
    synth.setFrequency(6, freq6);

    float freq7 = 220 * pow(2,(14/12.f));
    synth.setFrequency(7, freq7);

    float freq8 = 220 * pow(2,(15/12.f));
    synth.setFrequency(8, freq8);
    
    
    
    // set initial oscillator amplification
//    synthAmp1 = 0.5;
    
//    synth1.setAmplitude(1, synthAmp1);
//    synth1.setAmplitude(2, 0);
//    synth1.setAmplitude(3, synthAmp1);
    

}

void ofApp::update()
{
	cam.update();

    if(cam.isFrameNew())
    {
//        cam.mirror(false, true);
        ofxCvColorImage colorImg;
        colorImg.setFromPixels(cam.getPixels());
        colorImg.mirror(false, true);
//        ofxCvGrayscaleImage grayImg;
//        grayImg = colorImg;
//        grayImg.threshold(100);
        
        
        // TODO: add documentation
		flow.setWindowSize(stepSize);
		flow.calcOpticalFlow(colorImg);
        
		int i = 0;
        int iTotal = xSteps * ySteps;
		float distortionStrength = 6;
        
//        vector<float> newAmp[8];
        float newAmp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        
//        for(int i = 0; i < oscNums; i++) {
//            newAmp[i] = 0.0;
//        }
        
		for(int y = 1; y + 1 < ySteps; y++)
        {
			for(int x = 1; x + 1 < xSteps; x++)
            {
                
				int i = y * xSteps + x;
                glm::vec2 position(x * stepSize, y * stepSize);
                ofRectangle area(position - glm::vec2(stepSize, stepSize) / 2, stepSize, stepSize);
//                glm::vec2 offset = flow.getTotalFlowInRegion(area);
                ofVec2f offset = flow.getAverageFlowInRegion(area);
                
//                flowAmount[i] = offset;
                
                if(offset.length() < 3) {
//                    offset = ofVec2f(0, 0);
                    offset.scale(ofMap(offset.length(), 0, 3, 0.0, 0.1));
                } else {
//                    offset.normalize();
                    offset.scale(ofMap(offset.length(), 0, 10, 0.0, 1.0));
                    
                    ofVec2f v1(-1,1);
//                    ofVec2f v2(0,1);
                    float angle = v1.angle(offset)+180;
                    
                    ofLogNotice(ofToString(angle));

                    
                    if (angle < 90) {
                        newAmp[0] += 1.0 / iTotal;
                        newAmp[4] += offset.length() / iTotal * 0.25;
                    } else if (angle < 180) {
                        newAmp[1] += 1.0 / iTotal;
                        newAmp[5] += offset.length() / iTotal * 0.25;
                    } else if (angle < 270) {
                        newAmp[2] += 1.0 / iTotal;
                        newAmp[6] += offset.length() / iTotal * 0.25;
                    } else  {
                        newAmp[3] += 1.0 / iTotal;
                        newAmp[7] += offset.length() / iTotal * 0.25;
                    }
                }
                
                flowAmount[i] = offset;
                
//                mesh.setVertex(i, glm::vec3(position + distortionStrength * offset, 0));
				i++;
			}
		}
        
//        ofLogNotice(ofToString(oscAmp));
        
        for (int i = 0; i < 8; i++) {
            float amp = synth.getAmplitude(i) * 0.8 + newAmp[i] * 0.2;
//            float amp = ofLerp(synth.getAmplitude(i), newAmp[i], 0.1);
            synth.setAmplitude(i, amp);
        }
        
        
//        synthAmp1 = synthAmp1 * 0.7 + newAmp1 * 0.3;
//        synthAmp2 = synthAmp2 * 0.7 + newAmp2 * 0.3;
//        synthAmp3 = synthAmp3 * 0.7 + newAmp3 * 0.3;
//        synthAmp4 = synthAmp4 * 0.7 + newAmp4 * 0.3;
        

	}
    
//    synth1.setAmplitude(1, synthAmp1);
//    synth1.setAmplitude(3, synthAmp1);
    

}

void ofApp::draw()
{
    
	ofBackground(0);
	//ofScale(2.0, 2.0);

    // TODO: What is happening here?
    // Why is this call necessary? What is the result?
    // Why don't you simply call cam.draw() instead?
    // What might be an additional use case for texture binding?
//    cam.getTexture().bind();
//	mesh.draw();
    
//    grayImg.draw(680,0);
//    cam.draw(680,0);
    
    float xScale = ofGetScreenWidth() / cam.getWidth();
    float yScale = ofGetScreenHeight() / cam.getHeight();
    
    int i = 0;
    for(int y = 1; y + 1 < ySteps; y++)
    {
        for(int x = 1; x + 1 < xSteps; x++)
        {
            int i = y * xSteps + x;
            
            auto shift_x = stepSize * x * xScale;
            auto shift_y = stepSize * y * yScale;
            
//            ofRectangle(shift_x, shift_y, );
            
            ofSetLineWidth(1);
            
            ofDrawLine(shift_x, shift_y, shift_x + (flowAmount[i].x * stepSize * xScale), shift_y + (flowAmount[i].y * stepSize * yScale));
            i++;
            
        }
    }
//    cam.getTexture().unbind();
    
    
	if (! ofGetMousePressed() )
    {
		//mesh.drawFaces();
//        mesh.drawWireframe();
	}
}



//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    
    synth.updateSoundBuffer(outBuffer);

    
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
    
//    synth1.setSampleRate(settings.sampleRate);

}

// Based on: ofxCV addon example

#include "stimberg.h"

void stimberg::setup() {
    ofSetVerticalSync(true);
//    ofSetFrameRate(60);
    cam.setup(640, 480);
    
//    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
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
    
    colorImg.allocate(640, 480);
    flow.setWindowSize(stepSize);
    
    setupAudio();
    ofHideCursor();
    setupSynth();
}

void stimberg::update()
{
    cam.update();

    if(cam.isFrameNew())
    {
        colorImg.setFromPixels(cam.getPixels());
        colorImg.mirror(false, true);

        flow.calcOpticalFlow(colorImg);
        
        int i = 0;
        int iTotal = xSteps * ySteps;
        float distortionStrength = 6;
        
        float newAmp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        
        for(int y = 1; y + 1 < ySteps; y++)
        {
            for(int x = 1; x + 1 < xSteps; x++)
            {
                
                int i = y * xSteps + x;
                glm::vec2 position(x * stepSize, y * stepSize);
                ofRectangle area(position - glm::vec2(stepSize, stepSize) / 2, stepSize, stepSize);
                ofVec2f offset = flow.getAverageFlowInRegion(area);
                
                
                if(offset.length() < 3) {
                    offset.scale(ofMap(offset.length(), 0, 3, 0.0, 0.1));
                } else {
                    offset.scale(ofMap(offset.length(), 0, 10, 0.0, 1.0));
                    
                    ofVec2f v1(-1,1);
                    float angle = v1.angle(offset)+180;
                    
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
                i++;
            }
        }
        
        for (int i = 0; i < 8; i++) {
            float amp = synth2.getAmplitude(i) * 0.8 + newAmp[i] * 0.2;
            synth2.setAmplitude(i, amp);
        }
        

    }

}

void stimberg::draw()
{
    
    ofBackground(0);
    //ofScale(2.0, 2.0);

    // TODO: What is happening here?
    // Why is this call necessary? What is the result?
    // Why don't you simply call cam.draw() instead?
    // What might be an additional use case for texture binding?
//    cam.getTexture().bind();
//    mesh.draw();
    
//    grayImg.draw(680,0);
//    cam.draw(680,0);
    
    // Uncommented becaus we need it to fit into the window
    //    float xScale = ofGetScreenWidth() / cam.getWidth();
    //    float yScale = ofGetScreenHeight() / cam.getHeight();
    
    // Adaption to make it fit into the window
    float xScale = ofGetWidth() / cam.getWidth();
    float yScale = ofGetHeight() / cam.getHeight();
    
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
void stimberg::audioOut(ofSoundBuffer &outBuffer) {
    
    //synth.updateSoundBuffer(outBuffer);
    synth2.fillSoundBuffer(outBuffer);
    
    // THREAD INFO
    // lock_name is the "var" name of the lock guard, kind of
    // a variable that is being constructed with a mutex (audioMutex),
    // locks the mutex at its construction and unlocks the mutex
    // when it is being destroyed, i.e., at the end of the scope
    std::unique_lock<std::mutex> lock_name(audioMutex);
    lastBuffer = outBuffer;
}



//--------------------------------------------------------------
void stimberg::setupAudio() {
    
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
}

//--------------------------------------------------------------
void stimberg::setupSynth() {
    const std::array<float, 8> frequencies = {
        110,
        220 * pow(2.f,(3/12.f)),
        220 * pow(2.f,(10/12.f)),
        220 * pow(2.f,(7/12.f)),
        55,
        220 * pow(2.f,(19/12.f)),
        220 * pow(2.f,(14/12.f)),
        220 * pow(2.f,(15/12.f))
    };
    
    for (auto frequency : frequencies) {
        synth2.addOscillator(ofDCO::SINE, synth2.SAMPLE_RATE, frequency, 0.0);
    }
}

//--------------------------------------------------------------
void stimberg::shutdownApp(){
    soundStream.close();
    lastBuffer.clear();
    cam.close();
    audioMutex.unlock();
    synth2.reset();
}

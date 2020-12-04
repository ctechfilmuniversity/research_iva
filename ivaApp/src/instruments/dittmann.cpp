//#include "dittmann.h"
//
//
//struct Bin
//{
//    float min;
//    float max;
//    int count;
//};
//
////--------------------------------------------------------------
//void dittmann::setup()
//{
//
//    // ofSetLogLevel(OF_LOG_VERBOSE);
//    ofSetVerticalSync(true);
//    ofSetWindowShape(1280, 480);
//
//    grabber.setup(grabWidth, grabHeight);
//
//    setupAudio();
//    setupGui();
//    setupMidi();
//}
//
////--------------------------------------------------------------
//void dittmann::update()
//{
//    grabber.update();
//
//    waveform.clear();
//
//    // "lastBuffer" is shared between update() and audioOut(), which are called
//    // on two different threads. This lock makes sure we don't use lastBuffer
//    // from both threads simultaneously (see the corresponding lock in audioOut())
//    std::unique_lock<std::mutex> lock_name(audioMutex);
//
//    // this loop is building up a polyline representing the audio contained in
//    // the left channel of the buffer
//    // the x coordinates are evenly spaced on a grid from 0 to the window's width
//    // the y coordinates map each audio sample's range (-1 to 1) to the height
//    // of the window
//    for (size_t i = 0; i < lastBuffer.getNumFrames(); i++)
//    {
//
//        float sample = lastBuffer.getSample(i, 0);
//        float x = ofMap(i, 0, lastBuffer.getNumFrames(), 0, ofGetWidth());
//        float y = ofMap(sample, -1, 1, 0, ofGetHeight());
//        waveform.addVertex(x, y);
//    }
//
//    rms = lastBuffer.getRMSAmplitude();
//}
//
////--------------------------------------------------------------
//void dittmann::draw()
//{
//
//    ofBackground(ofColor::black);
//
//    grabber.draw(0, 0);
//
//    auto stepSize = 10;
//
//    // binning for rotation angles
//    const size_t brightnessSteps = synth.getNumberOfOscillators();
//    const float brightnessStepWidth = 255.0 / brightnessSteps;
//
//    std::vector<Bin> bins;
//    for (int step = 0; step < brightnessSteps; step++)
//    {
//        bins.push_back({step * brightnessStepWidth, (step + 1) * brightnessStepWidth, 0});
//    }
//
//    int count = 0;
//
//    ofSetColor(ofColor::lightGray);
//    ofSetLineWidth(1);
//    for (auto x{0}; x < grabber.getWidth(); x += stepSize)
//    {
//
//        for (auto y{0}; y < grabber.getHeight(); y += stepSize)
//        {
//
//            auto brightness = grabber.getPixels().getColor(x, y).getBrightness();
//            for (auto& bin : bins)
//            {
//                if (brightness >= bin.min && brightness < bin.max)
//                {
//                    bin.count += 1;
//                }
//            }
//            count += 1;
//            auto angle = ofMap(brightness, 0, 255, 0, TWO_PI);
//            auto shift_x = x + grabWidth;
//
//            // TODO: Explain the calculation here
//            // Why are cosine and sine operations applied here?
//            // What happens if you change the operations?
//            // TODO: Add sound
//            // How can you connect this with a synth or oscillator?
//
//            ofDrawLine(shift_x, y, shift_x + stepSize * cos(angle), y + stepSize * sin(angle));
//            // ofDrawCircle(i + 640, j, ofMap(brightness, 0, 255, 4,4.8));
//        }
//    }
//
//    for (auto i = 0; i < bins.size(); i++)
//    {
//        float scale = (float)bins[i].count / (float)count;
//        synth.setAmplitude(i, scale);
//    }
//
//    ofSetColor(ofColor::white);
//    ofSetLineWidth(1 + (rms * 30.));
//    waveform.draw();
//
//    // gui stuff
//    drawHelpText();
//    gui.draw();
//}
//
////--------------------------------------------------------------
//void dittmann::audioOut(ofSoundBuffer& outBuffer)
//{
//
//    synth.fillSoundBuffer(outBuffer);
//
//    // THREAD INFO
//    // lock_name is the "var" name of the lock guard, kind of
//    // a variable that is being constructed with a mutex (audioMutex),
//    // locks the mutex at its construction and unlocks the mutex
//    // when it is being destroyed, i.e., at the end of the scope
//    std::unique_lock<std::mutex> lock_name(audioMutex);
//    lastBuffer = outBuffer;
//}
//
////--------------------------------------------------------------
//void dittmann::exit()
//{
//    // clean up
//    midiIn.closePort();
//    midiIn.removeListener(this);
//    frequencySlider.removeListener(this, &dittmann::updateGui);
//}
//
////--------------------------------------------------------------
//void dittmann::keyPressed(int key)
//{
//
//    if (key == '1' && !changeAmp)
//    {
//
//        changeAmp = true;
//        oscNum = 1;
//
//        // TODO: Improve the interaction with mouse move
//        // For details see ofApp::mouseMoved
//        initialMouseDistY = ofGetMouseY() - ofGetWindowPositionY();
//    }
//    else if (key == '2' && !changeAmp)
//    {
//
//        changeAmp = true;
//        oscNum = 2;
//
//        // TODO: Improve the interaction with mouse move
//        // For details see ofApp::mouseMoved
//        initialMouseDistY = ofGetMouseY() - ofGetWindowPositionY();
//    }
//    else if (key == '3' && !changeAmp)
//    {
//
//        changeAmp = true;
//        oscNum = 3;
//
//        // TODO: Improve the interaction with mouse move
//        // For details see ofApp::mouseMoved
//        initialMouseDistY = ofGetMouseY() - ofGetWindowPositionY();
//    }
//    else if (key == 'a')
//    {
//
//        float freq = 220 * pow(2, (-5 / 12.f));
//        updateGui(freq);
//    }
//    else if (key == 's')
//    {
//
//        float freq = 220 * pow(2, (-2 / 12.f));
//        updateGui(freq);
//    }
//    else if (key == 'd')
//    {
//
//        float freq = 220;
//        updateGui(freq);
//    }
//    else if (key == 'f')
//    {
//
//        float freq = 220 * pow(2, (3 / 12.f));
//        updateGui(freq);
//    }
//    else if (key == 'g')
//    {
//
//        float freq = 220 * pow(2, (5 / 12.f));
//        // ofLogNotice("Freq is : " + ofToString(freq));
//        updateGui(freq);
//    }
//    else if (key == 'h')
//    {
//
//        float freq = 220 * pow(2, (7 / 12.0));
//        updateGui(freq);
//    }
//    else if (key == 'j')
//    {
//
//        float freq = 220 * pow(2, (10 / 12.f));
//        updateGui(freq);
//    }
//    else if (key == 'k')
//    {
//
//        float freq = 220 * pow(2, (12 / 12.f));
//        updateGui(freq);
//    }
//    else if (key == 'l')
//    {
//
//        float freq = 220 * pow(2, (15 / 12.f));
//        updateGui(freq);
//    }
//}
//
////--------------------------------------------------------------
//void dittmann::keyReleased(int key)
//{
//
//    if (key == '1' || key == '2' || key == '3')
//    {
//
//        changeAmp = false;
//    }
//}
//
////--------------------------------------------------------------
//void dittmann::mouseMoved(int x, int y)
//{
//
//    if (changeAmp)
//    {
//
//        // TODO: Improve the interaction with mouse move
//        // To do so, ensure that the amplitude per oscillator
//        // only changes when the mouse cursor is inside of the window.
//        // Ensure furthermore that whenever the direction changes
//        // the amplitude increases or decreases automatically.
//        // Check out the following functions to fix this issue:
//        // - ofMap
//        // - ofNormalize
//        int currentMouseDistY = y + ofGetWindowPositionY();
//
//        float amp = ofMap(currentMouseDistY, 0, ofGetScreenHeight(), 1.0, 0.0);
//
//        //        ofLogNotice("Set Osc No. " + ofToString(oscNum) + " to amp " + ofToString(amp));
//        //        ofLogNotice("MouseY: " + ofToString(y) + " | WindowPosition " + ofToString(ofGetWindowPositionY()));
//    }
//}
//
////--------------------------------------------------------------
//void dittmann::mouseDragged(int x, int y, int button) {}
//
////--------------------------------------------------------------
//void dittmann::mousePressed(int x, int y, int button) {}
//
////--------------------------------------------------------------
//void dittmann::mouseReleased(int x, int y, int button) {}
//
////--------------------------------------------------------------
//void dittmann::mouseEntered(int x, int y) {}
//
////--------------------------------------------------------------
//void dittmann::mouseExited(int x, int y) {}
//
////--------------------------------------------------------------
//void dittmann::windowResized(int w, int h) {}
//
////--------------------------------------------------------------
//void dittmann::gotMessage(ofMessage msg) {}
//
////--------------------------------------------------------------
//void dittmann::dragEvent(ofDragInfo dragInfo) {}
//
////--------------------------------------------------------------
//void dittmann::drawHelpText() {}
//
////--------------------------------------------------------------
//void dittmann::setupAudio()
//{
//
//    // start the sound stream with a sample rate of 44100 Hz, and a buffer
//    // size of 512 samples per audioOut() call
//    ofSoundStreamSettings settings;
//    settings.numOutputChannels = 2;
//    settings.sampleRate = synth.SAMPLE_RATE;
//    settings.bufferSize = 512;
//    settings.numBuffers = 4;
//
//    // Latest linux versions default to the HDMI output
//    // this usually fixes that. Also check the list of available
//    // devices if sound doesn't work
//    auto devices = soundStream.getMatchingDevices("default");
//    if (!devices.empty())
//    {
//        settings.setOutDevice(devices[0]);
//    }
//
//    settings.setOutListener(this);
//
//    // the following setup function initiates the whole audio connection
//    // it invokes the underlying RTAudioSoundStream to
//    // - create an RtAudio object
//    // - connect the object to the RtAudioCallback function
//    // - start the stream and hence have a continious connection to audio in & out
//    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
//
//    //synth.setSampleRate(settings.sampleRate);
//
//    // Setup all oscs
//    const std::array<float, 20> frequencies = {
//    130.81, 164.81, 185.00, 196.00, 246.94, 261.63, 329.63, 369.99, 392.00, 493.88, 523.25, 659.25, 739.99, 783.99, 987.77, 1046.50, 1318.51, 1479.98, 1567.98, 1975.53};
//
//    for (auto frequency : frequencies) {
//        synth.addOscillator(ofDCO::SINE, synth.SAMPLE_RATE, frequency, 1);
//    }
//}
//
////--------------------------------------------------------------
//void dittmann::setupGui()
//{
//
//    // TODO: Update or remove the Gui
//    gui.setup();
//    gui.setPosition(20, 250);
//    gui.add(frequencySlider.setup("fundamental frequency", 172.5, 0.0, 1000.0));
//    // gui.add(amplitudeSlider.setup("amplitude ratio", 1.0, 0.0, 1.0));
//
//    frequencySlider.addListener(this, &dittmann::updateGui);
//}
//
//void dittmann::updateGui(float& value)
//{
//
//    frequencySlider = value; // not the best design, how to improve?
//};
//
//void dittmann::setupMidi()
//{
//    // print input ports to console
//    midiIn.listInPorts();
//
//    // open port by number (you may need to change this)
//    midiIn.openPort(1);
//    // midiIn.openPort("IAC Pure Data In");	// by name
//    // midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
//
//    // don't ignore sysex, timing, & active sense messages,
//    // these are ignored by default
//    midiIn.ignoreTypes(false, false, false);
//
//    // add ofApp as a listener
//    midiIn.addListener(this);
//
//    // print received messages to the console
//    midiIn.setVerbose(true);
//}
//
//void dittmann::newMidiMessage(ofxMidiMessage& message)
//{
//
//    if (message.status == MIDI_NOTE_ON)
//    {
//        ofLog() << "MIDI "
//                << "note on  "
//                << "CH " << message.channel << " Note " << message.pitch << " Val " << message.velocity;
//    }
//
//    if (message.status == MIDI_NOTE_OFF)
//    {
//        ofLog() << "MIDI "
//                << "note off "
//                << "CH " << message.channel << " Note " << message.pitch << " Val " << message.velocity;
//    }
//
//    if (message.status == MIDI_CONTROL_CHANGE)
//    {
//        ofLog() << "MIDI "
//                << "control "
//                << "CH " << message.channel << " Ctl " << message.control << " Val " << message.value;
//    }
//}
//
//
////--------------------------------------------------------------
//void dittmann::shutdownApp(){
//    exit();
//    soundStream.close();
//    lastBuffer.clear();
//    grabber.close();
//    audioMutex.unlock();
//    midiIn.removeListener(this);
//    midiIn.closePort();
//    synth.reset();
//}

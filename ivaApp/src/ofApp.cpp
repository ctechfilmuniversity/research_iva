#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    //unique_ptr<pdsp::Engine> enginePtr (new pdsp::Engine);
    enginePtr = unique_ptr<pdsp::Engine> (new pdsp::Engine);
    apps = {
//        new brennecke(),
//        new clausen(),
//        new traber(),
//        new stimberg(),
//        new dittmann(),
//        new objectContours()

        //new traber(std::move(enginePtr))
        new traber(),
        new traber()
    };
    
    apps[appIndex]->setPDSPengine(std::move(enginePtr));
    apps[appIndex]->setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    apps[appIndex]->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    apps[appIndex]->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Works only with numbers 1 till 9 on keyboard
    if (key >= 49 && key <= 49 + apps.size() -1) {
        switchApp(key);
        return;
    }
    apps[appIndex]->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    apps[appIndex]->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    apps[appIndex]->mouseMoved(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    apps[appIndex]->mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    apps[appIndex]->mousePressed(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    apps[appIndex]->mouseReleased(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    apps[appIndex]->mouseEntered(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    apps[appIndex]->mouseExited(x,y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    apps[appIndex]->windowResized(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    apps[appIndex]->gotMessage(msg);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    apps[appIndex]->dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::switchApp(int key){
    cout << "## switchApp called" << endl;
    cout << "## switchApp shutting down current app" << endl;
    //apps[appIndex]->shutdownApp();
    
    // reset pdsp
    enginePtr = std::move(apps[appIndex]->getPDSPengine());
    resetPDSPengine();
    
    // Works only with numbers 1 till 9 on keyboard
    appIndex = key - 49;

    // Reset Window Shape to original shape
    ofSetWindowShape(512, 384);

    // set window title after typeid name
    ofSetWindowTitle(typeid(*apps[appIndex]).name());

    cout << "## Calling setup function of new app" << endl;
    apps[appIndex]->setPDSPengine(std::move(enginePtr));
    apps[appIndex]->setup();
}

//--------------------------------------------------------------
void ofApp::resetPDSPengine(){
    enginePtr->audio_out(0).disconnectAll();
    enginePtr->audio_out(1).disconnectAll();
    enginePtr->setup( 44100, 512, 3);
}

#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    currentApp = &theFirstApp;
    //theFirstApp.setup();
    currentApp->setup();
}


//--------------------------------------------------------------
void ofApp::update(){
    currentApp->update();
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    currentApp->draw();
}

////--------------------------------------------------------------
//void ofApp::audioOut(ofSoundBuffer &outBuffer) {
//    currentApp->audioOut(outBuffer);
//}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1' || key == '2') {
        switchApp(key);
    }
    currentApp->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    currentApp->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    currentApp->mouseMoved(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    currentApp->mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    currentApp->mousePressed(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    currentApp->mouseReleased(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    currentApp->mouseEntered(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    currentApp->mouseExited(x,y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    currentApp->windowResized(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    currentApp->gotMessage(msg);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    currentApp->dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofApp::switchApp(int key){
    cout << "## switchApp" << key << endl;
    switch (key) {
        case '1':
        default:
            currentApp = &theFirstApp;
            break;
        case '2':
            currentApp = &clausenApp;
    }
    currentApp->setup();
}

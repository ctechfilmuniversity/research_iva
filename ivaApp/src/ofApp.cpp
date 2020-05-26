#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    currentApp = &brenneckeApp;
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
    switch (key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            switchApp(key);
            return;
        default:
            break;
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
    cout << "## switchApp called" << endl;
    cout << "## switchApp shutting down current app" << endl;
    currentApp->shutdownApp();
    switch (key) {
        case '1':
        default:
            currentApp = &brenneckeApp;
            break;
        case '2':
            currentApp = &clausenApp;
            break;
        case '3':
            currentApp = &traberApp;
            break;
        case '4':
            currentApp = &stimbergApp;
            break;
        case '5':
            currentApp = &dittmannApp;
            break;
            
    }
    
    // Reset Window Shape to original shape
    ofSetWindowShape(512, 384);
    
    cout << "## Calling setup function of new app" << endl;
    currentApp->setup();
}

    // Source:
    // https://github.com/wearenocomputer/workshop



#include "ofMain.h"
#include "ofApp.h"

//--------------------------------------------------------------
int main(){
    
    // Setup the GL context
    
    ofSetupOpenGL(512, 512, OF_WINDOW);
    
    // This kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    
    ofRunApp(new ofApp());
    
}

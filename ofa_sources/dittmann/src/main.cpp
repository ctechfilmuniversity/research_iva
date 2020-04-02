#include "ofMain.h"
#include "ofApp.h"

//========================================================================

// THREAD INFO
// The OS (via XCode) loads and executes the binary file and starts the "main thread"

int main( ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
    ofSetupOpenGL(512, 384,OF_WINDOW);            // <-------- setup the GL context
    
    // THREAD INFO
    // During the execution of ofSetupOpenGL, the external library "GLFW" is initialized
    // see openFrameworks/app/ofAppGLFWWindow.cpp, lines 225 and 231 "glfwCreateWindow(...)"
    // This external library starts an "event thread" responsible for exchanging events

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

    // THREAD INFO
    // During the execution of ofRunApp, the "audio thread" is started.    
}

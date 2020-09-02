//
//  ivaApp.h
//  ivaApp
//
//  Created by Markus Traber on 27.04.20.
//

#ifndef ivaApp_h
#define ivaApp_h

#include "ofMain.h"
#include "ofxPDSP.h"

//
class ivaApp {
    
public:
    virtual void shutdownApp() = 0;
    
    virtual void setup(){};
    virtual void update(){};
    virtual void draw(){};

    virtual void keyPressed(int key){};
    virtual void keyReleased(int key){};
    virtual void mouseMoved(int x, int y ){};
    virtual void mouseDragged(int x, int y, int button){};
    virtual void mousePressed(int x, int y, int button){};
    virtual void mouseReleased(int x, int y, int button){};
    virtual void mouseEntered(int x, int y){};
    virtual void mouseExited(int x, int y){};
    virtual void windowResized(int w, int h){};
    virtual void dragEvent(ofDragInfo dragInfo){};
    virtual void gotMessage(ofMessage msg){};
    
    unique_ptr<pdsp::Engine> getPDSPengine() {
        return std::move(enginePtr);
    }
    
    void setPDSPengine(unique_ptr<pdsp::Engine> engine) {
        enginePtr = std::move(engine);
    }

protected:
//    ivaApp(pdsp::Engine & engine) : engine(engine){}
//    pdsp::Engine & engine;
    
    ivaApp(unique_ptr<pdsp::Engine> engine) : enginePtr(std::move(engine)){}
    unique_ptr<pdsp::Engine> enginePtr;
    
private:
//    ivaApp();
    
};

#endif /* ivaApp_h */

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
class ivaApp : public ofBaseApp {
    
public:
    virtual void shutdownApp() = 0;

protected:
    ivaApp(pdsp::Engine & engine) : engine(engine){}
    pdsp::Engine & engine;
//    static pdsp::Engine engineStatic;
    
private:
//    ivaApp();
    
};

#endif /* ivaApp_h */

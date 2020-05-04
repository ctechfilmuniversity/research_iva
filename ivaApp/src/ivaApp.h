//
//  ivaApp.h
//  ivaApp
//
//  Created by Markus Traber on 27.04.20.
//

#ifndef ivaApp_h
#define ivaApp_h

#include "ofMain.h"
#include "ofxOpenCv.h"


class ivaApp : public ofBaseApp {
    
public:
    virtual void shutdownApp() = 0;

private:
};

#endif /* ivaApp_h */

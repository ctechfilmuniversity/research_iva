//
//  ivaApp.h
//  ivaApp
//
//  Created by Markus Traber on 27.04.20.
//

#ifndef ivaApp_h
#define ivaApp_h

#include "ofMain.h"

// TODO: The current implementation is a great step into the direction
//I would like to go. Nonetheless, I do not want to continue using several
//apps but rather one app with several visualizations. This will be the next
//big task for IVAapp from my point of view. Let's put it on the agenda of our next meeting.

class ivaApp : public ofBaseApp {
    
public:
    virtual void shutdownApp() = 0;

    
protected:
    void setupAudio();
    
    ofSoundStream soundStream{};
};

#endif /* ivaApp_h */

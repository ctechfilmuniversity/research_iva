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
    // TODO: Hier habe ich den zu implementierenden Konstruktor definiert. Hier wäre noch die Frage ob ich richtig mit der Referenz zur engine umgehe, da ich noch etwas Probleme mit diese ganzen C++ Thematik habe.
    ivaApp(pdsp::Engine & engine) : engine(engine){}
    pdsp::Engine & engine;
    
    // TODO: Ich hatte auch mal angedacht die Engine als static object zu implementieren. Allerdings habe ich es nicht zum laufen bekommen.
//    static pdsp::Engine engineStatic;
    
private:
//    ivaApp();
    
};

#endif /* ivaApp_h */

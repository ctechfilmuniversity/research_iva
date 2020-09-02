//
//  ivaApp.cpp
//  ivaApp
//
//  Created by Markus Traber on 02.09.20.
//

#include "ivaApp.h"

unique_ptr<pdsp::Engine> ivaApp::getPDSPengine() {
    return std::move(enginePtr);
}

void ivaApp::setPDSPengine(unique_ptr<pdsp::Engine> engine) {
    enginePtr = std::move(engine);
}

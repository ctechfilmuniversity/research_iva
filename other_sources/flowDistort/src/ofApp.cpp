// Based on: ofxCV addon example

#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	cam.setup(640, 480);
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	stepSize = 15;
	ySteps = cam.getHeight() / stepSize;
	xSteps = cam.getWidth() / stepSize;
    
	for (int y = 0; y < ySteps; y++)
    {
		for (int x = 0; x < xSteps; x++)
        {
            mesh.addVertex(glm::vec3(x * stepSize, y * stepSize, 0));
			mesh.addTexCoord(glm::vec2(x * stepSize, y * stepSize));
		}
	}
    
	for(int y = 0; y + 1 < ySteps; y++)
    {
		for(int x = 0; x + 1 < xSteps; x++)
        {
			int nw = y * xSteps + x;
			int ne = nw + 1;
			int sw = nw + xSteps;
			int se = sw + 1;
			mesh.addIndex(nw);
			mesh.addIndex(ne);
			mesh.addIndex(se);
			mesh.addIndex(nw);
			mesh.addIndex(se);
			mesh.addIndex(sw);
		}
	}
    
    ofHideCursor();
}

void ofApp::update()
{
	cam.update();

    if(cam.isFrameNew())
    {
		flow.setWindowSize(15);
		flow.calcOpticalFlow(cam);
        
		int i = 0;
		float distortionStrength = 6;
        
		for(int y = 1; y + 1 < ySteps; y++)
        {
			for(int x = 1; x + 1 < xSteps; x++)
            {
				int i = y * xSteps + x;
                glm::vec2 position(x * stepSize, y * stepSize);
                ofRectangle area(position - glm::vec2(stepSize, stepSize) / 2, stepSize, stepSize);
                glm::vec2 offset = flow.getAverageFlowInRegion(area);
                mesh.setVertex(i, glm::vec3(position + distortionStrength * offset, 0));
				i++;
			}
		}
	}
}

void ofApp::draw()
{
    
	ofBackground(0);
	ofScale(2.0, 2.0);
	cam.getTexture().bind();
	mesh.draw();
	cam.getTexture().unbind();
    
	//if(ofGetMousePressed())
    {
		//mesh.drawFaces();
        mesh.drawWireframe();
	}
}

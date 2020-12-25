#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
    ofMesh mesh;
    ofImage image;
    ofEasyCam easyCam;
    vector<ofVec3f> offsets;
    bool mouseDisplacement;
    ofMesh meshCopy;
    
    bool orbiting;
    float startOrbitTime;

    vector<float> distances;
    vector<float> angles;
    ofVec3f meshCentroid;
    
    ofxPanel gui;
    ofxButton btn1;
    ofxButton btn2;
    ofxButton btn3;
    ofxButton btn4;
    ofxButton btn5;
    
    void btn1Pressed();
    void btn2Pressed();
    void btn3Pressed();
    void btn4Pressed();
    void btn5Pressed();

    void imagetoMesh(string imageStr);
    
    ofVideoPlayer video;
};

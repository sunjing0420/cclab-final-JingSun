#pragma once

#include "ofMain.h"
#include "ofMath.h"

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
    
    
    ofImage bg;
    
    ofEasyCam cam;
    
    vector<ofPoint> position;
    vector<ofVec3f> vel1;
    vector<ofVec3f> vel2;
    vector<int> lifeSpan;
    vector<float> alpha;
    vector<float> size;
    vector<bool> edge;
    int count;
    vector<float> theta;
    float friction;
    float angle;
    
    int timer;
    
    bool fw1;
    
    ofSoundPlayer soundPlayer;
   
    //arduino led
    int sensorValue;
    bool noLight=false;
    float led1Val;
    float led2Val;
    
    float amplitude=127;
    float frequency1=0.1;
    float frequency2=0.15;
    //float t=millis();
    float baseLine=128;
    
    ofArduino	ard;
    bool		bSetupArduino;			// flag variable for setting up arduino once
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();

		};

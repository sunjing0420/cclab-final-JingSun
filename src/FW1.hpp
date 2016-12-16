//
//  FW1.hpp
//  12_1_Firefly
//
//  Created by Lucien on 2016/12/1.
//
//
#include "ofMain.h"
#include "ofMath.h"

class FW1{
public:
    FW1(ofVec3f pos);
    
    void update();
    void draw();
    
    ofPoint a[50];
    ofPoint b[50];
    
    float alpha;
    int lifeSpan;
    float radius;
    ofVec3f mPos;
};

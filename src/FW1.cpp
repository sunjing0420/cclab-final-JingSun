//
//  FW1.cpp
//  12_1_Firefly
//
//  Created by Lucien on 2016/12/1.
//
//

#include "FW1.hpp"

FW1::FW1(ofVec3f pos){
    lifeSpan = 200;
    mPos = pos;
}

void FW1::update(){
    radius = ofMap(lifeSpan,200,0,1,40);
    for(int i = 0; i<50; i++){
        a[i] = ofPoint(mPos.x+ofRandom(-radius,radius)*sin(ofRandom(TWO_PI)),mPos.y+ofRandom(-radius,radius)*cos(ofRandom(TWO_PI)),mPos.z+ofRandom(-radius,radius)*sin(ofRandom(TWO_PI)));
        b[i] = ofPoint(mPos.x+ofRandom(-radius,radius)*sin(ofRandom(TWO_PI)),mPos.y+ofRandom(-radius,radius)*cos(ofRandom(TWO_PI)),mPos.z+ofRandom(-radius,radius)*sin(ofRandom(TWO_PI)));
    }
    alpha = ofMap(lifeSpan,200,0,255,0);
    lifeSpan--;
}

void FW1::draw(){
    for(int i = 0; i<50; i++){
        ofSetColor(255, alpha+ofRandom(-30,0));
        ofDrawLine(a[i], b[i]);
    }
}

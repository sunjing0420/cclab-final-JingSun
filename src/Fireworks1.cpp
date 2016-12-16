//
//  fireworks1.cpp
//  12_1_Firefly
//
//  Created by Lucien on 2016/12/1.
//
//

#include "Fireworks1.hpp"

Fireworks1::Fireworks1(ofVec3f pos, ofVec3f vel1){
    position = pos;
    velocity = vel1;
    acc = ofVec3f(0,-0.0018,0);
    friction = 0.99;
    lifeSpan = 800;
}

void Fireworks1::update(){
//    velocity*=friction;
    velocity+=acc;
    position+=velocity;
    if(lifeSpan>0){
        FW1 temp(position);
        if(lifeSpan%3==0){
            fws.push_back(temp);
        }
        lifeSpan-=1;
    }
    
    for(int i = 0; i<fws.size(); i++){
        fws[i].update();
        if(fws[i].lifeSpan<0){
            fws.erase(fws.begin()+i);
        }
    }
}

void Fireworks1::draw(){
    
    for(int i = 0; i<fws.size(); i++){
        fws[i].draw();
    }
}

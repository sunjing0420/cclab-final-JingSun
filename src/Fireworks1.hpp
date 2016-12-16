//
//  fireworks1.hpp
//  12_1_Firefly
//
//  Created by Lucien on 2016/12/1.
//
//

#include "ofMain.h"
#include "FW1.hpp"

class Fireworks1{
    public:
    Fireworks1(ofVec3f pos, ofVec3f vel1);
    
    void update();
    void draw();
    
    vector<FW1> fws;
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acc;
    float friction;
    int lifeSpan;
};

#include "ofApp.h"

//ofSerial	serial;
//int			incoming[4];
//int			nRead = 0;


//--------------------------------------------------------------
void ofApp::setup(){
    //ofBackground(0);
    
    
    bg.load("images/fireflies.jpg");
    
    soundPlayer.loadSound("merrygoround.mp3");
    soundPlayer.play();
    
    cam.setOrientation(ofPoint(0,0,0));
    count = 0;
    friction = 0.99;
    angle = 0;
    fw1 = false;
    
    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
    ard.connect("/dev/tty.usbmodem1421", 57600);
    
    // listen for EInitialized notification. this indicates that
    // the arduino is ready to receive commands and it is safe to
    // call setupArduino()
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)

        
}

//--------------------------------------------------------------
void ofApp::update(){
    
        updateArduino();
  
    if(noLight){
        
        angle+=ofMap(ofGetMouseX(),0,ofGetWidth(),-1,1);
        
        if(noLight&&count<600&&count%10<1){
            for(int i = 0; i<2; i++){
                position.push_back(ofPoint(0,-300,0));
                lifeSpan.push_back(2000);
                vel1.push_back(ofVec3f(ofRandom(-1,1),ofRandom(20,40),ofRandom(-1,1)));
                vel2.push_back(ofVec3f(ofRandom(-2,2),ofRandom(-1,6),ofRandom(-2,2)));
                alpha.push_back(255);
                theta.push_back(ofRandom(TWO_PI));
                size.push_back(0);
                edge.push_back(false);
            }
        }
        
        count++;
        
        for(int i = 0; i<position.size(); i++){
            if(lifeSpan[i]>=2000){
                position[i]+=vel1[i];
                vel1[i]*=friction;
                size[i]+=(10.f/1000.f);
            }else if(lifeSpan[i]<2000&&lifeSpan[i]>800){
                theta[i]+=0.1;
                position[i]+=vel2[i];
                vel2[i]*=friction;
                alpha[i] = ofMap((position[i]-ofPoint(0,0,0)).length(),0,400,200,0)+sin(theta[i])*20;
                
                size[i] = 10+sin(theta[i])*5;
                if((position[i]-ofPoint(0,0,0)).length()>350){
                    vel2[i] *= -1;
                }
                vel2[i]+=ofVec3f(ofRandom(-0.1,0.1),ofRandom(-0.1,0.1),ofRandom(-0.1,0.1));
            }else{
                theta[i]+=0.1;
                vel1[i]+=ofVec3f(ofRandom(-0.1,0.1),0,ofRandom(-0.1,0.1));
                position[i]-=vel1[i]*0.05;
                alpha[i] = ofMap((position[i]-ofPoint(0,0,0)).length(),0,400,200,0)+sin(theta[i])*20;
                
                size[i] = 10+sin(theta[i])*5;
                alpha[i]-=0.3;
            }
            lifeSpan[i]--;
            if(lifeSpan[i]<0){
                position.erase(position.begin()+i);
            }
        }
        
        if(position.size()<1){
            noLight = false;
            count = 0;
        }
        
        
    }
}


void ofApp::setupArduino(const int & version) {
    
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
//    // set pins D2 and A5 to digital input
//    ard.sendDigitalPinMode(2, ARD_INPUT);
//    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older
//    
//    // set pin A0 to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
//    
//    // set pin D13 as digital output
//    ard.sendDigitalPinMode(9, ARD_OUTPUT);
//    ard.sendDigitalPinMode(10, ARD_OUTPUT);


//    // set pin A4 as digital output
//    ard.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older
//    
//    // set pin D11 as PWM (analog output)
      ard.sendDigitalPinMode(9, ARD_PWM);
      ard.sendDigitalPinMode(10, ARD_PWM);
//    
//    // attach a servo to pin D9
//    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
//    ard.sendServoAttach(9);
//    
//    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

//--------------------------------------------------------------
void ofApp::updateArduino(){
    
    // update the arduino, get any data or messages.
    // the call to ard.update() is required
    ard.update();
    
    sensorValue=ard.getAnalog(0);
    
    if(sensorValue>150){
       
        for(int i = 0; i<position.size();i++){
            vel2[i]+=1.5*(position[i]-ofVec3f(0,0,0)).getNormalized();
        }
    }
    
    if(noLight){
        ard.sendPwm(9, 0);
        ard.sendPwm(10, 0);
    }
    
    // do not send anything until the arduino has been set up
    if (bSetupArduino) {
        // fade the led connected to pin D11
        if(noLight==false&&sensorValue<150){
            ard.sendPwm(9, (int)(128 + 128 * sin(ofGetElapsedTimef())));
            ard.sendPwm(10, (int)(64 + 128 * sin(ofGetElapsedTimef())));

            
        }else{
            noLight=true;
        }
    }
//    cout<<ofToString(ard.getAnalog(0))<<endl;
    
   //sensorValue=ofToString(ard.getAnalog(0));
    
    
}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum) {
    

    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    //buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    
    
    if(noLight==false&&sensorValue<150){
        led1Val=255;
        led2Val=255;
//        led1Val= amplitude*sin(frequency1*t)+baseLine;
//        led2Val= amplitude*sin(frequency2*t)+baseLine;
    }else{
        noLight=true;
    
       //potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
}
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255,255);
    bg.draw(0, 0,ofGetWidth(),ofGetHeight());
    
    cam.begin();
    ofRotateY(angle);
    for(int i = 0; i<position.size(); i++){
        ofSetColor(255,255,0,alpha[i]);
        ofDrawSphere(position[i], size[i]);
        ofSetColor(255,255,0,alpha[i]-30);
        ofDrawSphere(position[i], size[i]+2);
        ofSetColor(255,255,0,alpha[i]-60);
        ofDrawSphere(position[i], size[i]+4);
        ofSetColor(255,255,0,alpha[i]-90);
        ofDrawSphere(position[i], size[i]+6);
        ofSetColor(255,255,0,alpha[i]-120);
        ofDrawSphere(position[i], size[i]+9);
        ofSetColor(255,255,0,alpha[i]-150);
        ofDrawSphere(position[i], size[i]+13);
        ofSetColor(255,255,0,alpha[i]-180);
        ofDrawSphere(position[i], size[i]+18);
    }
    
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

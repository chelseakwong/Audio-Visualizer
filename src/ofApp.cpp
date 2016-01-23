#include "ofApp.h"
#include <math.h>

//-------------- ofBall = cyan ones, layer 2 -------------
void ofBall::setup(float angle){
//    angle = ang;
    float radianAng = M_PI/2 - 2*M_PI*(angle/360);
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    radius = 150;
    x =  cos(radianAng)*radius + centerX;
    y =  centerY - sin(radianAng)*radius;
    
//    speedX = ofRandom(-1, 1) * 1;           // and random speed and direction
//    speedY = ofRandom(-1, 1) * 1;
    speedX = 1;
    speedY = 1;
    
    dim = 10;
    angle = radianAng;
}

void ofBall::update(float rms){
    //if loud enough then move faster
    float toAdd = 0.01; //default movement
    if (rms >= 0.01){
        if (rms> 0.02) toAdd = 0.015;
        else if (rms > 0.05) toAdd = 0.02;
        else if (rms> 0.1) toAdd = 0.03;
    }
    
    angle += toAdd;
    //reset if necessary
//    if (angle < 0){
//        angle = 2*M_PI;
//    }
    
    if (angle > (2*M_PI)){
        angle = 0;
    }
    
    int factor = (int(rms*50) % 10);
    
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    x =  cos(angle)*radius + centerX;
    y =  centerY - sin(angle)*radius;
    
//    if(y < 0 ){
//        y = 0;
//        speedY *= -1;
//    } else if(y > ofGetHeight()){
//        y = ofGetHeight();
//        speedY *= -1;
//    }
//    
//    if (factor == 0) factor = 0.1;
    
}

void ofBall::draw(){
    // values for R, G, B
    ofSetColor(0,255,255);
    ofDrawCircle(x, y, dim);
}

void ofBall::drawInfo(){
    string info = "ofball\n angle: " +ofToString(angle)+ "\nx: "+ofToString(x)+"\ny: "+ofToString(y);
    ofDrawBitmapStringHighlight(info, x,y);
}

//-------------- layer 1 ball = red ones -------------
//class layer1Ball : public ofBall{
//public:

void layer1Ball::setup(float angle){
    float radianAng = M_PI/2 - 2*M_PI*(angle/360);
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    radius = 80;
    x =  cos(radianAng)*radius + centerX;
    y =  centerY - sin(radianAng)*radius;
    
    //    speedX = ofRandom(-1, 1) * 1;           // and random speed and direction
    //    speedY = ofRandom(-1, 1) * 1;
    speedX = 1;
    speedY = 1;
    
    dim = 5;
    angle = radianAng;
}

void layer1Ball::update(float rms){
    //if loud enough then move faster
    float toAdd = 0.01; //default movement
    if (rms >= 0.01){
        if (rms> 0.02) toAdd = 0.015;
        else if (rms > 0.05) toAdd = 0.02;
        else if (rms> 0.1) toAdd = 0.03;
    }
    
    angle -= toAdd;
    //reset if necessary
    if (angle < 0){
        angle = 2*M_PI;
    }
    //    if (angle > (2*M_PI)){
    //        angle = 0;
    //    }
    int factor = (int(rms*50) % 10);
    
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    x =  cos(angle)*radius + centerX;
    y =  centerY - sin(angle)*radius;
}

void layer1Ball::draw(){
        ofSetColor(255,0,255);
        ofDrawCircle(x, y, dim);
}
    
void layer1Ball::drawInfo(){
        string info = "layer 1 ball\n angle: " +ofToString(angle)+ "\nx: "+ofToString(x)+"\ny: "+ofToString(y);
        ofDrawBitmapStringHighlight(info, x,y);
}


//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(120);
    
    ofBackground(22);
    ofBackground(206,206,206);
    ofFbo::Settings s;
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    
    gpuBlur.setup(s, false);
//    ofLoadImage(tex, "zoidberg.png");
    
//    Audio analyzer setup
    int sampleRate = 44100;
    int bufferSize = 256;
    int outChannels = 0;
    int inChannels = 2;
    int ticksPerBuffer = bufferSize/64;
    
    buffer_1 =  new float[bufferSize];
    buffer_2 =  new float[bufferSize];
    for(int i = 0; i < bufferSize; i++) { buffer_1[i]=buffer_2[i]= 0;}
    audioAnalyzer1.setup(512, 44100);
    audioAnalyzer2.setup(512, 44100);
    soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, ticksPerBuffer);
 
    ofEnableAntiAliasing();
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
//    ofEnableSmoothing();
    
    for (int k = 0; k<NBALLS; k++){
        float angle = float((360.0)/(NBALLS)) * k;
        ofBall newBall;
        newBall.setup(angle);
        myBall[k] = newBall;
    }

    for (int m = 0; m<NLayer1; m++){
        float ang = float((360.0)/NLayer1) * m;
        layer1Ball lay1Ball;
        lay1Ball.setup(ang);
        layer1[m] = lay1Ball;
    }
}

//--------------------------------------------------------------
void testApp::update(){
    float hiRms = audioAnalyzer1.getRms();
    for (int i = 0; i<NBALLS; i++){
        myBall[i].update(hiRms);
    }
    
    for (int j = 0; j<NLayer1; j++){
        layer1[j].update(hiRms);
    }
    
    if (hiRms > 0.01){
        gpuBlur.blurOffset = 1000*hiRms;
    }
    else{
        gpuBlur.blurOffset = 5;
    }
//    gpuBlur.blurOffset = 100 * ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
    //gpuBlur.blurOffset = 15;

    float rms2 = audioAnalyzer2.getRms();
    if (rms2 > 0.01){
        gpuBlur.blurPasses = 1000*audioAnalyzer2.getRms();
    }
    else{
        gpuBlur.blurPasses = 5;
    }
//    gpuBlur.blurPasses = 50 * ofMap(mouseX, 0, ofGetWidth(), 0, 1, true);
    //gpuBlur.blurPasses = 1;

    gpuBlur.numBlurOverlays = 7;
    gpuBlur.blurOverlayGain = 255;
}

//--------------------------------------------------------------


void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        buffer_1[i]	= input[i*nChannels];
        buffer_2[i]	= input[i*nChannels+1];
    }
    audioAnalyzer1.analyze(buffer_1, bufferSize);
    audioAnalyzer2.analyze(buffer_2, bufferSize);
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
//    ofBackgroundGradient(ofColor::grey, ofColor::black);
//    ofBackground(0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur.beginDrawScene();
    ofClear(0, 0, 0, 0);
    
    //layer 2, cyan ones
    for (int j = 0; j<NBALLS; j++){
        myBall[j].draw();
    }
    
    for (int k = 0; k<NLayer1; k++){
        layer1[k].draw();
    }
    
    tex.draw(0,0);
    
    ofSetColor(0,0,0);
    
    gpuBlur.endDrawScene();
    
    //blur the fbo
    //blending will be disabled at this stage
    gpuBlur.performBlur();
    
    //draw the "clean" scene
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur.drawSceneFBO();
    
    //overlay the blur on top
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
    gpuBlur.drawBlurFbo();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
//    
//    for (int a = 0; a<NBALLS; a++){
//        myBall[a].drawInfo();
//    }
//    
//    for (int b=0; b<NLayer1; b++){
//        layer1[b].drawInfo();
//    }
//    
    
    
    //draw info
    string info =	"blurOffset: " + ofToString(gpuBlur.blurOffset) + "\n" +
    "blurPasses: " + ofToString(gpuBlur.blurPasses) + "\n" +
    "numBlurOverlays: " + ofToString(gpuBlur.numBlurOverlays) + "\n" +
    "blurOverlayGain: " + ofToString(gpuBlur.blurOverlayGain) + "\n";
//    ofDrawBitmapStringHighlight(info, 20,20);
    
    string audioInfo = "rms: " + ofToString(audioAnalyzer1.getRms()) + "\n" +
    "power: " + ofToString(audioAnalyzer1.getPower()) + "\n" +
    "energy: " + ofToString(audioAnalyzer1.getEnergy()) + "\n" +
    "pitch frequency: " + ofToString(audioAnalyzer1.getPitchFreq()) + "\n";
    
//    ofDrawBitmapStringHighlight(audioInfo, 20, 360);
    
//    ofDrawBitmapStringHighlight("MouseX to control Blur Passes\nMouseY to control blur Offset", 400,20);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    ofTexture shadowTexture = gpuBlur.getBlurredSceneFbo().getTexture();
    ofPixels pixels;
    shadowTexture.readToPixels(pixels);
//    ofSaveImage(pixels, "shadow.tga");
    
    
}

//--------------------------------------------------------------
void testApp::exit(){
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();
    
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
#include "ofApp.h"
#include <math.h>
#include <stdio.h>

//-------------- ofBall = cyan ones, layer 2 -------------
void ofBall::setup(float angle){
    float radianAng = M_PI/2 - 2*M_PI*float(angle/360.0);
    this->angle = radianAng;
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    radius = 150;
    x =  cos(radianAng)*radius + centerX;
    y =  centerY - sin(radianAng)*radius;
    speedX = 1;
    speedY = 1;
    
    dim = 10;
//    angle = radianAng;
}

void ofBall::update(float rms){
    //if loud enough then move faster
    float toAdd = 0.01; //default movement
    if (rms > 0.01){
        dim = rms * 400;
        if (rms> 0.03) toAdd = 0.02;
        else if (rms > 0.05) toAdd = 0.03;
        else if (rms> 0.1) toAdd = 0.04;
    }
    else dim = 4;
    
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
    
}

void ofBall::draw(){
    // values for R, G, B
    ofSetColor(0,255,255, 50);
    ofDrawCircle(x, y, dim);
}

void ofBall::drawInfo(){
    string info = "ofball\n angle: " +ofToString(angle)+ "\nx: "+ofToString(x)+"\ny: "+ofToString(y);
    ofDrawBitmapStringHighlight(info, x,y);
}

//-------------- layer 1 ball = magenta ones -------------
//class layer1Ball : public ofBall{
//public:

void layer1Ball::setup(float angle){
    float radianAng = M_PI/2 - 2*M_PI*float(angle/360.0);
    fprintf(stderr,"angle in = %F\n",angle);
    fprintf(stderr,"radian angle out = %F\n",radianAng);
    this->angle = radianAng;
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    radius = 150;
    x =  cos(radianAng)*radius + centerX;
    y =  centerY - sin(radianAng)*radius;
    
    //    speedX = ofRandom(-1, 1) * 1;           // and random speed and direction
    //    speedY = ofRandom(-1, 1) * 1;
    speedX = 1;
    speedY = 1;
    
    dim = 8;
    //    angle = radianAng;
}

void layer1Ball::update(float rms){
    //if loud enough then move faster
    float toAdd = 0.01; //default movement
    if (rms >= 0.01){
        dim = rms * 400;
        if (rms> 0.02) toAdd = 0.015;
        else if (rms > 0.05) toAdd = 0.02;
        else if (rms> 0.1) toAdd = 0.03;
    }
    else dim = 4;
    
    angle -= toAdd;
    //reset if necessary
        if (angle < -2*M_PI){
            angle = 2*M_PI;
        }
    
    int factor = (int(rms*50) % 10);
    
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    x =  cos(angle)*radius + centerX;
    y =  centerY - sin(angle)*radius;
}

void layer1Ball::draw(){
        ofSetColor(255,0,255,50);
        ofDrawCircle(x, y, dim);
}
    
void layer1Ball::drawInfo(){
        string info = "layer 1 ball\n angle: " +ofToString(angle)+ "\nx: "+ofToString(x)+"\ny: "+ofToString(y);
        ofDrawBitmapStringHighlight(info, x,y);
}
//--------------------------------------------------------------
void yellowBall::draw(){
    ofSetColor(255,255,0,50);
    ofDrawCircle(x, y, dim);
}

void yellowBall::update(float rms){
    float toAdd = 0.018; //default movement
    if (rms >= 0.01){
        dim = rms * 400;

        if (rms> 0.02) toAdd = 0.02;
        else if (rms > 0.05) toAdd = 0.03;
        else if (rms> 0.1) toAdd = 0.04;
    }
    else {
        radius = 150;
        dim = 4;
    }

    angle += toAdd;
    //reset if necessary
    if (angle > (2*M_PI)){
        angle = 0;
    }
    
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    x =  cos(angle)*radius + centerX;
    y =  centerY - sin(angle)*radius;
}

//------ white outs -----------------------------


void whiteOut::update(float rms){
    float toAdd = 0.012; //default movement
//    if (rms >= 0.01){
//        dim = rms * 400;
//    }
//    else{
//        dim = 6;
//    }
//    
    angle += toAdd;
    //reset if necessary
    if (angle > (2*M_PI)){
        angle = 0;
    }
    
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    x =  cos(angle)*radius + centerX;
    y =  centerY - sin(angle)*radius;
}

void whiteOut::draw(){
    ofSetColor(255,255,255,50);
    ofDrawCircle(x, y, dim);
}

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(120);
    
    ofBackground(22);
    ofBackground(220,220,220);
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
    int bufferSize = 128;
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
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
//    ofEnableSmoothing();
    
    //cyan
    for (int k = 0; k< NBALLS; k++){
        float angle = float((180)/(NBALLS)) * k;
        myBall[k].setup(angle);
    }

    //magenta
    for (int m = 0; m<NLayer1; m++){
//        fprintf(stderr, "created %d th ball for nlayer1\n",m);
        float angle = float((120)/(NLayer1)) * m;
        layer1[m].setup(angle);
//        fprintf(stderr, "angle after setup=%F\n",layer1[m].angle);
    }
    
    //yellow
    for (int q = 0; q< yellowBalls; q++){
        float angle = float((270)/(yellowBalls)) * q;
        //        ofBall newBall;
        //        newBall.setup(angle);
        yellows[q].setup(angle);
    }
    
    for (int p = 0; p< whites; p++){
        float angle = float((360)/(whites)) * p;
        whiteOuts[p].setup(angle);
        whiteOuts[p].radius = 190;
    }
    
//    for (int a = 0; a<NLayer1; a++){
//        fprintf(stderr, "ball %d angle = %F",a, layer1[a].angle);
//    }
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
    
    for (int k = 0; k<yellowBalls; k++){
        yellows[k].update(hiRms);
    }
    
    for (int l = 0; l<whites; l++){
        whiteOuts[l].update(hiRms);
    }
    
    if (hiRms > 0.01){
        gpuBlur.blurOffset = 400*hiRms;
    }
    else{
        gpuBlur.blurOffset = 3;
    }
//    gpuBlur.blurOffset = 100 * ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
    //gpuBlur.blurOffset = 15;

//    float rms2 = audioAnalyzer2.getRms();
    if (hiRms > 0.01){
        gpuBlur.blurPasses = 400*hiRms;
    }
    else{
        gpuBlur.blurPasses = 3;
    }
//    gpuBlur.blurPasses = 50 * ofMap(mouseX, 0, ofGetWidth(), 0, 1, true);
    //gpuBlur.blurPasses = 1;

    gpuBlur.numBlurOverlays = 5;
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
    
//    ofBackgroundGradient(ofColor::white, ofColor::black);
    ofBackground(0,0,0);
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
    
    for (int o = 0; o<yellowBalls; o++){
        yellows[o].draw();
    }
    
    for (int l = 0; l<whites; l++){
        whiteOuts[l].draw();
    }
    
    tex.draw(0,0);
    
    ofSetColor(0,0,0);

    gpuBlur.endDrawScene();
    
    
    //draw core
    gpuBlur.beginDrawScene();
//    ofClear(0, 0, 0, 0);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, 10);
    tex.draw(0,0);
    ofSetColor(0,0,0);
    
    gpuBlur.endDrawScene();
    
    //blur the fbo
    //blending will be disabled at this stage
    gpuBlur.performBlur();
    
    //draw the "clean" scene
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    gpuBlur.drawSceneFBO();
    

    
    //overlay the blur on top
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
    gpuBlur.drawBlurFbo();
    
//    
//    for (int a = 0; a<NBALLS; a++){
//        myBall[a].drawInfo();
//    }
//    
//    for (int b=0; b<NLayer1; b++){
//        layer1[b].drawInfo();
//    }
    
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
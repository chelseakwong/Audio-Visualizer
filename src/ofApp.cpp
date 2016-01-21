#include "particle.h"
#include "ofApp.h"


ofBall::ofBall(int angle){
    int centerX = ofGetWidth()/2;
    int centerY = ofGetHeight()/2;
    int radius = 120;
    x = cos(angle)*radius + centerX;
    y =  centerY - sin(angle)*radius;
    string info = "angle: " +ofToString(angle)+ "x: "+ofToString(x)+"y: "+ofToString(y);
    ofDrawBitmapStringHighlight(info, x,y);
    
//    x = ofRandom(0,ofGetWidth());   //x origin
//    y = ofRandom(0,ofGetHeight());  //y origin
    
//    speedX = ofRandom(-1, 1) * 1;           // and random speed and direction
//    speedY = ofRandom(-1, 1) * 1;
    speedX = 1;
    speedY = 1;
    
    dim = 10;
}

void ofBall::update(float factor){
    factor = (int(factor) % 10);
    
    if(x < 0 ){
        x = 0;
        speedX *= -1;
    } else if(x > ofGetWidth()){
        x = ofGetWidth();
        speedX *= -1;
    }
    
    if(y < 0 ){
        y = 0;
        speedY *= -1;
    } else if(y > ofGetHeight()){
        y = ofGetHeight();
        speedY *= -1;
    }
    
    if (factor == 0) factor = 0.1;
    
    x+=(speedX * factor);
    y+=(speedY * factor);
}

void ofBall::draw(){
    // values for R, G, B
    ofSetColor(255,255,255);
    ofDrawCircle(x, y, dim);
}


//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    
    ofBackground(22);
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
    ofLoadImage(tex, "zoidberg.png");
    
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
    
//    particles = new Particle[300];
//    for(int j = 0; j < NPARTICLES; j++){
//        particles[j] = Particle(ofPoint(100, ofGetHeight()-100));
//    }
    
    for (int k = 0; k<NBALLS; k++){
        int angle = (360/NBALLS)*k;
        myBall[k] = new ofBall(angle);
    }
 
}

//--------------------------------------------------------------
void testApp::update(){
    float rms = audioAnalyzer1.getRms();
    if (rms >= 0.05){
        for (int i = 0; i<NBALLS; i++){
            myBall[i]->update(50*rms);
        }
    }
    
    if (rms > 0.01){
        gpuBlur.blurOffset = 1000*rms;
    }
    else{
        gpuBlur.blurOffset = 0;
    }
//    gpuBlur.blurOffset = 100 * ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
    //gpuBlur.blurOffset = 15;

    float rms2 = audioAnalyzer2.getRms();
    if (rms2 > 0.01){
        gpuBlur.blurPasses = 1000*audioAnalyzer2.getRms();
    }
    else{
        gpuBlur.blurPasses = 0;
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
    
    ofBackgroundGradient(ofColor::grey, ofColor::black);
//    ofBackground(0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur.beginDrawScene();
    ofClear(0, 0, 0, 0);
    for (int j = 0; j<NBALLS; j++){
        myBall[j]->draw();
    }
    tex.draw(0,0);
    
    ofSetColor(0,0,0);
   
//    PARTICLES
//    for (int k = 0; k<NPARTICLES; k++){
//        particles[k].update();
//        particles[k].draw();
//    }
    
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
    
    ofDrawBitmapStringHighlight(audioInfo, 20, 360);
    
//    ofDrawBitmapStringHighlight("MouseX to control Blur Passes\nMouseY to control blur Offset", 400,20);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    ofTexture shadowTexture = gpuBlur.getBlurredSceneFbo().getTexture();
    ofPixels pixels;
    shadowTexture.readToPixels(pixels);
    ofSaveImage(pixels, "shadow.tga");
    
    
}

//--------------------------------------------------------------
void testApp::exit(){
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();
    
}
//--------------------------------------------------------------

Particle::Particle(ofPoint l){
    counter = 0;
    float randmin = -HALF_PI;
    float randmax = 0;
    
    float r = ofRandom(0, TWO_PI);
    float x = cos(r);
    float y = sin(r);
    acc = ofPoint(x / 250, y / 250);
    
    float q = ofRandom(0, 1);
    r = ofRandom(randmin, randmax);
    x = cos(r) * q;
    y = sin(r) * q;
    vel = ofPoint(x, y);
    loc = l;
    hist = new ofPoint[1000];
}
//--------------------------------------------------------------

void Particle::update(){
    vel += acc;
    loc += vel;
    if(ofGetFrameNum() % 10 == 0 && counter < 1000){
        hist[counter].x = loc.x;
        hist[counter].y = loc.y;
        counter++;
    }
}
//--------------------------------------------------------------

void Particle::draw(){
    ofFill();
    ofSetColor(100, 100, 100, 100);
    drawArrowHead(vel, loc, 10);
    ofNoFill();
    ofSetColor(0, 0, 0, 100);
    ofBeginShape();
    for(int i = 0; i < counter; i++){
        ofVertex(hist[i].x, hist[i].y);
    }
    if(counter > 0) ofVertex(loc.x, loc.y);
    ofEndShape(false);
}
//--------------------------------------------------------------

void Particle::drawArrowHead(ofPoint v, ofPoint loc, float scale){
    ofPushMatrix();
    float arrowsize = 5.0f;
    ofTranslate(loc.x, loc.y, 0);
    float rotate = atan2(v.y, v.x);
    ofRotate(ofRadToDeg(rotate), 0, 0, 1);
    
    float inX = v.x;
    float inY = v.y;
    
    float len = (sqrt(inX * inX + inY * inY)) * scale;
    arrowsize = ofMap(len, 0.f, 10.f, 0.f, 1.f, false) * arrowsize;
    
    ofDrawLine(0, 0, len-arrowsize, 0);
    
    ofBeginShape();
    ofVertex(len, 0);
    ofVertex(len-arrowsize, arrowsize/2);
    ofVertex(len-arrowsize, -arrowsize/2);
    ofEndShape(true);
    
    ofPopMatrix();
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
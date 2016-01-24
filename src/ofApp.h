#pragma once

#include "ofMain.h"
#include "ofxFboBlur.h"
#include "ofxAudioAnalyzer.h"

#define NBALLS 12
#define NLayer1 4
#define yellowBalls 8
#define whites 18

class ofBall{
public:
    //methods
    virtual void setup(float angle);
    virtual void update(float rms);
    virtual void draw();
    virtual void drawInfo();
    
    //variables
    float angle;
    float x;
    float y;
    int dim;
    int radius;
    float speedX;
    float speedY;
private:
};

//magenta
class layer1Ball : public ofBall{
public:
    void setup(float angle);
    void update(float rms);
    void draw();
    void drawInfo();
};

//yellow
class yellowBall : public ofBall{
public:
    void draw();
    void update(float rms);
};

//white extra with bouncing off
class whiteOut : public ofBall{
public:
    void update(float rms);
    void draw();
};

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();

    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    

    //    visual
    ofxFboBlur gpuBlur;
    ofTexture tex;

    //    audio
    void audioIn(float * input, int bufferSize, int nChannels);

    ofSoundStream soundStream;
    ofxAudioAnalyzer audioAnalyzer1, audioAnalyzer2;
    
    float *buffer_1;
    float *buffer_2;
    
    //balls
    ofBall myBall[NBALLS];
    layer1Ball layer1[NLayer1];
    yellowBall yellows[yellowBalls];
    whiteOut whiteOuts[whites];
    
};


//
//  particle.h
//  myProductiveSketch
//
//  Created by Chelsea Kwong on 1/20/16.
//
//
#pragma once

#include "ofMain.h"

class Particle {
public:
    ofPoint loc, vel, acc;
    ofPoint *hist;
    int counter;
    Particle(){};
    Particle(ofPoint l);
    float mag(ofPoint in);
    void update();
    void draw();
    void drawArrowHead(ofPoint v, ofPoint loc, float scale);
};

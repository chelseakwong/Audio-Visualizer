//
//  particle.cpp
//  myProductiveSketch
//
//  Created by Chelsea Kwong on 1/20/16.
//
//

// ===========================================================
// - Simulate_Particles.cpp
// ===========================================================
#include "particle.h"

Particle *particles;
bool saving = false;

float mag(ofPoint in){
    float retf = sqrt(in.x * in.x + in.y * in.y);
    return retf;
}

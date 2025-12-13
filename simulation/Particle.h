#ifndef POSITION_BASED_DYNAMIC_PARTICLE_H
#define POSITION_BASED_DYNAMIC_PARTICLE_H


#include "../Vec2.h"

class Particle {

public:
    Vec2 position;
    Vec2 velocity;
    double radius;
    double mass;
    Particle(Vec2 position, Vec2 velocity, double radius, double mass);
};


#endif //POSITION_BASED_DYNAMIC_PARTICLE_H
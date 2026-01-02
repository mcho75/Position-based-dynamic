#ifndef POSITION_BASED_DYNAMIC_RIGIDBODY_H
#define POSITION_BASED_DYNAMIC_RIGIDBODY_H


#include "Attribute.h"
#include "Vec2.h"

class Rigidbody : public Attribute {

public:
    Rigidbody(Entity* parent, double mass, double elasticity)
        : Attribute(parent)
        , _mass(mass)
        , _elasticity(elasticity){}
    ~Rigidbody() override = default;
    double getMass() { return _mass; }
    double getElasticity() { return _elasticity; }
    void applyGravity(Vec2 g, double dt);
    void applyFriction(double lambda, double dt);

protected:
    double _mass;
    double _elasticity;
};


#endif //POSITION_BASED_DYNAMIC_RIGIDBODY_H
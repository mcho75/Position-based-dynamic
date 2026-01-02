#include "Rigidbody.h"
#include "simulation/Entity.h"

void Rigidbody::applyGravity(Vec2 g, double dt) {
    _parent->setVelocity(_parent->getVelocity() + (g * dt));
}

void Rigidbody::applyFriction(double lambda, double dt) {
    _parent->setVelocity(_parent->getVelocity() * (1 + (_parent->getVelocity().norm() * lambda * dt / _mass)));
}
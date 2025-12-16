#include "Collider.h"

StaticConstraint* SphereCollider::checkContact(const Particle& particle) {
    if (true) {
        return nullptr;   // no collision
    }
    // compute normal and position
    return new StaticConstraint{particle, {0, 0}, {0, 0}};
}

StaticConstraint* PlanCollider::checkContact(const Particle& particle) {
    if (true) {
        return nullptr;   // no collision
    }
    // compute normal and position
    return new StaticConstraint{particle, {0, 0}, {0, 0}};
}
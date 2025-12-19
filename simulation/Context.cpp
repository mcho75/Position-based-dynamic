#include "Context.h"

void Context::addParticle(const Particle& particle) {
    _particles.append(particle);
}

void Context::addCollider(const Collider& collider) {
    _colliders.append(collider);
}

QList<Particle> Context::getParticles() {
    return _particles;
}

QList<Collider> Context::getColliders() {
    return _colliders;
}

void Context::updatePhysicalSystem(const double dt) {
    applyExternalForce(dt);
    applyFriction(dt);
    addDynamicContactConstraints(dt);
    projectConstraints();
    deleteContactConstraints();
    applyPositions();
}

void Context::applyExternalForce(const double dt) {

    // sum of all external forces
    Vec2 Fext(0, -9.81);

    // compute velocities
    for (Particle& particle : _particles) {
        particle.velocity = particle.velocity + (Fext * dt / particle.mass);
    }
}

void Context::addDynamicContactConstraints(const double dt) {}

void Context::addStaticContactConstraints(const double dt) {
    for (Collider& collider : _colliders) {
        for (Particle& particle : _particles) {
            StaticConstraint* constraint = collider.checkContact(particle);
            if (constraint != nullptr) {
                _staticConstraints.append(constraint);
            }
        }
    }
}

void Context::projectConstraints() {
    for (StaticConstraint* constraint : _staticConstraints) {
        (constraint->particle).nextPosition = {0, 0}; // TODO: compute positions
    }
}

void Context::applyFriction(const double dt) {

    // friction coefficient
    double lambda = 0.0;

    // compute velocities
    for (Particle& particle : _particles) {
        particle.velocity = particle.velocity + particle.velocity * (-lambda * dt / particle.mass);
    }

    // compute positions
    for (Particle& particle : _particles) {
        particle.nextPosition = particle.nextPosition + (particle.velocity * dt);
    }
}

void Context::deleteContactConstraints() {}

void Context::applyPositions() {
    for (Particle& particle : _particles) {
        particle.position = particle.nextPosition;
    }
}
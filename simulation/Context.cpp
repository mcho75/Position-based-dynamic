#include "Context.h"

void Context::addParticle(const Particle& particle) {
    _particles.append(particle);
}

void Context::addCollider(Collider* collider) {
    _colliders.append(collider);
}

QList<Particle> Context::getParticles() {
    return _particles;
}

QList<Collider*> Context::getColliders() {
    return _colliders;
}

void Context::updatePhysicalSystem(const double dt) {
    applyExternalForce(dt);
    addStaticContactConstraints();
    addDynamicContactConstraints();
    projectConstraints();
    deleteContactConstraints();
    applyPositions(dt);
}

void Context::applyExternalForce(const double dt) {

    // sum of all external forces
    Vec2 Fext(0, -9.81);

    // compute velocities
    for (Particle& particle : _particles) {
        particle.velocity = particle.velocity + (Fext * dt / particle.mass);
        particle.nextPosition = particle.position + (particle.velocity * dt);
    }
}

void Context::addDynamicContactConstraints() {}

void Context::addStaticContactConstraints() {
    for (Collider* collider : _colliders) {
        for (Particle& particle : _particles) {
            StaticConstraint* constraint = collider->checkContact(particle);
            if (constraint != nullptr) {
                _staticConstraints.append(constraint);
            }
        }
    }
}

void Context::projectConstraints() {
    for (StaticConstraint* constraint : _staticConstraints) {
        constraint->particle.nextPosition = constraint->particle.nextPosition + constraint->position;
    }
}

void Context::deleteContactConstraints() {

    // delete static constraints
    for (StaticConstraint* constraint : _staticConstraints) {
        delete constraint;
    }
    _staticConstraints.clear();
}

void Context::applyPositions(const double dt) {
    for (Particle& particle : _particles) {
        particle.velocity = (particle.nextPosition - particle.position) / dt;
        particle.position = particle.nextPosition;
    }
}
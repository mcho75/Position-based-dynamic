#include "Context.h"

void Context::initialize(const Vec2& dim1, const Vec2& dim2) {
    _colliders.append(new PlanCollider(Vec2(dim1[0], dim1[1]), Vec2(dim1[0], dim2[1])));
    _colliders.append(new PlanCollider(Vec2(dim1[0], dim2[1]), Vec2(dim2[0], dim2[1])));
    _colliders.append(new PlanCollider(Vec2(dim2[0], dim2[1]), Vec2(dim2[0], dim1[1])));
    _colliders.append(new PlanCollider(Vec2(dim2[0], dim1[1]), Vec2(dim1[0], dim1[1])));
}

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
    addStaticContactConstraints(dt);
    addDynamicContactConstraints(dt);
    projectConstraints(dt);
    deleteContactConstraints();
    applyPositions(dt);
}

void Context::applyExternalForce(const double dt) {

    // constants
    Vec2 g(0.0, -9.81);
    double lambda = 2;

    // compute velocities
    for (Particle& particle : _particles) {
        particle.velocity = particle.velocity + (g * dt);
        particle.velocity = particle.velocity - particle.velocity * (particle.velocity.norm() * lambda * dt / particle.mass);
    }
}

void Context::addStaticContactConstraints(const double dt) {
    for (Collider* collider : _colliders) {
        for (Particle& particle : _particles) {
            StaticConstraint* constraint = collider->checkContact(particle, dt);
            if (constraint != nullptr) {
                _staticConstraints.append(constraint);
            }
        }
    }
}

void Context::addDynamicContactConstraints(const double dt) {
    for (int i = 0; i < _particles.size(); i++) {
        for (int j = 0; j < i; j++) {
            Particle& firstParticle = _particles[i];
            Particle& secondParticle = _particles[j];
            Vec2 firstNextPosition = firstParticle.position + (firstParticle.velocity * dt);
            Vec2 secondNextPosition = secondParticle.position + (secondParticle.velocity * dt);
            Vec2 x = firstNextPosition - secondNextPosition;
            double norm = x.norm();
            double C = norm - firstParticle.radius - secondParticle.radius;
            if (C < 0) {
                double sigmai = (1 / firstParticle.mass) / (1 / firstParticle.mass + 1 / secondParticle.mass) * C;
                double sigmaj = (1 / secondParticle.mass) / (1 / firstParticle.mass + 1 / secondParticle.mass) * C;
                _dynamicConstraints.append(new DynamicConstraint{firstParticle, secondParticle,
                    x * -sigmai / norm - firstParticle.position, x * sigmaj / norm - secondParticle.position});
            }
        }
    }
}

void Context::projectConstraints(const double dt) {

    // check static constraints
    for (StaticConstraint* constraint : _staticConstraints) {
        constraint->particle.velocity = constraint->di;
    }

    // check dynamic constraints
    for (DynamicConstraint* constraint : _dynamicConstraints) {
        constraint->firstParticle.velocity = constraint->di;
        constraint->secondParticle.velocity = constraint->dj;
    }
}

void Context::deleteContactConstraints() {

    // delete static constraints
    for (StaticConstraint* constraint : _staticConstraints) {
        delete constraint;
    }
    _staticConstraints.clear();

    // delete dynamic constraints
    for (DynamicConstraint* constraint : _dynamicConstraints) {
        delete constraint;
    }
    _dynamicConstraints.clear();
}

void Context::applyPositions(const double dt) {
    for (Particle& particle : _particles) {
        particle.position = particle.position + particle.velocity * dt;
    }
}
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
    addStaticContactConstraints();
    addDynamicContactConstraints();
    projectConstraints();
    deleteContactConstraints();
    applyPositions(dt);
}

void Context::applyExternalForce(const double dt) {

    // constants
    Vec2 g(0.0, -9.81);
    double lambda = -0.5;

    // compute velocities
    for (Particle& particle : _particles) {

        particle.velocity = particle.velocity + (g * dt);
        particle.velocity = particle.velocity - particle.velocity * (particle.velocity.norm() * lambda * dt / particle.mass);

        particle.nextPosition = particle.position + (particle.velocity * dt);
    }
}

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

void Context::addDynamicContactConstraints() {
    for (int i = 0; i < _particles.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            Particle& firstParticle = _particles[i];
            Particle& secondParticle = _particles[j];
            Vec2 x = firstParticle.nextPosition - secondParticle.nextPosition;
            double norm = sqrt(x[0] * x[0] + x[1] * x[1]);
            double C = norm - firstParticle.radius - secondParticle.radius;
            double sigmai = (1 / firstParticle.mass) / (1 / firstParticle.mass + 1 / secondParticle.mass) * C;
            double sigmaj = (1 / secondParticle.mass) / (1 / firstParticle.mass + 1 / secondParticle.mass) * C;
            if (C < 0) {
                _dynamicConstraints.append(new DynamicConstraint{firstParticle, secondParticle,
                                                      x * -sigmai / norm, x * sigmaj / norm});
            }
        }
    }
}

void Context::projectConstraints() {

    // check static constraints
    for (StaticConstraint* constraint : _staticConstraints) {
        constraint->particle.nextPosition = constraint->contact + constraint->di;
    }

    // check dynamic constraints
    for (DynamicConstraint* constraint : _dynamicConstraints) {
        constraint->firstParticle.nextPosition = constraint->firstParticle.nextPosition + constraint->di;
        constraint->secondParticle.nextPosition = constraint->secondParticle.nextPosition + constraint->dj;
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
        particle.velocity = (particle.nextPosition - particle.position) / dt;
        particle.position = particle.nextPosition;
    }
}
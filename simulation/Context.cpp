#include "Context.h"

void Context::initialize(const Vec2& dim1, const Vec2& dim2) {
    _colliders.append(new PlanCollider(Vec2(dim1[0], dim1[1]), Vec2(dim1[0], dim2[1]), 0.8));
    _colliders.append(new PlanCollider(Vec2(dim1[0], dim2[1]), Vec2(dim2[0], dim2[1]), 0.8));
    _colliders.append(new PlanCollider(Vec2(dim2[0], dim2[1]), Vec2(dim2[0], dim1[1]), 0.8));
    _colliders.append(new PlanCollider(Vec2(dim2[0], dim1[1]), Vec2(dim1[0], dim1[1]), 0.8));
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
    double lambda = -5;

    // compute velocities
    for (Particle& particle : _particles) {
        particle.velocity = particle.velocity + (g * dt);
        particle.velocity = particle.velocity + particle.velocity * (particle.velocity.norm() * lambda * dt / particle.mass);
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
            double norm = x.norm();
            if (norm - firstParticle.radius - secondParticle.radius < -1e-10) {
                Vec2 nc = x / norm;
                Vec2 tc(nc[1], -nc[0]);
                double sigmai = (1 / firstParticle.mass) / (1 / firstParticle.mass + 1 / secondParticle.mass);
                double sigmaj = (1 / secondParticle.mass) / (1 / firstParticle.mass + 1 / secondParticle.mass);
                Vec2 qc = (firstParticle.nextPosition + secondParticle.nextPosition) / 2;
                _dynamicConstraints.append(new DynamicConstraint{firstParticle, secondParticle,
                    qc + (firstParticle.radius * nc), qc - (secondParticle.radius * nc),
                    sigmai * ((firstParticle.velocity * tc) * tc - (firstParticle.velocity * nc) * nc),
                    sigmaj * ((secondParticle.velocity * tc) * tc - (secondParticle.velocity * nc) * nc)});
            }
        }
    }
}

void Context::projectConstraints() {

    // check static constraints
    for (StaticConstraint* constraint : _staticConstraints) {
        constraint->particle.nextPosition = constraint->di;
        constraint->particle.velocity = constraint->vi;
    }

    // check dynamic constraints
    for (DynamicConstraint* constraint : _dynamicConstraints) {
        constraint->firstParticle.nextPosition = constraint->di;
        constraint->secondParticle.nextPosition = constraint->dj;
        constraint->firstParticle.velocity = constraint->vi;
        constraint->secondParticle.velocity = constraint->vj;
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
        particle.position = particle.nextPosition;
    }
}
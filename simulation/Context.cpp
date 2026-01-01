#include "Context.h"

void Context::initialize(const Vec2& dim1, const Vec2& dim2) {
    _colliders.append(new PlanCollider(Vec2(dim1[0], dim1[1]), Vec2(dim1[0], dim2[1]), 0.05));
    _colliders.append(new PlanCollider(Vec2(dim1[0], dim2[1]), Vec2(dim2[0], dim2[1]), 0.05));
    _colliders.append(new PlanCollider(Vec2(dim2[0], dim2[1]), Vec2(dim2[0], dim1[1]), 0.05));
    _colliders.append(new PlanCollider(Vec2(dim2[0], dim1[1]), Vec2(dim1[0], dim1[1]), 0.05));
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
    addDynamicContactConstraints();
    projectDynamicConstraints(dt);
    addStaticContactConstraints();
    projectStaticConstraints(dt);
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

void Context::addDynamicContactConstraints() {
    for (int i = 0; i < _particles.size(); i++) {
        for (int j = 0; j < i; ++j) {
            Particle& firstParticle = _particles[i];
            Particle& secondParticle = _particles[j];
            Vec2 x = firstParticle.nextPosition - secondParticle.nextPosition;
            double norm = x.norm();
            if (norm - firstParticle.radius - secondParticle.radius < 0) {
                Vec2 nc = x / norm;
                Vec2 tc(nc[1], -nc[0]);
                Vec2 qc = (firstParticle.position + secondParticle.position) / 2;
                _dynamicConstraints.append(new DynamicConstraint{firstParticle, secondParticle, nc, tc, qc});
            }
        }
    }
}

void Context::projectDynamicConstraints(const double dt) {

    for (DynamicConstraint* constraint : _dynamicConstraints) {

        Particle& firstParticle = constraint->firstParticle;
        Particle& secondParticle = constraint->secondParticle;

        double sigmai = (1 / firstParticle.mass) / ((1 / firstParticle.mass) + (1 / secondParticle.mass));
        double sigmaj = (1 / secondParticle.mass) / ((1 / firstParticle.mass) + (1 / secondParticle.mass));

        // projection on tangent and normal
        Vec2 firstTangent = (firstParticle.velocity * constraint->tangent) * constraint->tangent;
        Vec2 secondTangent = (secondParticle.velocity * constraint->tangent) * constraint->tangent;
        Vec2 firstNormal = (firstParticle.velocity * constraint->normal) * constraint->normal;
        Vec2 secondNormal = (secondParticle.velocity * constraint->normal) * constraint->normal;

        // new position, nextPosition and velocity
        firstParticle.position = constraint->contact + (firstParticle.radius * constraint->normal);
        secondParticle.position = constraint->contact - (secondParticle.radius * constraint->normal);
        firstParticle.velocity -= (1 + firstParticle.elasticity - sigmai) * firstNormal;
        secondParticle.velocity -= (1 + secondParticle.elasticity + sigmaj) * secondNormal;
        firstParticle.nextPosition = firstParticle.position + (firstParticle.velocity * dt);
        secondParticle.nextPosition = secondParticle.position + (secondParticle.velocity * dt);
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

void Context::projectStaticConstraints(const double dt) {

    for (StaticConstraint* constraint : _staticConstraints) {

        Particle& particle = constraint->particle;
        Collider* collider = constraint->collider;

        // projection on tangent and normal
        Vec2 velocityTangent = (particle.velocity * constraint->tangent) * constraint->tangent;
        Vec2 velocityNormal = (particle.velocity * constraint->normal) * constraint->normal;

        // new position, nextPosition and velocity
        particle.position = constraint->contact + particle.radius * constraint->normal;
        particle.velocity -= (1 + particle.elasticity) * velocityNormal + collider->getDamping() * velocityTangent;
        particle.nextPosition = particle.position + (particle.velocity * dt);
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
#ifndef POSITION_BASED_DYNAMIC_CONTEXT_H
#define POSITION_BASED_DYNAMIC_CONTEXT_H


#include <qlist.h>
#include "Particle.h"
#include "Collider.h"

class Context {

public:
    Context()
        : _particles(QList<Particle>())
        , _colliders(QList<Collider>())
        , _staticConstraints(QList<StaticConstraint*>()) {}
    ~Context() = default;
    void addParticle(const Particle& particle);
    void addCollider(const Collider &collider);
    QList<Particle> getParticles();
    QList<Collider> getColliders();
    void updatePhysicalSystem(const double dt);
    void applyExternalForce(const double dt);
    void addDynamicContactConstraints(const double dt);
    void addStaticContactConstraints(const double dt);
    void projectConstraints();
    void applyFriction(const double dt);
    void deleteContactConstraints();
    void applyPositions();

private:
    QList<Particle> _particles;
    QList<Collider> _colliders;
    QList<StaticConstraint*> _staticConstraints;
};


#endif //POSITION_BASED_DYNAMIC_CONTEXT_H
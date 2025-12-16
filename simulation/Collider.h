#ifndef POSITION_BASED_DYNAMIC_COLLIDER_H
#define POSITION_BASED_DYNAMIC_COLLIDER_H


#include "Particle.h"

struct StaticConstraint {
    const Particle& particle;
    Vec2 position;
    Vec2 normal;
};

class Collider {

public:
    virtual ~Collider() = default;
    virtual StaticConstraint* checkContact(const Particle& particle) {return nullptr;}
};

class SphereCollider final : public Collider {

public:
    explicit SphereCollider(const double position, const double radius)
        : _position(position)
        , _radius(radius) {}
    ~SphereCollider() override = default;
    StaticConstraint* checkContact(const Particle& particle) override;

private:
    double _position;
    double _radius;
};

class PlanCollider final : public Collider {

public:
    explicit PlanCollider(const double start, const double end)
        : _start(start)
        , _end(end) {}
    ~PlanCollider() override = default;
    StaticConstraint* checkContact(const Particle& particle) override;

private:
    double _start;
    double _end;
};


#endif //POSITION_BASED_DYNAMIC_COLLIDER_H
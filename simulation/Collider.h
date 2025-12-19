#ifndef POSITION_BASED_DYNAMIC_COLLIDER_H
#define POSITION_BASED_DYNAMIC_COLLIDER_H


#include "Particle.h"

struct StaticConstraint {
    Particle& particle;
    Vec2 position;
    Vec2 normal;
};

class Collider {

public:
    virtual ~Collider() = default;
    virtual StaticConstraint* checkContact(Particle& particle) { return nullptr; }
};

class SphereCollider final : public Collider {

public:
    explicit SphereCollider(const Vec2 position, const double radius)
        : _position(position)
        , _radius(radius) {}
    ~SphereCollider() override = default;
    StaticConstraint* checkContact(Particle& particle) override;
    Vec2 getPosition() const { return _position; }
    double getRadius() const { return _radius; }

private:
    Vec2 _position;
    double _radius;
};

class PlanCollider final : public Collider {

public:
    explicit PlanCollider(const Vec2 start, const Vec2 end)
        : _start(start)
        , _end(end) {}
    ~PlanCollider() override = default;
    StaticConstraint* checkContact(Particle& particle) override;
    Vec2 getStart() { return _start; }
    Vec2 getEnd() { return _end; }

private:
    Vec2 _start;
    Vec2 _end;
};


#endif //POSITION_BASED_DYNAMIC_COLLIDER_H
#ifndef POSITION_BASED_DYNAMIC_COLLIDER_H
#define POSITION_BASED_DYNAMIC_COLLIDER_H


#include <QPainter>
#include "Particle.h"

class Collider;
class Viewport;

struct StaticConstraint {
    Particle& particle;
    Collider* collider;
    Vec2 normal;
    Vec2 tangent;
    Vec2 contact;
};

struct DynamicConstraint {
    Particle& firstParticle;
    Particle& secondParticle;
    Vec2 normal;
    Vec2 tangent;
    Vec2 contact;
};

class Collider {

public:
    explicit Collider(const double damping)
        : _damping(damping) {}
    virtual ~Collider() = default;
    virtual StaticConstraint* checkContact(Particle& particle) { return nullptr; }
    virtual void drawCollider(QPainter& painter, Viewport* viewport) {}
    double getDamping() const { return _damping; }

private:
    double _damping;
};

class SphereCollider final : public Collider {

public:
    explicit SphereCollider(const Vec2 position, const double radius, const double damping)
        : _position(position)
        , _radius(radius)
        , Collider(damping) {}
    ~SphereCollider() override = default;
    StaticConstraint* checkContact(Particle& particle) override;
    void drawCollider(QPainter& painter, Viewport* viewport) override;

private:
    Vec2 _position;
    double _radius;
};

class PlanCollider final : public Collider {

public:
    explicit PlanCollider(const Vec2 start, const Vec2 end, const double damping)
        : _start(start)
        , _end(end)
        , Collider(damping) {}
    ~PlanCollider() override = default;
    StaticConstraint* checkContact(Particle& particle) override;
    void drawCollider(QPainter& painter, Viewport* viewport) override;

private:
    Vec2 _start;
    Vec2 _end;
};


#endif //POSITION_BASED_DYNAMIC_COLLIDER_H
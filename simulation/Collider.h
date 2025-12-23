#ifndef POSITION_BASED_DYNAMIC_COLLIDER_H
#define POSITION_BASED_DYNAMIC_COLLIDER_H


#include <QPainter>
#include "Particle.h"

class Viewport;

struct StaticConstraint {
    Particle& particle;
    Vec2 di;
    Vec2 vi;
};

struct DynamicConstraint {
    Particle& firstParticle;
    Particle& secondParticle;
    Vec2 di;
    Vec2 dj;
    Vec2 vi;
    Vec2 vj;
};

class Collider {

public:
    virtual ~Collider() = default;
    virtual StaticConstraint* checkContact(Particle& particle) { return nullptr; }
    virtual void drawCollider(QPainter& painter, Viewport* viewport) {}
};

class SphereCollider final : public Collider {

public:
    explicit SphereCollider(const Vec2 position, const double radius)
        : _position(position)
        , _radius(radius) {}
    ~SphereCollider() override = default;
    StaticConstraint* checkContact(Particle& particle) override;
    void drawCollider(QPainter& painter, Viewport* viewport) override;

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
    void drawCollider(QPainter& painter, Viewport* viewport) override;

private:
    Vec2 _start;
    Vec2 _end;
};


#endif //POSITION_BASED_DYNAMIC_COLLIDER_H
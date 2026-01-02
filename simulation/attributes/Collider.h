#ifndef POSITION_BASED_DYNAMIC_COLLIDER_H
#define POSITION_BASED_DYNAMIC_COLLIDER_H


#include "Attribute.h"
#include "Vec2.h"

typedef struct Constraint {
    Vec2 contact;
    Vec2 normal;
} Constraint;

class BoxCollider;
class SphereCollider;

class Collider : public Attribute {

public:
    Collider(Entity* parent, double damping)
        : Attribute(parent)
        , _damping(damping) {}
    ~Collider() override = default;
    double getDamping() { return _damping; }
    virtual void checkContact(Entity* other) {}
    virtual Constraint* checkContactBox(Entity* other, BoxCollider* collider) { return nullptr; }
    virtual Constraint* checkContactSphere(Entity* other, SphereCollider* collider) { return nullptr; }

protected:
    double _damping;
};

class BoxCollider : public Collider {

public:
    BoxCollider(Entity* parent, double damping, double width, double height, Vec2 normal)
        : Collider(parent, damping)
        , _width(width)
        , _height(height)
        , _normal(normal) {}
    ~BoxCollider() override = default;
    double getWidth() { return _width; }
    double getHeight() { return _height; }
    Vec2 getNormal() { return _normal; }
    void checkContact(Entity* other) override;
    Constraint* checkContactBox(Entity* other, BoxCollider* collider) override;
    Constraint* checkContactSphere(Entity* other, SphereCollider* collider) override;

protected:
    double _width;
    double _height;
    Vec2 _normal;
};

class SphereCollider : public Collider {

public:
    SphereCollider(Entity* parent, double damping, double radius)
        : Collider(parent, damping)
        , _radius(radius) {}
    ~SphereCollider() override = default;
    double getRadius() { return _radius; }
    void checkContact(Entity* other) override;
    Constraint* checkContactBox(Entity* other, BoxCollider* collider) override;
    Constraint* checkContactSphere(Entity* other, SphereCollider* collider) override;

protected:
    double _radius;
};


#endif //POSITION_BASED_DYNAMIC_COLLIDER_H
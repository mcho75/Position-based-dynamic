#include "Collider.h"
#include "simulation/Entity.h"
#include <QDebug>

void BoxCollider::checkContact(Entity* other) {

    // if other doesn't have a collider, then there can't be any contact
    if (other->getCollider() == nullptr) {
        return;
    }

    // TODO: contact box/anything else
}

void SphereCollider::checkContact(Entity* other) {

    // if other doesn't have a collider or _parent doesn't have a rigidbody, then there can't be any contact
    if (other->getCollider() == nullptr || _parent->getRigidbody() == nullptr) {
        return;
    }

    // we get the position of the contact and its normal
    Constraint* constraint = other->getCollider()->checkContactSphere(_parent, this);

    // if other's collider doesn't intersect this, then there can't be any contact
    if (constraint == nullptr) {
        return;
    }

    // we get the data from the collision
    Vec2 normal = constraint->normal;
    Vec2 contact = constraint->contact;
    Vec2 tangent = Vec2(normal[1], -normal[0]);
    free(constraint);

    // we update the position and velocity based on the collision
    qDebug() << "Contact between " << _parent->getName() << " and " << other->getName() << " at " << contact[0] << contact[1];
    _parent->setPosition(contact + (_radius * normal));
    Vec2 tangentVelocity = (_parent->getVelocity() * tangent) * tangent;
    Vec2 normalVelocity = (_parent->getVelocity() * normal) * normal;
    _parent->setVelocity((1 - other->getCollider()->getDamping()) * tangentVelocity - _parent->getRigidbody()->getElasticity() * normalVelocity);
}

Constraint* BoxCollider::checkContactBox(Entity* other, BoxCollider* collider) {
    // TODO: contact box/box
    return nullptr;
}

Constraint* SphereCollider::checkContactBox(Entity* other, BoxCollider* collider) {
    // TODO: contact sphere/box
    return nullptr;
}

Constraint* BoxCollider::checkContactSphere(Entity* other, SphereCollider* collider) {

    Vec2 tangent = Vec2(_normal[1], -_normal[0]);
    double widthDistance = (other->getPosition() - _parent->getPosition()) * tangent;
    double heightDistance = (other->getPosition() - _parent->getPosition()) * _normal;

    // we check whether the sphere is inside the current BoxCollider
    if ((abs(widthDistance) - collider->getRadius() > _width / 2) || (abs(heightDistance) - collider->getRadius() > _height / 2)) {
        return nullptr;
    }
    if ((abs(widthDistance) > _width / 2) && (abs(heightDistance) > _height / 2)) {
        if ((other->getPosition() - (_width / 2 * tangent) + (_height / 2 * _normal) - _parent->getPosition()).norm() > collider->getRadius()
            && (other->getPosition() + (_width / 2 * tangent) + (_height / 2 * _normal) - _parent->getPosition()).norm() > collider->getRadius()
            && (other->getPosition() - (_width / 2 * tangent) - (_height / 2 * _normal) - _parent->getPosition()).norm() > collider->getRadius()
            && (other->getPosition() + (_width / 2 * tangent) - (_height / 2 * _normal) - _parent->getPosition()).norm() > collider->getRadius()) {
            return nullptr;
        }
    }

    // we search the closest contact point on each border
    if (widthDistance < 0) {
        widthDistance = std::max(widthDistance, -_width / 2);
    } else {
        widthDistance = std::min(widthDistance, _width / 2);
    }
    if (heightDistance < 0) {
        heightDistance = std::max(heightDistance, -_height / 2);
    } else {
        heightDistance = std::min(heightDistance, _height / 2);
    }
    Constraint constraints[4] = {
        {_parent->getPosition() + (widthDistance * tangent) + (_height / 2 * _normal), _normal},
        {_parent->getPosition() + (widthDistance * tangent) - (_height / 2 * _normal), _normal * -1},
        {_parent->getPosition() + (heightDistance * _normal) + (_width / 2 * tangent), tangent},
        {_parent->getPosition() + (heightDistance * _normal) - (_width / 2 * tangent), tangent * -1}
    };
    Constraint* minConstraint = new Constraint{constraints[0].contact, constraints[0].normal};
    for (auto& constraint : constraints) {
        if ((other->getPosition() - constraint.contact).norm() < (other->getPosition() - minConstraint->contact).norm()) {
            minConstraint->normal = constraint.normal;
            minConstraint->contact = constraint.contact;
        }
    }
    return minConstraint;
}

Constraint* SphereCollider::checkContactSphere(Entity* other, SphereCollider* collider) {

    // we check whether the sphere is inside the current SphereCollider
    if ((other->getPosition() - _parent->getPosition()).norm() - _radius - collider->getRadius() > 0) {
        return nullptr;
    }

    // we search the closest contact point on the border
    Vec2 normal = other->getPosition() - _parent->getPosition();
    normal /= normal.norm();
    return new Constraint{_parent->getPosition() + (_radius * normal), normal};
}
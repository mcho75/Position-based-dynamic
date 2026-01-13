#include "Collider.h"
#include "simulation/Entity.h"
#include <QDebug>
#include <cmath>

void BoxCollider::checkContact(Entity* other) {

    // if other doesn't have a collider or _parent doesn't have a rigidbody, then there can't be any contact
    if (other->getCollider() == nullptr || _parent->getRigidbody() == nullptr) {
        return;
    }

    // we get the position of the contact and its normal
    Constraint* constraint = other->getCollider()->checkContactBox(_parent, this);

    // if other's collider doesn't intersect this, then there can't be any contact
    if (constraint == nullptr) {
        return;
    }

    // we get the data from the collision
    Vec2 normal = constraint->normal;
    free(constraint);

    // we update the position and velocity based on the collision
    // TODO

    // the mass of the other object allows for an energy transfer
    if (other->getRigidbody() != nullptr) {
        double v1 = _parent->getVelocity().norm();
        double v2 = other->getVelocity().norm();
        double mass1 = _parent->getRigidbody()->getMass();
        double mass2 = other->getRigidbody()->getMass();
        double v1_updated = (2 * mass2 * v2 + v1 * (mass1 - mass2)) / (mass1 + mass2);
        _parent->setVelocity(-v1_updated * normal);
    }
}

// https://www.owl-ge.ch/IMG/pdf/choc_2D_avec_citation.pdf pour la formule des collisions

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
    _parent->setPosition(contact + (_radius * normal));
    Vec2 tangentVelocity = (_parent->getVelocity() * tangent) * tangent;
    Vec2 normalVelocity = (_parent->getVelocity() * normal) * normal;
    _parent->setVelocity((1 - other->getCollider()->getDamping()) * tangentVelocity - _parent->getRigidbody()->getElasticity() * normalVelocity);

    // the mass of the other object allows for an energy transfer
    if (other->getRigidbody() != nullptr) {
        double v1 = _parent->getVelocity().norm();
        double v2 = other->getVelocity().norm();
        double mass1 = _parent->getRigidbody()->getMass();
        double mass2 = other->getRigidbody()->getMass();
        double alpha1 = (_parent->getVelocity() * normal) / (_parent->getVelocity().norm());
        double alpha2 = (other->getVelocity()*normal) / (other->getVelocity().norm());
        double theta1 = atan( (mass1 - mass2) * tan(90 - alpha1) / (mass1 + mass2) + 2 * mass2 / (mass1 + mass2) * v2 / v1 * sin(90 - alpha2) / cos(90 - alpha1)) ;
        double alpha1_prime = 90 - theta1; // angle between normal and directing vector
        double v1_updated = sqrt( (((mass1 - mass2)*v1*sin(90-alpha1))/(mass1+mass2) + 2*mass2*v2*sin(90-alpha2)/(mass1+mass2)) * (((mass1 - mass2)*v1*sin(90-alpha1))/(mass1+mass2) + 2*mass2*v2*sin(90-alpha2)/(mass1+mass2)) + (v1*cos(90-alpha1))*(v1*cos(90-alpha1)));
        _parent->setVelocity(_parent->getVelocity() - v1_updated * (cos(alpha1_prime) * normal + sin(alpha1_prime) * tangent));
    }
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

    // we search the closest contact point on each border
    Constraint constraints[4] = {
        {_parent->getPosition() + (widthDistance * tangent) + (_height / 2 * _normal), _normal},
        {_parent->getPosition() + (widthDistance * tangent) - (_height / 2 * _normal), _normal * -1},
        {_parent->getPosition() + (heightDistance * _normal) + (_width / 2 * tangent), tangent},
        {_parent->getPosition() + (heightDistance * _normal) - (_width / 2 * tangent), tangent * -1}
    };
    Constraint* minConstraint = new Constraint{constraints[0].contact, constraints[0].normal};
    qDebug() << "Before: " << minConstraint->contact[0] << minConstraint->contact[1];
    for (auto& constraint : constraints) {
        qDebug() << "Checking " << constraint.contact[0] << constraint.contact[1];
        if ((other->getPosition() - constraint.contact).norm() < (other->getPosition() - minConstraint->contact).norm()) {
            minConstraint->normal = constraint.normal;
            minConstraint->contact = constraint.contact;
        }
    }
    qDebug() << "Best result: " << minConstraint->contact[0] << minConstraint->contact[1];
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
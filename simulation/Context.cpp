#include "Context.h"

void Context::addEntity(Entity* entity) {
    _entities.push_front(entity);
}

void Context::update() {
    checkRigidbodies();
    checkColliders();
    updatePositions();
}

void Context::checkRigidbodies() {

    // constants
    // TODO: correct gravity error
    Vec2 g(0.0, -90.81);
    double lambda = -5;

    // for each particle, we apply forces if there is a rigidbody
    for (auto entity : _entities) {
        if (entity->getRigidbody() != nullptr) {
            entity->getRigidbody()->applyGravity(g, _dt);
            entity->getRigidbody()->applyFriction(lambda, _dt);
        }
    }
}

void Context::checkColliders() {

    // we check collisions between entities that both have a collider (only rigidbodies can move)
    for (auto entity1 : _entities) {
        if (entity1->getCollider() != nullptr && entity1->getRigidbody() != nullptr) {
            for (auto entity2 : _entities) {
                if (entity1->getId() != entity2->getId()) {
                    entity1->getCollider()->checkContact(entity2);
                }
            }
        }
    }
}

void Context::updatePositions() {
    for (auto entity : _entities) {
        entity->setPosition(entity->getPosition() + _dt * entity->getVelocity());
    }
}
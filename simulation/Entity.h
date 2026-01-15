#ifndef POSITION_BASED_DYNAMIC_ENTITY_H
#define POSITION_BASED_DYNAMIC_ENTITY_H


#include "attributes/Collider.h"
#include "attributes/Rigidbody.h"
#include "attributes/Sprite.h"

class Entity {

public:
    Entity(std::string name, Vec2 position, Vec2 velocity)
        : _id(++UNIQUE_ID)
        , _name(name)
        , _position(position)
        , _velocity(velocity) {}
    ~Entity() = default;
    std::string getName() { return _name; }
    int getId() { return _id; }
    Vec2 getPosition() { return _position; }
    Vec2 getVelocity() { return _velocity; }
    Collider* getCollider() { return _collider; }
    Rigidbody* getRigidbody() { return _rigidbody; }
    Sprite* getSprite() { return _sprite; }
    void setName(std::string name) { _name = name; }
    void setPosition(Vec2 position) {
        position = Vec2(std::max(-1000.,  position[0]), std::max(-1000.,position[1]));
        _position = position;
    }
    void setVelocity(Vec2 velocity) { _velocity = velocity; }
    void setCollider(Collider* collider) { _collider = collider; }
    void setRigidbody(Rigidbody* rigidbody) { _rigidbody = rigidbody; }
    void setSprite(Sprite* sprite) { _sprite = sprite; }

protected:
    static int UNIQUE_ID;
    int _id;
    std::string _name;
    Vec2 _position;
    Vec2 _velocity;
    Collider* _collider = nullptr;
    Rigidbody* _rigidbody = nullptr;
    Sprite* _sprite = nullptr;
};


#endif //POSITION_BASED_DYNAMIC_ENTITY_H
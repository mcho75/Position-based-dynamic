#ifndef POSITION_BASED_DYNAMIC_ATTRIBUTE_H
#define POSITION_BASED_DYNAMIC_ATTRIBUTE_H


class Entity;

class Attribute {

public:
    Attribute(Entity* parent)
        : _parent(parent) {}
    virtual ~Attribute() = default;

protected:
    Entity* _parent;
};


#endif //POSITION_BASED_DYNAMIC_ATTRIBUTE_H
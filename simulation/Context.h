#ifndef POSITION_BASED_DYNAMIC_CONTEXT_H
#define POSITION_BASED_DYNAMIC_CONTEXT_H


#include <qlist.h>
#include "Entity.h"

class Context {

public:
    Context(double dt)
        : _dt(dt)
        , _entities(QList<Entity*>()) {}
    ~Context() = default;
    double getDt() { return _dt; }
    QList<Entity*> getEntities() { return _entities; }
    void addEntity(Entity* entity);
    void update();
    void checkRigidbodies();
    void checkColliders();
    void updatePositions();

private:
    double _dt;
    QList<Entity*> _entities;
};


#endif //POSITION_BASED_DYNAMIC_CONTEXT_H
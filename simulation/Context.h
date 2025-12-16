#ifndef POSITION_BASED_DYNAMIC_CONTEXT_H
#define POSITION_BASED_DYNAMIC_CONTEXT_H


#include <qlist.h>
#include "Particle.h"

class Context {

public:
    Context() : _particles(QList<Particle>()) {}
    ~Context() = default;
    void addParticle(const Particle& particle);
    QList<Particle> getParticles();
    void updatePhysicalSystem(double dt);

private:
    QList<Particle> _particles;
};


#endif //POSITION_BASED_DYNAMIC_CONTEXT_H
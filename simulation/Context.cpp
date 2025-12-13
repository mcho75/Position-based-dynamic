#include "Context.h"

void Context::addParticle(const Particle* particle) {
    Context::_particles.append(*particle);
}

void Context::updatePhysicalSystem(double dt) {}
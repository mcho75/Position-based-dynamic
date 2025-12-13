#include "Context.h"

void Context::addParticle(Particle* particle) {
    _particles.append(*particle);
}

QList<Particle> Context::getParticles() {
    return _particles;
}

void Context::updatePhysicalSystem(double dt) {
    Vec2 Fext(0, -9000.81);

    // compute velocities
    for (Particle& particle : _particles) {
        particle.velocity = particle.velocity + (Fext * dt / particle.mass);
    }

    // compute positions
    for (Particle& particle : _particles) {
        particle.position = particle.position + (particle.velocity * dt);
    }
}
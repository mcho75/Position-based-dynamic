#include "Collider.h"
#include "math.h"

StaticConstraint* SphereCollider::checkContact(Particle& particle) {
    double norm = sqrt((particle.position[0]- _position[0])*(particle.position[0]- _position[0]) + particle.position[1]- _position[1])*(particle.position[1]- _position[1]);
    Vec2 nc ((particle.position-_position)/norm);
    if (norm - _radius < 0) {
        return new StaticConstraint{particle, particle.position-(norm - _radius)*nc, nc};
    }
}

StaticConstraint* PlanCollider::checkContact(Particle& particle) {
    double a = (_end[1]- _start[1])/(_end[0]-_start[0]);
    Vec2 pc((_end[0]+_start[0])/2, (_end[1]+_start[1])/2);
    double x = particle.position[0];
    double y = particle.position[1];
    Vec2 n(a, -1);
    double ri = particle.radius;
    if (n[0]*(x-pc[0])+ n[1]*n[1]*(y-pc[1]) - ri < 0){ // result of the check
        return new StaticConstraint{particle, pc, n};
    }
    return nullptr;
    Vec2 qc(x-n[0]*n[0]*(x-pc[0])+ n[1]*n[1]*(y-pc[1]), y-n[0]*n[0]*(x-pc[0])+ n[1]*n[1]*(y-pc[1]));
    double C = (n[0]*(x-qc[0])+n[1]*(y-qc[1]))-ri;
    Vec2 delta_i(-C*n[0], -C*n[1]);
}
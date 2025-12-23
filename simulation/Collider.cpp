#include <cmath>
#include "Collider.h"
#include "widgets/Viewport.h"

StaticConstraint* SphereCollider::checkContact(Particle& particle, const double dt) {
    double norm = (particle.position - _position).norm();
    Vec2 nc = (particle.position - _position) / norm;
    if (norm - particle.radius - _radius < 0) {
        return new StaticConstraint{particle, particle.position - (norm - particle.radius - _radius) * nc};
    }
    return nullptr;
}

StaticConstraint* PlanCollider::checkContact(Particle& particle, const double dt) {
    Vec2 pc = (_end + _start) / 2;
    Vec2 nc(_start[1] - _end[1], _end[0] - _start[0]);
    nc = nc / nc.norm();
    if ((particle.position - pc) * nc - particle.radius < 0) {
        nc = nc * -1;
    }
    if ((particle.position + particle.velocity * dt - pc) * nc - particle.radius < 0) {
        Vec2 tc(_end[0] - _start[0], _end[1] - _start[1]);
        tc = tc / tc.norm();
        Vec2 qc = pc + tc * ((particle.position - pc) * tc);
        if (qc[0] >= std::min(_start[0], _end[0]) && qc[0] <= std::max(_start[0], _end[0])
            && qc[1] >= std::min(_start[1], _end[1]) && qc[1] <= std::max(_start[1], _end[1])) {
            return new StaticConstraint{particle, nc * (particle.velocity * nc * -1) + tc * (particle.velocity * tc)};
        }
    }
    return nullptr;
}

void SphereCollider::drawCollider(QPainter& painter, Viewport* viewport) {
    painter.setBrush(Qt::black);
    painter.setPen(Qt::red);
    Vec2 position = viewport->worldToView(_position );
    Vec2 radius = viewport->worldToView(Vec2(_radius, _radius));
    painter.drawEllipse(position[0] - radius[0], position[1] - radius[1], radius[0], radius[1]);
}

void PlanCollider::drawCollider(QPainter& painter, Viewport* viewport) {
    painter.setBrush(Qt::black);
    painter.setPen(Qt::red);
    Vec2 start = viewport->worldToView(_start);
    Vec2 end = viewport->worldToView(_end);
    painter.drawLine(start[0], start[1], end[0], end[1]);
}
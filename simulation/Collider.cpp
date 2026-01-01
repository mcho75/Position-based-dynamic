#include <cmath>
#include "Collider.h"
#include "widgets/Viewport.h"

StaticConstraint* SphereCollider::checkContact(Particle& particle) {
    double norm = (particle.position - _position).norm();
    if (norm - _radius < -1e-10) {
        Vec2 nc = (particle.position - _position) / norm;
        Vec2 tc(nc[1], -nc[0]);
        Vec2 qc = _position + (_radius * nc);
        return new StaticConstraint{particle, this, nc, tc, qc};
    }
    return nullptr;
}

StaticConstraint* PlanCollider::checkContact(Particle& particle) {
    Vec2 pc = (_end + _start) / 2;
    Vec2 nc(_start[1] - _end[1], _end[0] - _start[0]);
    nc = nc / nc.norm();
    if ((particle.position - pc) * nc - (particle.radius * 0.99) < 0) {
        nc = nc * -1;
    }
    if ((particle.nextPosition - pc) * nc - (particle.radius * 0.99) < 0) {
        Vec2 tc(nc[1], -nc[0]);
        Vec2 qc = pc + ((particle.nextPosition - pc) * tc) * tc;
        if (qc[0] >= std::min(_start[0], _end[0]) && qc[0] <= std::max(_start[0], _end[0])
            && qc[1] >= std::min(_start[1], _end[1]) && qc[1] <= std::max(_start[1], _end[1])) {
            return new StaticConstraint{particle, this, nc, tc, qc};
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
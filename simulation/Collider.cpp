#include <cmath>
#include "Collider.h"
#include "widgets/Viewport.h"

StaticConstraint* SphereCollider::checkContact(Particle& particle) {
    double norm = sqrt((particle.position[0] - _position[0]) * (particle.position[0] - _position[0])
                     + (particle.position[1] - _position[1]) * (particle.position[1] - _position[1]));
    Vec2 nc = (particle.position - _position) / norm;
    if (norm - _radius < 0) {
        return new StaticConstraint{particle, particle.position - (norm - _radius) * nc};
    }
    return nullptr;
}

StaticConstraint* PlanCollider::checkContact(Particle& particle) {
    double a = (_end[1] - _start[1]) / (_end[0] - _start[0]);
    Vec2 pc = (_end + _start) / 2;
    Vec2 nc(-a, 1);
    nc = nc / sqrt(nc[0] * nc[0] + nc[1] * nc[1]);
    double isOverBefore = (particle.position - pc) * nc - particle.radius;
    if (isOverBefore < 0) {
        nc = nc * -1;
    }
    double isOverAfter = (particle.nextPosition - pc) * nc - particle.radius;
    if (isOverAfter < 0) {
        if (particle.position[0] > std::min(_start[0], _end[0])
                && particle.position[0] < std::max(_start[0], _end[0])
                && particle.position[1] > std::min(_start[1], _end[1])
                && particle.position[1] < std::max(_start[1], _end[1])) {
            Vec2 qc = particle.nextPosition - (nc * isOverAfter);
            double C = (particle.nextPosition - qc) * nc - particle.radius;
            return new StaticConstraint{particle, nc * -C};
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
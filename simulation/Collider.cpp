#include "Collider.h"
#include "math.h"
#include "widgets/Viewport.h"

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
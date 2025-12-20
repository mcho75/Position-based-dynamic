#include "Collider.h"
#include "widgets/Viewport.h"

StaticConstraint* SphereCollider::checkContact(Particle& particle) {
    if (true) {
        return nullptr;   // no collision
    }
    // compute normal and position
    return new StaticConstraint{particle, {0, 0}, {0, 0}};
}

StaticConstraint* PlanCollider::checkContact(Particle& particle) {
    if (true) {
        return nullptr;   // no collision
    }
    // compute normal and position
    return new StaticConstraint{particle, {0, 0}, {0, 0}};
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
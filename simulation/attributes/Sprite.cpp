#include "Sprite.h"
#include "widgets/Viewport.h"

void BoxSprite::paint(Viewport* viewport, QPainter& painter) {
    painter.setBrush(_brush);
    painter.setPen(_pen);
    Vec2 tangent = Vec2(_normal[1], -_normal[0]);
    Vec2 p1 = viewport->worldToView(_parent->getPosition() + (_width / 2 * tangent) + (_height / 2 * _normal));
    Vec2 p2 = viewport->worldToView(_parent->getPosition() + (_width / 2 * tangent) - (_height / 2 * _normal));
    Vec2 p3 = viewport->worldToView(_parent->getPosition() - (_width / 2 * tangent) - (_height / 2 * _normal));
    Vec2 p4 = viewport->worldToView(_parent->getPosition() - (_width / 2 * tangent) + (_height / 2 * _normal));
    QList<QPoint> points;
    points.append(QPoint(p1[0], p1[1]));
    points.append(QPoint(p2[0], p2[1]));
    points.append(QPoint(p3[0], p3[1]));
    points.append(QPoint(p4[0], p4[1]));
    points.append(QPoint(p1[0], p1[1]));
    QPolygon polygon = QPolygon(points);
    painter.drawPolygon(polygon);
}

void SphereSprite::paint(Viewport* viewport, QPainter& painter) {
    painter.setBrush(_brush);
    painter.setPen(_pen);
    Vec2 pos = viewport->worldToView(_parent->getPosition());
    double radius = viewport->toScale(_radius);
    QRect dimensions = QRect(pos[0] - radius, pos[1] - radius, radius * 2, radius * 2);
    painter.drawEllipse(dimensions);
}
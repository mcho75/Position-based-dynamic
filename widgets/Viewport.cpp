#include "Viewport.h"
#include <QPainter>
#include <QPaintEvent>

Viewport::Viewport(QWidget* parent)
    : QOpenGLWidget(parent) {}

Vec2 Viewport::worldToView(Vec2 worldPos) {
    return Vec2(worldPos[0], height() - worldPos[1]);
}

Vec2 Viewport::viewToWorld(Vec2 viewPos) {
    return Vec2(viewPos[0], height() - viewPos[1]);
}

void Viewport::setDt(int dt) {
    _dt = dt;
}

void Viewport::paintEvent(QPaintEvent* event) {
    qDebug() << "PaintEvent";
    if (_context == nullptr) {
        return;
    }
    QList<Particle> particles = _context->getParticles();
    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));
    QRect dimensions = QRect(0, 0, width(), height());

    painter.setPen(Qt::blue);
    painter.setBrush(QBrush(Qt::red));
    for (Particle& particle : particles) {
        Vec2 pos = worldToView(particle.position);
        double radius = particle.radius;
        dimensions.setRect(pos[0] - radius / 2, pos[1] - radius / 2, radius, radius);
        painter.drawEllipse(dimensions);
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent* event) {
    Vec2 worldPos = viewToWorld(Vec2(event->pos().x(), event->pos().y()));
    _context->addParticle(new Particle(worldPos, Vec2(0, 0), 10, 50));
}

void Viewport::setContext(Context* context) {
    _context = context;
}

void Viewport::animate() {
    if (_context == nullptr) {
        return;
    }
    _context->updatePhysicalSystem(((double) _dt) / 1000);
    update();
}
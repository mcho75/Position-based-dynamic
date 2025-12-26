#include "Viewport.h"

double Viewport::toScale(double value) {
    return value * _scale;
}

double Viewport::fromScale(double value) {
    return value / _scale;
}

Vec2 Viewport::worldToView(Vec2 worldPos) {
    return Vec2(toScale(worldPos[0]), height() - toScale(worldPos[1]));
}

Vec2 Viewport::viewToWorld(Vec2 viewPos) {
    return Vec2(fromScale(viewPos[0]), fromScale(height() - viewPos[1]));
}

void Viewport::setDt(double dt) {
    _dt = dt;
}

void Viewport::setScale(double scale) {
    _scale = scale;
}

void Viewport::paintEvent(QPaintEvent* event) {
    if (_context == nullptr) {
        return;
    }

    QList<Particle> particles = _context->getParticles();
    QList<Collider*> colliders = _context->getColliders();
    QPainter painter = QPainter(this);

    painter.fillRect(event->rect(), QBrush(Qt::white));

    // draw particles
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::blue);
    for (Particle& particle : particles) {
        Vec2 pos = worldToView(particle.position);
        double radius = toScale(particle.radius);
        QRect dimensions = QRect(pos[0] - radius, pos[1] - radius, radius * 2, radius * 2);
        painter.drawEllipse(dimensions);
    }

    // draw colliders
    for (Collider* collider : colliders) {
        collider->drawCollider(painter, this);
    }

    // draw hypothetic collider
    if (_creatingWall) {
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::gray);
        painter.drawLine(_startClick[0], _startClick[1], _endClick[0], _endClick[1]);
    }
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    if (!_creatingWall) {
        _creatingWall = true;
        _startClick = Vec2(event->pos().x(), event->pos().y());
        _endClick = Vec2(event->pos().x(), event->pos().y());
    }
}

void Viewport::mouseDoubleClickEvent(QMouseEvent *event) {
    Vec2 worldPos = viewToWorld(Vec2(event->pos().x(), event->pos().y()));
    _context->addParticle({worldPos, worldPos, Vec2(0, 0), 0.01, 5.0, 0.5});
}

void Viewport::mouseMoveEvent(QMouseEvent* event) {
    if (_creatingWall) {
        _endClick = Vec2(event->pos().x(), event->pos().y());
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent* event) {
    if (_creatingWall) {
        _creatingWall = false;
        _endClick = Vec2(event->pos().x(), event->pos().y());
        _context->addCollider(new PlanCollider(viewToWorld(_startClick), viewToWorld(_endClick), 0.8));
    }
}

void Viewport::setContext(Context* context) {
    _context = context;
    double border = 20;
    Vec2 dim1 = viewToWorld(Vec2(border, border));
    Vec2 dim2 = viewToWorld(Vec2(width() - border, height() - border));
    _context->initialize(dim1, dim2);
}

void Viewport::animate() {
    if (_context == nullptr) {
        return;
    }
    _context->updatePhysicalSystem(_dt);
    update();
}
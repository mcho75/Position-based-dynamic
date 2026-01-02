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

void Viewport::paintEvent(QPaintEvent* event) {

    // only paint if context is defined
    if (_context == nullptr) {
        return;
    }

    QPainter painter = QPainter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    // draw the sprite of each entity if it exists
    for (auto& entity : _context->getEntities()) {
        Sprite* sprite = entity->getSprite();
        if (sprite != nullptr) {
            sprite->paint(this, painter);
        }
    }

    // draw hypothetic collider
    if (!_placingParticles && _creatingWall) {
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::gray);
        painter.drawLine(_startClick[0], _startClick[1], _endClick[0], _endClick[1]);
    }
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    if (_placingParticles) {
        Vec2 worldPos = viewToWorld(Vec2(event->pos().x(), event->pos().y()));
        Entity* entity = new Entity("Particle " + std::to_string(_context->getEntities().length()), worldPos, Vec2(0, 0));
        entity->setCollider(new SphereCollider(entity, 0.05, 0.01));
        entity->setRigidbody(new Rigidbody(entity, 10, 0.05));
        entity->setSprite(new SphereSprite(entity, 0.01, Qt::blue, Qt::blue));
        _context->addEntity(entity);
    }
    else if (!_creatingWall) {
        _creatingWall = true;
        _startClick = Vec2(event->pos().x(), event->pos().y());
        _endClick = Vec2(event->pos().x(), event->pos().y());
    }
}

void Viewport::mouseMoveEvent(QMouseEvent* event) {
    if (_placingParticles) {
        Vec2 worldPos = viewToWorld(Vec2(event->pos().x(), event->pos().y()));
        Entity* entity = new Entity("Particle " + std::to_string(_context->getEntities().length()), worldPos, Vec2(0, 0));
        entity->setCollider(new SphereCollider(entity, 0.05, 0.01));
        entity->setRigidbody(new Rigidbody(entity, 10, 0.05));
        entity->setSprite(new SphereSprite(entity, 0.01, Qt::blue, Qt::blue));
        _context->addEntity(entity);
    }
    else if (_creatingWall) {
        _endClick = Vec2(event->pos().x(), event->pos().y());
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent* event) {
    if (_placingParticles) {}
    else if (_creatingWall) {
        _creatingWall = false;
        _endClick = Vec2(event->pos().x(), event->pos().y());
        Vec2 start = viewToWorld(_startClick);
        Vec2 end = viewToWorld(_endClick);
        Vec2 middle = (start + end) / 2;
        Vec2 tangent = start - end;
        tangent /= tangent.norm();
        Vec2 normal = Vec2(-tangent[1], tangent[0]);
        double width = (start - end).norm();
        Entity* entity = new Entity("Wall", middle, Vec2(0, 0));
        entity->setCollider(new BoxCollider(entity, 0.05, width, 0.02, normal));
        entity->setSprite(new BoxSprite(entity, width, 0.02, normal, Qt::red, Qt::red));
        _context->addEntity(entity);
    }
}

void Viewport::setContext(Context* context) {
    _context = context;
}

void Viewport::switchMode() {
    _placingParticles = !_placingParticles;
}

void Viewport::animate() {
    if (_context == nullptr) {
        return;
    }
    _context->update();
    update();
}
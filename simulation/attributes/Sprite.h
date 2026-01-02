#ifndef POSITION_BASED_DYNAMIC_SPRITE_H
#define POSITION_BASED_DYNAMIC_SPRITE_H


#include <QPainter>
#include "Vec2.h"
#include "Attribute.h"

class Viewport;

class Sprite : public Attribute {

public:
    Sprite(Entity* parent, QColor brush, QColor pen)
        : Attribute(parent)
        , _brush(brush)
        , _pen(pen) {}
    ~Sprite() override = default;
    virtual void paint(Viewport* viewport, QPainter& painter) {}

protected:
    QColor _brush;
    QColor _pen;
};

class BoxSprite : public Sprite {

public:
    BoxSprite(Entity* parent, double width, double height, Vec2 normal, QColor brush, QColor pen)
        : Sprite(parent, brush, pen)
        , _width(width)
        , _height(height)
        , _normal(normal) {}
    ~BoxSprite() override = default;
    void paint(Viewport* viewport, QPainter& painter) override;

protected:
    double _width;
    double _height;
    Vec2 _normal;
};

class SphereSprite : public Sprite {

public:
    SphereSprite(Entity* parent, double radius, QColor brush, QColor pen)
        : Sprite(parent, brush, pen)
        , _radius(radius) {}
    ~SphereSprite() override = default;
    void paint(Viewport* viewport, QPainter& painter) override;

protected:
    double _radius;
};


#endif //POSITION_BASED_DYNAMIC_SPRITE_H
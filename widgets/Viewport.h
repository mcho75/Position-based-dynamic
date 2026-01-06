#ifndef POSITION_BASED_DYNAMIC_VIEWPORT_H
#define POSITION_BASED_DYNAMIC_VIEWPORT_H


#include <QOpenGLWidget>
#include <QPaintEvent>
#include "simulation/Context.h"
#include "Vec2.h"

class Viewport : public QOpenGLWidget {
    Q_OBJECT

public:
    Viewport(QWidget* parent = nullptr)
        : QOpenGLWidget(parent) {}
    void setScale(double scale) { _scale = scale; }
    double toScale(double value);
    double fromScale(double value);
    Vec2 worldToView(Vec2 worldPos);
    Vec2 viewToWorld(Vec2 viewPos);
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void setContext(Context *context);
    void switchMode();
    void animate();

protected:
    Context* _context = nullptr;
    bool _placingParticles = true;
    bool _creatingWall = false;
    Vec2 _startClick = {0.0, 0.0};
    Vec2 _endClick = {0.0, 0.0};
    double _scale = 1;
};


#endif //POSITION_BASED_DYNAMIC_VIEWPORT_H
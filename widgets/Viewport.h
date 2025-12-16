#ifndef POSITION_BASED_DYNAMIC_VIEWPORT_H
#define POSITION_BASED_DYNAMIC_VIEWPORT_H


#include <QOpenGLWidget>
#include "simulation/Context.h"

class Viewport : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit Viewport(QWidget* parent = nullptr);
    double toScale(double value);
    double fromScale(double value);
    Vec2 worldToView(Vec2 worldPos);
    Vec2 viewToWorld(Vec2 viewPos);
    void setDt(double dt);
    void setScale(double scale);
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void setContext(Context *context);
    void animate();

private:
    Context* _context;
    double _dt;
    double _scale;
};


#endif //POSITION_BASED_DYNAMIC_VIEWPORT_H
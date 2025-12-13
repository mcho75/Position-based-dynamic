#ifndef POSITION_BASED_DYNAMIC_VIEWPORT_H
#define POSITION_BASED_DYNAMIC_VIEWPORT_H


#include <QOpenGLWidget>
#include <QPainter>

class Viewport : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit Viewport(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};


#endif //POSITION_BASED_DYNAMIC_VIEWPORT_H
#include "Viewport.h"

Viewport::Viewport(QWidget* parent)
    : QOpenGLWidget(parent) {}

void Viewport::paintEvent(QPaintEvent* event) {
    qDebug() << "PaintEvent";
}

void Viewport::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "MouseReleaseEvent";
}
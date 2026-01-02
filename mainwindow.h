#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "widgets/Viewport.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, double dt = 0.001);
    ~MainWindow() override;
    void keyPressEvent(QKeyEvent *e) override;

protected:
    Ui::MainWindow* _ui;
    Context* _context;
    Viewport* _viewport;
    QTimer* _timer;
};


#endif // MAINWINDOW_H

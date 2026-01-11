#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, double dt)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _context(new Context(dt))
    , _timer(new QTimer(this)) {

    _ui->setupUi(this);
    _viewport = _ui->viewport;
    _viewport->setFixedWidth(700);
    _viewport->setFixedHeight(500);
    _viewport->setScale(1000);
    _viewport->setContext(_context);

    connect(_timer, &QTimer::timeout, _viewport, &Viewport::animate);
    _timer->start(dt * 1000);
}

MainWindow::~MainWindow() {
    delete _ui;
    delete _context;
    delete _timer;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Space) {
        _viewport->switchMode();
    }
    if (e->key() == Qt::Key_Enter) {
        _viewport->animate();
    }
}

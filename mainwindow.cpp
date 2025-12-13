#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _context(new Context())
    , _timer(new QTimer(this)) {

    int dt = 10;

    ui->setupUi(this);
    _viewport = ui->viewport;
    _viewport->setContext(_context);
    _viewport->setDt(dt);

    connect(_timer, &QTimer::timeout, _viewport, &Viewport::animate);
    _timer->start(dt);
}

MainWindow::~MainWindow() {
    delete ui;
    delete _context;
    delete _timer;
}

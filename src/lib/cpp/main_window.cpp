#include "include/main_window.h"

namespace octane {
  MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralwidget = new QWidget(this);
    this->setCentralWidget(centralwidget);
    label            = new QLabel("hello");
    horizontalLayout = new QHBoxLayout(centralwidget);
    horizontalLayout->addWidget(label);
  }
  MainWindow::~MainWindow() {}
} // namespace octane
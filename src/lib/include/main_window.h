#ifndef OCTANE_MAIN_WINDOW_H_
#define OCTANE_MAIN_WINDOW_H_

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

namespace octane {
  class MainWindow : public QMainWindow {
    // Q_OBJECT
  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
  };
} // namespace octane

#endif // OCTANE_MAIN_WINDOW_H_
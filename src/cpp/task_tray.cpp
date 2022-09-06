#include "include/task_tray.h"

#include <QAction>
#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>

namespace octane::gui {
  TaskTray::TaskTray(QWidget *parent, const std::function<void()> &onClick)
    : QSystemTrayIcon(parent) {
    init(parent, onClick);
  }

  void TaskTray::init(QWidget *parent, const std::function<void()> &onClick) {
    QObject::connect(
      this, &QSystemTrayIcon::activated, [=](const auto &reason) {
        if (reason == QSystemTrayIcon::Trigger) {
          onClick();
        }
      });

    auto quit = new QAction(tr("&Quit"), parent);
    QObject::connect(quit, &QAction::triggered, qApp, [=]() {
      if (QMessageBox::question(
            parent, tr("Quit"), tr("Are you sure you want to quit?"))
          == QMessageBox::Yes) {
        qApp->quit();
      }
    });

    auto menu = new QMenu(parent);
    menu->addAction(quit);

    setIcon(QIcon(":/images/icon.png"));

    setContextMenu(menu);
  }
} // namespace octane::gui
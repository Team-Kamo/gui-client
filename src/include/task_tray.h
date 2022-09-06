#ifndef OCTANE_GUI_CLIENT_TASK_TRAY_H_
#define OCTANE_GUI_CLIENT_TASK_TRAY_H_

#include <QSystemTrayIcon>
#include <QWidget>
#include <functional>

namespace octane::gui {
  class TaskTray : public QSystemTrayIcon {
  public:
    TaskTray(QWidget *parent, const std::function<void()>& onClick);

  private:
    void init(QWidget* parent, const std::function<void()>& onClick);
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_TASK_TRAY_H_
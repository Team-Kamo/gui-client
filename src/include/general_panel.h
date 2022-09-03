#ifndef OCTANE_GUI_CLIENT_GENERAL_PANEL_H_
#define OCTANE_GUI_CLIENT_GENERAL_PANEL_H_

#include <QString>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <functional>

namespace octane::gui {
  class GeneralPanel : public QWidget {
  public:
    GeneralPanel(QWidget* parent,
                 const QString& roomId,
                 const std::function<void(const QString&)>& onSettingChanged,
                 const std::function<QString(const QString&)>& onCreateNewRoom);

  private:
    void initLayout(
      const QString& roomId,
      const std::function<void(const QString&)>& onSettingChanged,
      const std::function<QString(const QString&)>& onCreateNewRoom);
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_GENERAL_PANEL_H_
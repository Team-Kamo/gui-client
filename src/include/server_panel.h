#ifndef OCTANE_GUI_CLIENT_SERVER_PANEL_H_
#define OCTANE_GUI_CLIENT_SERVER_PANEL_H_

#include <QtWidgets/QWidget>

namespace octane::gui {
  class ServerPanel : public QWidget {
  public:
    ServerPanel(QWidget* parent);

  private:
    void initLayout();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_SERVER_PANEL_H_
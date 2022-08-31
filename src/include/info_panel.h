#ifndef OCTANE_GUI_CLIENT_INFO_PANEL_H_
#define OCTANE_GUI_CLIENT_INFO_PANEL_H_

#include <QtWidgets/QWidget>

namespace octane::gui {
  class InfoPanel : public QWidget {
  public:
    InfoPanel(QWidget *parent);

  private:
    void initLayout();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_INFO_PANEL_H_
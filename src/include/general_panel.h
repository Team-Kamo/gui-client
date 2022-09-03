#ifndef OCTANE_GUI_CLIENT_GENERAL_PANEL_H_
#define OCTANE_GUI_CLIENT_GENERAL_PANEL_H_

#include <QtWidgets/QWidget>

#include "./room.h"

namespace octane::gui {
  class GeneralPanel : public QWidget {
  public:
    GeneralPanel(QWidget* parent);

  private:
    void initLayout();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_GENERAL_PANEL_H_
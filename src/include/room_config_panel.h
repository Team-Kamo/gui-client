#ifndef OCTANE_GUI_CLIENT_ROOM_CONFIG_PANEL_H_
#define OCTANE_GUI_CLIENT_ROOM_CONFIG_PANEL_H_

#include <QtWidgets/QWidget>

namespace octane::gui {
  class RoomConfigPanel : public QWidget {
  public:
    RoomConfigPanel(QWidget* parent);

  private:
    void initLayout();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_ROOM_CONFIG_PANEL_H_
#include "include/room_config_panel.h"

#include <QtWidgets/QHBoxLayout>

namespace octane::gui {
  RoomConfigPanel::RoomConfigPanel(QWidget* parent) : QWidget(parent) {
    initLayout();
  }

  void RoomConfigPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    roomNameLabel = new QLabel();

    rootLayout->addWidget(roomNameLabel);
  }
} // namespace octane::gui
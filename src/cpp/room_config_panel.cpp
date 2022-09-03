#include "include/room_config_panel.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

#include "include/config.h"
#include "include/settings.h"

namespace octane::gui {
  RoomConfigPanel::RoomConfigPanel(QWidget* parent) : QWidget(parent) {
    initLayout();
  }

  void RoomConfigPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();

    auto roomNameLabel = new QLabel("ルーム名: ");
    auto roomNameInput = new QLabel();
    // roomNameInput->setDisabled(true);
    roomNameInput->setText(Settings::getAsStr(SETTING_KEY_ROOM_NAME));
    Settings::watchAsStr(SETTING_KEY_ROOM_NAME, [=](const auto& name) {
      roomNameInput->setText(name);
    });
    form->addRow(roomNameLabel, roomNameInput);

    auto roomIdLabel = new QLabel("ルームID: ");
    auto roomIdInput = new QLabel();
    // roomIdInput->setDisabled(true);
    roomIdInput->setText(Settings::getAsStr(SETTING_KEY_ROOM_ID));
    Settings::watchAsU64(SETTING_KEY_ROOM_ID, [=](auto id) {
      roomIdInput->setText(QString::number(id));
    });
    form->addRow(roomIdLabel, roomIdInput);

    rootLayout->addLayout(form);
  }
} // namespace octane::gui
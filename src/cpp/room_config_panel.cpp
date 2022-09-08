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

    const auto inRoom = [=]() {
      auto form = new QFormLayout();

      auto roomNameLabel = new QLabel("ルーム名: ");
      auto roomNameInput = new QLabel();
      roomNameInput->setText(Settings::getAsStr(SETTING_KEY_ROOM_NAME));
      Settings::watchAsStr(SETTING_KEY_ROOM_NAME, [=](const auto& name) {
        roomNameInput->setText(name);
      });
      form->addRow(roomNameLabel, roomNameInput);

      auto roomIdLabel = new QLabel("ルームID: ");
      auto roomIdInput = new QLabel();
      roomIdInput->setText(Settings::getAsStr(SETTING_KEY_ROOM_ID));
      Settings::watchAsU64(SETTING_KEY_ROOM_ID, [=](auto id) {
        roomIdInput->setText(QString::number(id));
      });
      form->addRow(roomIdLabel, roomIdInput);

      rootLayout->addLayout(form);
    };
    const auto outRoom = [=]() {
      auto label = new QLabel(
        "ルームに接続されていません。\n一般タブから接続してください。");
      rootLayout->addWidget(label);
    };

    if (Settings::getAsU64(SETTING_KEY_ROOM_ID) == 0) {
      outRoom();
    } else {
      inRoom();
    }

    Settings::watchAsU64(SETTING_KEY_ROOM_ID, [=](auto id) {
      qDeleteAll(
        rootLayout->findChildren<QWidget*>(Qt::FindDirectChildrenOnly));
      if (id == 0) {
        outRoom();
      } else {
        inRoom();
      }
    });
  }
} // namespace octane::gui
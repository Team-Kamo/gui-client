#include "include/general_panel.h"

#include <QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include "include/api.h"
#include "include/config.h"
#include "include/message_box.h"
#include "include/settings.h"

namespace octane::gui {
  GeneralPanel::GeneralPanel(QWidget* parent) : QWidget(parent) {
    initLayout();
  }

  void GeneralPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();

    auto idLabel = new QLabel("ルームID: ");
    auto idInput = new QLineEdit();
    idInput->setText(Settings::getAsStr(SETTING_KEY_ROOM_ID));
    Settings::watchAsU64(SETTING_KEY_ROOM_ID, [=](auto roomId) {
      idInput->setText(QString::number(roomId));
    });
    form->addRow(idLabel, idInput);

    auto saveButton = new QPushButton("保存");
    form->addRow(saveButton);
    QObject::connect(saveButton, &QPushButton::clicked, qApp, [=]() {
      Settings::setAsU64(SETTING_KEY_ROOM_ID, idInput->text().toULongLong());
    });

    auto orLabel = new QLabel("またはルームを新規作成");
    orLabel->setAlignment(Qt::AlignHCenter);
    orLabel->setStyleSheet("margin: 1em");
    form->addRow(orLabel);

    auto nameLabel = new QLabel("ルーム名: ");
    auto nameInput = new QLineEdit();
    form->addRow(nameLabel, nameInput);

    auto newButton = new QPushButton("新規作成");
    form->addRow(newButton);
    QObject::connect(newButton, &QPushButton::clicked, qApp, [=]() {
      auto name = nameInput->text();
      if (name.size() == 0) {
        openCritical(this, tr("ルーム名を入力してください。"));
        return;
      }
      auto result = Api::createRoom(name);
      if (!result) {
        openCritical(this, result.err());
        return;
      }
      Settings::setAsU64(SETTING_KEY_ROOM_ID, result.get());
      Settings::setAsStr(SETTING_KEY_ROOM_NAME, name);
      nameInput->setText("");
    });

    rootLayout->addLayout(form);
  }
} // namespace octane::gui
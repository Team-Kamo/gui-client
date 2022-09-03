#include "include/general_panel.h"

#include <QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include "include/config.h"

namespace octane::gui {
  GeneralPanel::GeneralPanel(
    QWidget* parent,
    const QString& roomId,
    const std::function<void(const QString&)>& onSettingChanged,
    const std::function<QString(const QString&)>& onCreateNewRoom)
    : QWidget(parent) {
    initLayout(roomId, onSettingChanged, onCreateNewRoom);
  }

  void GeneralPanel::initLayout(
    const QString& roomId,
    const std::function<void(const QString&)>& onSettingChanged,
    const std::function<QString(const QString&)>& onCreateNewRoom) {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();

    auto idLabel = new QLabel("ルームID: ");
    auto idInput = new QLineEdit();
    idInput->setText(roomId);
    form->addRow(idLabel, idInput);

    auto saveButton = new QPushButton("保存");
    form->addRow(saveButton);
    QObject::connect(saveButton, &QPushButton::clicked, qApp, [=]() {
      onSettingChanged(idInput->text());
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
      auto newRoomId = onCreateNewRoom(idInput->text());
      idInput->setText(newRoomId);
    });

    rootLayout->addLayout(form);
  }
} // namespace octane::gui
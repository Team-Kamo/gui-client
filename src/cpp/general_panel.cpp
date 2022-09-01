#include "include/general_panel.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

namespace octane::gui {
  GeneralPanel::GeneralPanel(QWidget* parent)
    : QWidget(parent), idInput(nullptr) {
    initLayout();
  }

  void GeneralPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();

    auto idLabel = new QLabel("ルームID: ");
    idInput = new QLineEdit();
    form->addRow(idLabel, idInput);

    auto saveButton = new QPushButton("保存");
    form->addRow(saveButton);

    auto orLabel = new QLabel("またはルームを新規作成");
    orLabel->setAlignment(Qt::AlignHCenter);
    orLabel->setStyleSheet("margin: 1em");
    form->addRow(orLabel);

    auto nameLabel = new QLabel("ルーム名: ");
    auto nameInput = new QLineEdit();
    form->addRow(nameLabel, nameInput);

    auto newButton = new QPushButton("新規作成");
    form->addRow(newButton);

    rootLayout->addLayout(form);
  }
} // namespace octane::gui
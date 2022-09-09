#include "include/server_panel.h"

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
  ServerPanel::ServerPanel(QWidget* parent) : QWidget(parent) {
    initLayout();
  }

  void ServerPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    auto tokenLabel = new QLabel("APIトークン: ");
    auto tokenInput = new QLineEdit();
    tokenInput->setText(Settings::getAsStr(SETTING_KEY_API_TOKEN));
    Settings::watchAsStr(SETTING_KEY_API_TOKEN, [=](const auto& token) {
      tokenInput->setText(token);
    });
    form->addRow(tokenLabel, tokenInput);

    auto originLabel = new QLabel("APIオリジン: ");
    auto originInput = new QLineEdit();
    originInput->setText(Settings::getAsStr(SETTING_KEY_API_ORIGIN));
    Settings::watchAsStr(SETTING_KEY_API_ORIGIN, [=](const auto& origin) {
      originInput->setText(origin);
    });
    form->addRow(originLabel, originInput);

    auto baseUrlLabel = new QLabel("APIベースURL: ");
    auto baseUrlInput = new QLineEdit();
    baseUrlInput->setText(Settings::getAsStr(SETTING_KEY_API_BASE_URL));
    Settings::watchAsStr(SETTING_KEY_API_BASE_URL, [=](const auto& baseUrl) {
      baseUrlInput->setText(baseUrl);
    });
    form->addRow(baseUrlLabel, baseUrlInput);

    const auto initApi = [=]() {
      auto result = Api::init(Settings::getAsStr(SETTING_KEY_API_TOKEN),
                              Settings::getAsStr(SETTING_KEY_API_ORIGIN),
                              Settings::getAsStr(SETTING_KEY_API_BASE_URL));
      if (!result) {
        openCritical(this, result.err());
      }
      Settings::setAsU64(SETTING_KEY_ROOM_ID, 0);
      Settings::setAsStr(SETTING_KEY_ROOM_NAME, "");
    };

    auto restoreButton = new QPushButton("デフォルト設定に戻す");
    QObject::connect(restoreButton, &QPushButton::clicked, qApp, [=]() {
      Settings::setAsStr(SETTING_KEY_API_TOKEN, API_TOKEN);
      Settings::setAsStr(SETTING_KEY_API_ORIGIN, API_ORIGIN);
      Settings::setAsStr(SETTING_KEY_API_BASE_URL, API_BASE_URL);
      initApi();
    });
    auto saveButton = new QPushButton("保存");
    QObject::connect(saveButton, &QPushButton::clicked, qApp, [=]() {
      Settings::setAsStr(SETTING_KEY_API_TOKEN, tokenInput->text());
      Settings::setAsStr(SETTING_KEY_API_ORIGIN, originInput->text());
      Settings::setAsStr(SETTING_KEY_API_BASE_URL, baseUrlInput->text());
      initApi();
    });

    form->addRow(restoreButton, saveButton);

    rootLayout->addLayout(form);
  }
} // namespace octane::gui
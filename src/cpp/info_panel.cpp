#include "include/info_panel.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include "include/config.h"

namespace octane::gui {
  InfoPanel::InfoPanel(QWidget *parent) : QWidget(parent) {
    initLayout();
  }

  void InfoPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();

    auto serverLabel = new QLabel("Server Health: ");
    auto serverInfo  = new QLabel("Healthy");
    form->addRow(serverLabel, serverInfo);

    auto appLabel = new QLabel("Application: ");
    auto appInfo  = new QLabel(APP_NAME);
    form->addRow(appLabel, appInfo);

    auto orgLabel = new QLabel("Organization: ");
    auto orgInfo  = new QLabel(ORGANIZATION);
    form->addRow(orgLabel, orgInfo);

    auto versionLabel = new QLabel("Version: ");
    auto versionInfo  = new QLabel(VERSION);
    form->addRow(versionLabel, versionInfo);

    auto copyright = new QLabel(COPYRIGHT);
    copyright->setAlignment(Qt::AlignHCenter);
    form->addRow(copyright);

    rootLayout->addLayout(form);
  }
} // namespace octane::gui
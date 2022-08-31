/**
 * @file main_window.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-30
 *
 * main_window.hの実装。
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "include/main_window.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTabWidget>

#include "include/config.h"
#include "include/general_panel.h"
#include "include/info_panel.h"
#include "include/room_config_panel.h"

namespace octane::gui {
  MainWindow::MainWindow(QApplication* app, QWidget* parent)
    : QMainWindow(parent), hotkey(app), settings(ORGANIZATION, APP_NAME) {
    this->setWindowTitle(APP_NAME);
    initSettings();
    initLayout();
    registerHotkeys();
  }
  MainWindow::~MainWindow() {}

  void MainWindow::initSettings() {
    if (!settings.contains(SETTING_KEY_ROOM_ID)) {
      settings.setValue(SETTING_KEY_ROOM_ID, SETTING_DEFAULT_VALUE_ROOM_ID);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_COPY)) {
      settings.setValue(SETTING_KEY_KEYMAP_COPY,
                        SETTING_DEFAULT_VALUE_KEYMAP_COPY);
    }
  }

  void MainWindow::initLayout() {
    auto centralWidget = new QWidget(this);

    auto rootLayout = new QHBoxLayout(centralWidget);

    auto tabWidget       = new QTabWidget();
    auto generalPanel    = new GeneralPanel(tabWidget);
    auto roomConfigPanel = new RoomConfigPanel(tabWidget);
    auto infoPanel       = new InfoPanel(tabWidget);

    tabWidget->addTab(generalPanel, "一般");
    tabWidget->addTab(roomConfigPanel, "ルーム設定");
    tabWidget->addTab(infoPanel, "情報");

    rootLayout->addWidget(tabWidget);

    this->setCentralWidget(centralWidget);
  }
  void MainWindow::registerHotkeys() {
    // hotkey.addGlobalMapping
  }
} // namespace octane::gui
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

#include <QDebug>
#include <QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTabWidget>

#include "include/api.h"
#include "include/config.h"
#include "include/general_panel.h"
#include "include/info_panel.h"
#include "include/key_config_panel.h"
#include "include/room_config_panel.h"
#include "include/settings.h"

#if defined(Q_OS_WIN)
#include "windows/include/win_clipboard_manager.h"
#endif

namespace octane::gui {
  MainWindow::MainWindow(QApplication* app, QWidget* parent)
    : QMainWindow(parent),
      apiClient(API_TOKEN, API_ORIGIN, API_BASE_URL),
      copyFromSelectionHotkey(nullptr),
      copyFromClipboardHotkey(nullptr),
      clipboardManager(nullptr) {
    auto result = apiClient.init();
    if (!result) {
      QMessageBox::critical(
        this,
        tr("Error"),
        tr((result.err().code + "\n" + result.err().reason).c_str()));
      exit(1);
      return;
    }
    this->setWindowTitle(APP_NAME);
    initLayout();
    initClipboardManager();
    registerHotkeys();
  }
  MainWindow::~MainWindow() {}

  void MainWindow::initLayout() {
    auto centralWidget = new QWidget(this);

    auto rootLayout = new QHBoxLayout(centralWidget);

    auto tabWidget       = new QTabWidget();
    auto generalPanel    = new GeneralPanel(tabWidget);
    auto roomConfigPanel = new RoomConfigPanel(tabWidget);
    auto keyConfigPanel  = new KeyConfigPanel(tabWidget);
    auto infoPanel       = new InfoPanel(tabWidget);

    tabWidget->addTab(generalPanel, "一般");
    tabWidget->addTab(roomConfigPanel, "ルーム設定");
    tabWidget->addTab(keyConfigPanel, "キー設定");
    tabWidget->addTab(infoPanel, "情報");

    rootLayout->addWidget(tabWidget);

    this->setCentralWidget(centralWidget);
  }
  void MainWindow::initClipboardManager() {
#if defined(Q_OS_WIN)
    clipboardManager = new windows::WinClipboardManager();
#endif
    if (clipboardManager) {
      qApp->installNativeEventFilter(clipboardManager);
    }
  }
  void MainWindow::registerHotkeys() {
    const auto setCopyFromClipboardHotkey = [=](const QString& hotkey) {
      delete copyFromClipboardHotkey;
      copyFromClipboardHotkey = new QHotkey(hotkey, true, qApp);
      QObject::connect(
        copyFromClipboardHotkey, &QHotkey::activated, qApp, [=]() {
          qDebug() << "Activated 'copyFromClipboardHotkey'";
          if (clipboardManager == nullptr) return;
          auto data = clipboardManager->copyFromClipboard();
          if (!data) return;
          qDebug() << data->mime.c_str() << data->data;
          // TODO: 実装
          api.uploadContent(
            Settings::getAsU64(SETTING_KEY_ROOM_ID),
            Settings::getAsStr(SETTING_KEY_ROOM_NAME).toStdString(),
            {});
        });
    };
    const auto setCopyFromSelectionHotkey = [=](const QString& hotkey) {
      delete copyFromSelectionHotkey;
      copyFromSelectionHotkey = new QHotkey(hotkey, true, qApp);
      QObject::connect(
        copyFromSelectionHotkey, &QHotkey::activated, qApp, [&]() {
          qDebug() << "Activated 'copyFromSelectionHotkey'";
          if (clipboardManager == nullptr) return;
          clipboardManager->copyFromSelection([&](ClipboardData&& data) {
            qDebug() << data.mime.c_str() << data.data;
          });
        });
    };

    setCopyFromClipboardHotkey(
      Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD));
    setCopyFromClipboardHotkey(
      Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION));
  }
} // namespace octane::gui
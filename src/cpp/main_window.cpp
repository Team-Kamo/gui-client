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

#include "include/config.h"
#include "include/general_panel.h"
#include "include/info_panel.h"
#include "include/room_config_panel.h"

#if defined(Q_OS_WIN)
#include "windows/include/win_clipboard_manager.h"
#endif

namespace octane::gui {
  MainWindow::MainWindow(QApplication* app, QWidget* parent)
    : QMainWindow(parent),
      settings(ORGANIZATION, APP_NAME),
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
    initSettings();
    initLayout();
    initClipboardManager();
    registerHotkeys();
  }
  MainWindow::~MainWindow() {}

  void MainWindow::initSettings() {
    if (!settings.contains(SETTING_KEY_ROOM_ID)) {
      settings.setValue(SETTING_KEY_ROOM_ID, SETTING_DEFAULT_VALUE_ROOM_ID);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION)) {
      settings.setValue(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
                        SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_SELECTION);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD)) {
      settings.setValue(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
                        SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_CLIPBOARD);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION)) {
      settings.setValue(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
                        SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_SELECTION);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD)) {
      settings.setValue(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
                        SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_CLIPBOARD);
    }
  }

  void MainWindow::initLayout() {
    auto centralWidget = new QWidget(this);

    auto rootLayout = new QHBoxLayout(centralWidget);

    auto tabWidget    = new QTabWidget();
    auto generalPanel = new GeneralPanel(
      tabWidget,
      settings.value(SETTING_KEY_ROOM_ID).toString(),
      [=](const QString& roomId) {
        settings.setValue(SETTING_KEY_ROOM_ID, roomId);
      },
      [=](const QString& roomName) {
        auto result = apiClient.createRoom(roomName.toStdString());
        if (!result) {
          QMessageBox::critical(
            this,
            tr("Error"),
            tr((result.err().code + "\n" + result.err().reason).c_str()));
          return settings.value(SETTING_KEY_ROOM_ID).toString();
        }
        QString roomId = QString::fromStdString(result.get());
        settings.setValue(SETTING_KEY_ROOM_ID, roomId);
        return roomId;
      });
    auto roomConfigPanel = new RoomConfigPanel(tabWidget);
    auto infoPanel       = new InfoPanel(tabWidget);

    tabWidget->addTab(generalPanel, "一般");
    tabWidget->addTab(roomConfigPanel, "ルーム設定");
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
    copyFromClipboardHotkey = new QHotkey(
      QKeySequence(
        settings.value(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD).toString()),
      true,
      qApp);
    QObject::connect(copyFromClipboardHotkey, &QHotkey::activated, qApp, [&]() {
      qDebug() << "Activated 'copyFromClipboardHotkey'";
      if (clipboardManager) {
        if (auto data = clipboardManager->copyFromClipboard()) {
        }
      }
    });
    copyFromSelectionHotkey = new QHotkey(
      QKeySequence(
        settings.value(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION).toString()),
      true,
      qApp);
    QObject::connect(copyFromSelectionHotkey, &QHotkey::activated, qApp, [&]() {
      qDebug() << "Activated 'copyFromSelectionHotkey'";
      if (clipboardManager) {
        clipboardManager->copyFromSelection([&](ClipboardData&& data) {
          qDebug() << data.mime.c_str() << data.data;
        });
      }
    });
  }
} // namespace octane::gui
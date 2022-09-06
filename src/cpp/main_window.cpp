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
#include <QIcon>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTabWidget>

#include "include/api.h"
#include "include/config.h"
#include "include/general_panel.h"
#include "include/info_panel.h"
#include "include/key_config_panel.h"
#include "include/message_box.h"
#include "include/room_config_panel.h"
#include "include/settings.h"
#include "include/task_tray.h"

#if defined(Q_OS_WIN)
#include "windows/include/win_clipboard_manager.h"
#endif

namespace octane::gui {
  MainWindow::MainWindow(QApplication* app, QWidget* parent)
    : QMainWindow(parent),
      apiClient(API_TOKEN, API_ORIGIN, API_BASE_URL),
      copyFromSelectionHotkey(nullptr),
      copyFromClipboardHotkey(nullptr),
      pasteToSelectionHotkey(nullptr),
      pasteToClipboardHotkey(nullptr),
      clipboardManager(nullptr) {
    auto result = apiClient.init();
    if (!result) {
      openCritical(this, result.err());
      exit(1);
      return;
    }
    setWindowTitle(APP_NAME);
    setWindowIcon(QIcon(":/images/icon.png"));
    setAttribute(Qt::WA_QuitOnClose, false);
    initLayout();
    initClipboardManager();
    registerHotkeys();

    auto taskTray = new TaskTray(this, [=]() { show(); });
    taskTray->show();

    const auto connect = [=](uint64_t id) {
      if (id) {
        auto result = Api::connect(id);
        if (!result) {
          if (result.err().code == "ERR_DUP_DEVICE") {
            qDebug() << QString::fromStdString(result.err().code) << " "
                    << QString::fromStdString(result.err().reason);
          } else {
            openCritical(this, result.err());
          }
        }
      }
    };
    Settings::watchAsU64(SETTING_KEY_ROOM_ID, connect);
    connect(Settings::getAsU64(SETTING_KEY_ROOM_ID));

    qDebug() << "Initialize";
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
    clipboardManager = ClipboardManager::getSystemClipboardManager();
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
          Api::upload(data.value());
        });
    };
    const auto setCopyFromSelectionHotkey = [=](const QString& hotkey) {
      delete copyFromSelectionHotkey;
      copyFromSelectionHotkey = new QHotkey(hotkey, true, qApp);
      QObject::connect(
        copyFromSelectionHotkey, &QHotkey::activated, qApp, [=]() {
          qDebug() << "Activated 'copyFromSelectionHotkey'";
          if (clipboardManager == nullptr) return;
          clipboardManager->copyFromSelection(
            [&](ClipboardData&& data) { Api::upload(data); });
        });
    };
    const auto setPasteToClipboardHotkey = [=](const QString& hotkey) {
      delete pasteToClipboardHotkey;
      pasteToClipboardHotkey = new QHotkey(hotkey, true, qApp);
      QObject::connect(
        pasteToClipboardHotkey, &QHotkey::activated, qApp, [=]() {
          qDebug() << "Activated 'pasteToClipboardHotkey'";
          if (clipboardManager == nullptr) return;
          auto result = Api::download();
          if (!result) {
            openCritical(this, result.err());
            return;
          }
          clipboardManager->pasteToClipboard(result.get());
        });
    };
    const auto setPasteToSelectionHotkey = [=](const QString& hotkey) {
      delete pasteToSelectionHotkey;
      pasteToSelectionHotkey = new QHotkey(hotkey, true, qApp);
      QObject::connect(
        pasteToSelectionHotkey, &QHotkey::activated, qApp, [=]() {
          qDebug() << "Activated 'pasteToSelectionHotkey'";
          if (clipboardManager == nullptr) return;
          auto result = Api::download();
          if (!result) {
            openCritical(this, result.err());
            return;
          }
          clipboardManager->pasteToSelection(result.get());
        });
    };

    setCopyFromClipboardHotkey(
      Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD));
    setCopyFromSelectionHotkey(
      Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION));
    setPasteToClipboardHotkey(
      Settings::getAsStr(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD));
    setPasteToSelectionHotkey(
      Settings::getAsStr(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION));

    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
      [=](const auto& hotkey) { setCopyFromClipboardHotkey(hotkey); });
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
      [=](const auto& hotkey) { setCopyFromSelectionHotkey(hotkey); });
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
      [=](const auto& hotkey) { setPasteToClipboardHotkey(hotkey); });
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
      [=](const auto& hotkey) { setPasteToSelectionHotkey(hotkey); });
  }
} // namespace octane::gui
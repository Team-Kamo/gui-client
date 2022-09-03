/**
 * @file main_window.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-30
 *
 * メインウィンドウ。
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef OCTANE_GUI_CLIENT_MAIN_WINDOW_H_
#define OCTANE_GUI_CLIENT_MAIN_WINDOW_H_

#include <api_client.h>

#include <QApplication>
#include <QHotkey>
#include <QSettings>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include "./clipboard_manager.h"

namespace octane::gui {
  class MainWindow : public QMainWindow {
    QSettings settings;
    QHotkey* copyFromSelectionHotkey;
    QHotkey* copyFromClipboardHotkey;
    ClipboardManager* clipboardManager;

    ApiClient apiClient;

  public:
    MainWindow(QApplication* app, QWidget* parent = nullptr);
    ~MainWindow();

  private:
    void initSettings();
    void initLayout();
    void initClipboardManager();
    void registerHotkeys();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_MAIN_WINDOW_H_
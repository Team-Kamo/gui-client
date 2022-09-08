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

#include <QApplication>
#include <QHotkey>
#include <QSettings>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include "./clipboard_manager.h"

namespace octane::gui {
  class MainWindow : public QMainWindow {
    QHotkey* copyFromSelectionHotkey;
    QHotkey* copyFromClipboardHotkey;
    QHotkey* pasteToSelectionHotkey;
    QHotkey* pasteToClipboardHotkey;
    ClipboardManager* clipboardManager;

  public:
    MainWindow(QApplication* app, QWidget* parent = nullptr);
    ~MainWindow();

  private:
    void initLayout();
    void initClipboardManager();
    void registerHotkeys();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_MAIN_WINDOW_H_
/**
 * @file main.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-30
 *
 * liboctaneテスト用のmain関数置き場。
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <QApplication>

#include "include/main_window.h"

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

int WinMain([[maybe_unused]] HINSTANCE hInst,
            [[maybe_unused]] HINSTANCE hPrev,
            [[maybe_unused]] LPSTR lpCmdLine,
            [[maybe_unused]] int nCmdShow) {
  QApplication app(__argc, __argv);
  octane::gui::MainWindow mainWindow(&app);
  mainWindow.show();
  return app.exec();
}

#else

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  octane::gui::MainWindow mainWindow(&app);
  mainWindow.show();
  return app.exec();
}

#endif // _WIN32
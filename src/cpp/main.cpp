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
  auto mutex = CreateMutex(nullptr, false, TEXT("Global\\OctaneMutex"));
  if (mutex == nullptr) {
    MessageBox(nullptr,
               TEXT("Mutexの作成に失敗しました。"),
               TEXT("FATAL"),
               MB_ICONERROR);
    return GetLastError();
  }

  QApplication app(__argc, __argv);
  octane::gui::MainWindow mainWindow(&app);
  auto result = app.exec();

  CloseHandle(mutex);

  return result;
}

#else

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  octane::gui::MainWindow mainWindow(&app);
  return app.exec();
}

#endif // _WIN32
#include <QApplication>

#include "lib/include/main_window.h"

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

int WinMain([[maybe_unused]] HINSTANCE hInst,
            [[maybe_unused]] HINSTANCE hPrev,
            [[maybe_unused]] LPSTR lpCmdLine,
            [[maybe_unused]] int nCmdShow) {
  QApplication app(__argc, __argv);
  octane::MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}

#else

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  octane::MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}

#endif // _WIN32
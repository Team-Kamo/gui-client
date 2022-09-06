#include "include/win_clipboard_manager.h"

#include <Windows.h>
#include "../../include/tmp_dir.h"
#include <tchar.h>

#include <QDebug>

namespace octane::gui::windows {
  WinClipboardManager::WinClipboardManager() : ClipboardManager() {}
  WinClipboardManager::~WinClipboardManager() noexcept {}
  std::optional<ClipboardData> WinClipboardManager::copyFromClipboard() {
    return getClipboardData();
  }
  void WinClipboardManager::copyFromSelection(
    const std::function<void(ClipboardData &&)> &callback) {
    constexpr int inputSize = 6;
    INPUT inputs[inputSize];
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].ki.wVk     = VK_MENU;
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[1].ki.wVk = VK_CONTROL;

    inputs[2].ki.wVk = 'C';

    inputs[3].ki.wVk     = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[4].ki.wVk     = 'C';
    inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[5].ki.wVk = VK_MENU;

    for (auto &input : inputs) {
      input.type     = INPUT_KEYBOARD;
      input.ki.wScan = MapVirtualKey(input.ki.wVk, MAPVK_VK_TO_VSC);
    }

    if (SendInput(inputSize, inputs, sizeof(INPUT)) != inputSize) {
      qDebug() << HRESULT_FROM_WIN32(GetLastError());
      return;
    }

    copyFromSelectionCallback = callback;
  }
  void WinClipboardManager::pasteToClipboard(const ClipboardData &data) {
    setClipboardData(data, TmpDir::makeTmpDir());
  }
  void WinClipboardManager::pasteToSelection(const ClipboardData &data) {
    setClipboardData(data, TmpDir::makeTmpDir());

    constexpr int inputSize = 6;
    INPUT inputs[inputSize];
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].ki.wVk     = VK_MENU;
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[1].ki.wVk = VK_CONTROL;

    inputs[2].ki.wVk = 'V';

    inputs[3].ki.wVk     = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[4].ki.wVk     = 'V';
    inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[5].ki.wVk = VK_MENU;

    for (auto &input : inputs) {
      input.type     = INPUT_KEYBOARD;
      input.ki.wScan = MapVirtualKey(input.ki.wVk, MAPVK_VK_TO_VSC);
    }

    if (SendInput(inputSize, inputs, sizeof(INPUT)) != inputSize) {
      qDebug() << HRESULT_FROM_WIN32(GetLastError());
      return;
    }
  }
  bool WinClipboardManager::nativeEventFilter(const QByteArray &eventType,
                                              void *message,
                                              qintptr *result) {
    if (eventType != "windows_dispatcher_MSG"
        && eventType != "windows_generic_MSG") {
      return false;
    }
    switch (static_cast<MSG *>(message)->message) {
      case WM_CLIPBOARDUPDATE:
        if (copyFromSelectionCallback) {
          if (auto data = getClipboardData()) {
            copyFromSelectionCallback.value()(std::move(data.value()));
          }
          copyFromSelectionCallback = std::nullopt;
        }
        if (result) {
          (*result) = 0;
        }
        return true;
    }
    return false;
  }
} // namespace octane::gui::windows
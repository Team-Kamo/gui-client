#include "include/win_clipboard_manager.h"

#include <Windows.h>
#include <tchar.h>

#include <QDebug>

#include "../../include/tmp_dir.h"

namespace octane::gui::windows {
  WinClipboardManager::WinClipboardManager() : ClipboardManager() {}
  WinClipboardManager::~WinClipboardManager() noexcept {}
  std::optional<ClipboardData> WinClipboardManager::copyFromClipboard() {
    return getClipboardData();
  }
  void WinClipboardManager::copyFromSelection(
    const std::function<void(ClipboardData &&)> &callback) {
    INPUT inputs[] = {
      INPUT{
        .ki = KEYBDINPUT{
          .wVk = VK_MENU,
          .dwFlags = KEYEVENTF_KEYUP,
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = VK_CONTROL,
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = 'C',
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = VK_CONTROL,
          .dwFlags = KEYEVENTF_KEYUP,
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = 'C',
          .dwFlags = KEYEVENTF_KEYUP,
        },
      },
    };

    for (auto &input : inputs) {
      input.type     = INPUT_KEYBOARD;
      input.ki.wScan = MapVirtualKey(input.ki.wVk, MAPVK_VK_TO_VSC);
    }

    auto size = sizeof(inputs) / sizeof(inputs[0]);
    if (SendInput(size, inputs, sizeof(INPUT)) != size) {
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

    INPUT inputs[] = {
      INPUT{
        .ki = KEYBDINPUT{
          .wVk = VK_MENU,
          .dwFlags = KEYEVENTF_KEYUP,
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = VK_CONTROL,
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = 'V',
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = VK_CONTROL,
          .dwFlags = KEYEVENTF_KEYUP,
        },
      },
      INPUT {
        .ki = KEYBDINPUT{
          .wVk = 'V',
          .dwFlags = KEYEVENTF_KEYUP,
        },
      },
    };

    for (auto &input : inputs) {
      input.type     = INPUT_KEYBOARD;
      input.ki.wScan = MapVirtualKey(input.ki.wVk, MAPVK_VK_TO_VSC);
    }

    auto size = sizeof(inputs) / sizeof(inputs[0]);
    if (SendInput(size, inputs, sizeof(INPUT)) != size) {
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
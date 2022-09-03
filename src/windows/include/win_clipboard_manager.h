#ifndef OCTANE_GUI_CLIENT_WINDOWS_WIN_CLIPBOARD_MANAGER_H_
#define OCTANE_GUI_CLIENT_WINDOWS_WIN_CLIPBOARD_MANAGER_H_

#include <functional>
#include <optional>
#include <string>
#include <variant>

#include "../../include/clipboard_manager.h"

namespace octane::gui::windows {
  class WinClipboardManager : public ClipboardManager {
    std::optional<std::function<void(ClipboardData &&)>>
      copyFromSelectionCallback;

  public:
    WinClipboardManager();
    virtual ~WinClipboardManager() noexcept;
    virtual std::optional<ClipboardData> copyFromClipboard() override;
    virtual void copyFromSelection(
      const std::function<void(ClipboardData &&)> &callback) override;
    virtual void pasteToClipboard() override;
    virtual void pasteToSelection() override;
    virtual bool nativeEventFilter(const QByteArray &eventType,
                                   void *message,
                                   qintptr *result) override;

  private:
    std::optional<ClipboardData> getClipboardData();
  };
}; // namespace octane::gui::windows

#endif // OCTANE_GUI_CLIENT_WINDOWS_WIN_CLIPBOARD_MANAGER_H_
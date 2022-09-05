#ifndef OCTANE_GUI_DARWIN_OSX_CLIPBOARD_MANAGER_H_
#define OCTANE_GUI_DARWIN_OSX_CLIPBOARD_MANAGER_H_

#include <functional>
#include <optional>
#include <string>
#include <variant>

#include "../../include/clipboard_manager.h"

namespace octane::gui::darwin {
  class OsxClipboardManager : public ClipboardManager {
    std::optional<std::function<void(ClipboardData &&)>>
      copyFromSelectionCallback;

  public:
    OsxClipboardManager();
    virtual ~OsxClipboardManager() noexcept;
    virtual std::optional<ClipboardData> copyFromClipboard() override;
    virtual void copyFromSelection(
      const std::function<void(ClipboardData &&)> &callback) override;
    virtual void pasteToClipboard(const ClipboardData &data) override;
    virtual void pasteToSelection(const ClipboardData &data) override;
    virtual bool nativeEventFilter(const QByteArray &eventType,
                                   void *message,
                                   qintptr *result) override;

  private:
    std::optional<ClipboardData> getClipboardData();
    void setClipboardData(const ClipboardData &data);
  };
}

#endif // OCTANE_GUI_DARWIN_OSX_CLIPBOARD_MANAGER_H_
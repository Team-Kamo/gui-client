#ifndef OCTANE_GUI_CLIENT_CLIPBOARD_MANAGER_H_
#define OCTANE_GUI_CLIENT_CLIPBOARD_MANAGER_H_

#include <QAbstractNativeEventFilter>
#include <functional>
#include <optional>

#include "./clipboard_data.h"

namespace octane::gui {
  /**
   * @brief APIと通信し、クリップボードの管理をする抽象クラス。
   *
   */
  class ClipboardManager : public QAbstractNativeEventFilter {
  public:
    virtual ~ClipboardManager() noexcept                     = 0;
    virtual std::optional<ClipboardData> copyFromClipboard() = 0;
    virtual void copyFromSelection(
      const std::function<void(ClipboardData&&)>& callback)
      = 0;
    virtual void pasteToClipboard() = 0;
    virtual void pasteToSelection() = 0;
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CLIPBOARD_MANAGER_H_
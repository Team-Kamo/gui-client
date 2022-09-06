#ifndef OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_
#define OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_

#include <QBuffer>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace octane::gui {
  struct UniData {
    std::string mime;
    QByteArray data;
  };
  struct MultiData {
    std::unordered_map<std::string, QByteArray> files;
  };
  struct ClipboardData {
    std::variant<UniData, MultiData> data;
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_
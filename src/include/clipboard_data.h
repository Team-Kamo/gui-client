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
    std::vector<std::uint8_t> data;
  };
  struct MultiData {
    std::unordered_map<std::string, std::vector<std::uint8_t>> files;
  };
  struct ClipboardData {
    std::variant<UniData, MultiData> data;
  };
  bool operator==(const UniData& lhs, const UniData& rhs);
  bool operator!=(const UniData& lhs, const UniData& rhs);
  bool operator==(const MultiData& lhs, const MultiData& rhs);
  bool operator!=(const MultiData& lhs, const MultiData& rhs);
  bool operator==(const ClipboardData& lhs, const ClipboardData& rhs);
  bool operator!=(const ClipboardData& lhs, const ClipboardData& rhs);
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_
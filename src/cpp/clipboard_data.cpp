#include "include/clipboard_data.h"

namespace octane::gui {
  bool operator==(const UniData& lhs, const UniData& rhs) {
    return !(lhs != rhs);
  }
  bool operator!=(const UniData& lhs, const UniData& rhs) {
    if (lhs.mime != rhs.mime) return true;
    if (lhs.data != rhs.data) return true;
    return false;
  }
  bool operator==(const MultiData& lhs, const MultiData& rhs) {
    return !(lhs != rhs);
  }
  bool operator!=(const MultiData& lhs, const MultiData& rhs) {
    return lhs.files != rhs.files;
  }
  bool operator==(const ClipboardData& lhs, const ClipboardData& rhs) {
    return !(lhs != rhs);
  }
  bool operator!=(const ClipboardData& lhs, const ClipboardData& rhs) {
    return lhs.data != rhs.data;
  }
} // namespace octane::gui
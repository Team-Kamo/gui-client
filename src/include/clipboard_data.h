#ifndef OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_
#define OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_

#include <QBuffer>
#include <string>
#include <vector>

namespace octane::gui {
  struct ClipboardData {
    std::string mime;
    QByteArray data;
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CLIPBOARD_DATA_H_
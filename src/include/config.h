#ifndef OCTANE_GUI_CLIENT_CONFIG_H_
#define OCTANE_GUI_CLIENT_CONFIG_H_

#include <QApplication>

namespace octane::gui {
  constexpr auto MAJOR_VERSION = "0";
  constexpr auto MINOR_VERSION = "0";
  constexpr auto PATCH_VERSION = "0";
  constexpr auto VERSION       = "v0.0.0";

  constexpr auto APP_NAME     = "Octane";
  constexpr auto ORGANIZATION = "Team Kamo";
  constexpr auto COPYRIGHT    = "© 2022 Team Kamo";

  constexpr auto API_TOKEN    = "mock";
  constexpr auto API_ORIGIN   = "http://localhost:3000";
  constexpr auto API_BASE_URL = "/api/v1";

  constexpr auto SETTING_KEY_API_TOKEN              = "api/token";
  constexpr auto SETTING_DEFAULT_VALUE_API_TOKEN    = API_TOKEN;
  constexpr auto SETTING_KEY_API_ORIGIN             = "api/origin";
  constexpr auto SETTING_DEFAULT_VALUE_API_ORIGIN   = API_ORIGIN;
  constexpr auto SETTING_KEY_API_BASE_URL           = "api/base_url";
  constexpr auto SETTING_DEFAULT_VALUE_API_BASE_URL = API_BASE_URL;

  constexpr auto SETTING_KEY_ROOM_ID             = "room/id";
  constexpr auto SETTING_DEFAULT_VALUE_ROOM_ID   = 0;
  constexpr auto SETTING_KEY_ROOM_NAME           = "room/name";
  constexpr auto SETTING_DEFAULT_VALUE_ROOM_NAME = "";

  constexpr auto SETTING_KEY_KEYMAP_COPY_FROM_SELECTION
    = "keymap/copy_from_selection";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_SELECTION
#if defined(Q_OS_MAC)
    = "Meta+Shift+C";
#else
    = "Ctrl+Alt+C";
#endif // defined(Q_OS_MAC)

  constexpr auto SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD
    = "keymap/copy_from_clipboard";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_CLIPBOARD
#if defined(Q_OS_MAC)
    = "Meta+Shift+B";
#else
    = "Ctrl+Alt+B";
#endif // defined(Q_OS_MAC)

  constexpr auto SETTING_KEY_KEYMAP_PASTE_TO_SELECTION
    = "keymap/paste_to_selection";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_SELECTION
#if defined(Q_OS_MAC)
    = "Meta+Shift+V";
#else
    = "Ctrl+Alt+V";
#endif // defined(Q_OS_MAC)

  constexpr auto SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD
    = "keymap/paste_to_clipboard";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_CLIPBOARD
#if defined(Q_OS_MAC)
    = "Meta+Shift+G";
#else
    = "Ctrl+Alt+G";
#endif // defined(Q_OS_MAC)

} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CONFIG_H_
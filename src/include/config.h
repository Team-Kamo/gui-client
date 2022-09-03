#ifndef OCTANE_GUI_CLIENT_CONFIG_H_
#define OCTANE_GUI_CLIENT_CONFIG_H_

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

  constexpr auto SETTING_KEY_ROOM_ID             = "room/id";
  constexpr auto SETTING_DEFAULT_VALUE_ROOM_ID   = 0;
  constexpr auto SETTING_KEY_ROOM_NAME           = "room/name";
  constexpr auto SETTING_DEFAULT_VALUE_ROOM_NAME = "";

  constexpr auto SETTING_KEY_KEYMAP_COPY_FROM_SELECTION
    = "keymap/copy_from_selection";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_SELECTION
    = "Ctrl+Alt+C";

  constexpr auto SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD
    = "keymap/copy_from_clipboard";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_CLIPBOARD
    = "Ctrl+Alt+B";

  constexpr auto SETTING_KEY_KEYMAP_PASTE_TO_SELECTION
    = "keymap/paste_to_selection";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_SELECTION = "Ctrl+Alt+V";

  constexpr auto SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD
    = "keymap/paste_to_clipboard";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_CLIPBOARD = "Ctrl+Alt+G";
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CONFIG_H_
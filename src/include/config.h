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

  constexpr auto SETTING_KEY_ROOM_ID = "room/id";
  constexpr auto SETTING_DEFAULT_VALUE_ROOM_ID = "";

  constexpr auto SETTING_KEY_KEYMAP_COPY = "keymap/copy";
  constexpr auto SETTING_DEFAULT_VALUE_KEYMAP_COPY = "Ctrl+Alt+C";
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CONFIG_H_
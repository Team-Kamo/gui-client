#include "include/settings.h"

#include "include/config.h"

namespace octane::gui {
  Settings Settings::instance;
  Settings::Settings() : settings(ORGANIZATION, APP_NAME), cbCount(0) {
    if (!settings.contains(SETTING_KEY_API_TOKEN)) {
      settings.setValue(SETTING_KEY_API_TOKEN, SETTING_DEFAULT_VALUE_API_TOKEN);
    }

    if (!settings.contains(SETTING_KEY_API_ORIGIN)) {
      settings.setValue(SETTING_KEY_API_ORIGIN,
                        SETTING_DEFAULT_VALUE_API_ORIGIN);
    }

    if (!settings.contains(SETTING_KEY_API_BASE_URL)) {
      settings.setValue(SETTING_KEY_API_BASE_URL,
                        SETTING_DEFAULT_VALUE_API_BASE_URL);
    }

    if (!settings.contains(SETTING_KEY_ROOM_ID)) {
      settings.setValue(SETTING_KEY_ROOM_ID, SETTING_DEFAULT_VALUE_ROOM_ID);
    }

    if (!settings.contains(SETTING_KEY_ROOM_NAME)) {
      settings.setValue(SETTING_KEY_ROOM_NAME, SETTING_DEFAULT_VALUE_ROOM_NAME);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION)) {
      settings.setValue(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
                        SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_SELECTION);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD)) {
      settings.setValue(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
                        SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_CLIPBOARD);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION)) {
      settings.setValue(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
                        SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_SELECTION);
    }

    if (!settings.contains(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD)) {
      settings.setValue(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
                        SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_CLIPBOARD);
    }
  }

  QString Settings::getAsStr(const QString& key) {
    return instance.settings.value(key, "").toString();
  }
  std::uint64_t Settings::getAsU64(const QString& key) {
    return instance.settings.value(key, 0).toULongLong();
  }

  QString Settings::setAsStr(const QString& key, const QString& value) {
    instance.settings.setValue(key, value);
    instance.callStrCallbacks(key, value);
    return value;
  }
  std::uint64_t Settings::setAsU64(const QString& key, std::uint64_t value) {
    instance.settings.setValue(key, value);
    instance.callU64Callbacks(key, value);
    return value;
  }

  std::uint64_t Settings::watchAsStr(
    const QString& key,
    const std::function<void(const QString&)>& callback) {
    instance.strCallbackPool[instance.cbCount] = callback;
    instance.strCallbackMap[key].push_back(instance.cbCount);
    return instance.cbCount++;
  }
  std::uint64_t Settings::watchAsU64(
    const QString& key,
    const std::function<void(std::uint64_t)>& callback) {
    instance.u64CallbackPool[instance.cbCount] = callback;
    instance.u64CallbackMap[key].push_back(instance.cbCount);
    return instance.cbCount++;
  }

  void Settings::stopWatching(std::uint64_t id) {
    // idは重複しないからこれでok
    instance.strCallbackPool.erase(id);
    instance.u64CallbackPool.erase(id);
  }

  void Settings::callStrCallbacks(const QString& key, const QString& value) {
    for (const auto& id : strCallbackMap[key]) {
      if (const auto& cb = strCallbackPool[id]) {
        cb.value()(value);
      }
    }
  }
  void Settings::callU64Callbacks(const QString& key, std::uint64_t value) {
    for (const auto& id : u64CallbackMap[key]) {
      if (const auto& cb = u64CallbackPool[id]) {
        cb.value()(value);
      }
    }
  }
} // namespace octane::gui
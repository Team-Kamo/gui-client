#ifndef OCTANE_GUI_CLIENT_SETTINGS_H_
#define OCTANE_GUI_CLIENT_SETTINGS_H_

#include <QSettings>
#include <QString>
#include <functional>
#include <map>
#include <optional>
#include <vector>

namespace octane::gui {
  class Settings {
    using StrCallback = std::function<void(const QString&)>;
    using U64Callback = std::function<void(std::uint64_t)>;

    QSettings settings;

    std::map<size_t, std::optional<StrCallback>> strCallbackPool;
    std::map<size_t, std::optional<U64Callback>> u64CallbackPool;

    std::map<QString, std::vector<size_t>> strCallbackMap;
    std::map<QString, std::vector<size_t>> u64CallbackMap;
    size_t cbCount;

    Settings();
    Settings(const Settings&) = delete;
    Settings(Settings&&)      = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

    static Settings instance;

  public:
    static QString getAsStr(const QString& key);
    static std::uint64_t getAsU64(const QString& key);

    static QString setAsStr(const QString& key, const QString& value);
    static std::uint64_t setAsU64(const QString& key, std::uint64_t value);

    static std::uint64_t watchAsStr(
      const QString& key,
      const std::function<void(const QString&)>& callback);
    static std::uint64_t watchAsU64(
      const QString& key,
      const std::function<void(std::uint64_t)>& callback);

    static void stopWatching(std::uint64_t id);

  private:
    void callStrCallbacks(const QString& key, const QString& value);
    void callU64Callbacks(const QString& key, std::uint64_t value);
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_SETTINGS_H_